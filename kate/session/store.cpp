
#include "store.h"

#include <KConfig>
#include <QDir>
#include <QStandardPaths>
#include <QUuid>

#include <KConfigGroup>
#include <QUrl>

#ifndef Q_OS_WIN
#include <unistd.h>
#endif

static const QString SessionFileName = QStringLiteral("katesession");

using namespace Kate::Session;

Store::Store(const QString &dir)
    : m_dir(dir)
    , m_dirWatch(this)
{
    if (m_dir.isEmpty()) {
        m_dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/kate/sessions");
    }

    QDir().mkpath(m_dir);

    m_dirWatch.addDir(m_dir);
    connect(&m_dirWatch, &KDirWatch::dirty, this, &Store::reread);

    reread();

    /* migration mechanism
     * TODO: remove after some time
     */
    if (m_sessions.isEmpty()) {
        migrateSessions();
        reread();
    }
}

void Store::reread()
{
    const QStringList list = sessionsOnDisk();
    bool changed = false;

    // Add new sessions to our list
    for (const QString &sid : list) {
        if (!m_sessions.contains(sid)) {
            m_sessions.insert(sid, readSession(sid));
            changed = true;
        }
    }
    // Remove gone sessions from our list
    for (const QString &sid : m_sessions.keys()) {
        if ((list.indexOf(sid) < 0) && !isActiveSession(m_sessions.value(sid))) {
            m_sessions.remove(sid);
            changed = true;
        }
    }

    if (changed) {
        Q_EMIT sessionListChanged();
    }
}

QStringList Store::sessionsOnDisk() const
{
    QStringList sessionsOnDisk;

    const QStringList dirs = QDir(m_dir).entryList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &id : dirs) {
        const QString sessionFile = m_dir + QDir::separator() + id + QDir::separator() + SessionFileName;
        if (QFile::exists(sessionFile)) {
            sessionsOnDisk << id;
        }
    }

    return sessionsOnDisk;
}

bool Store::isActiveSession(KateSession::Ptr session) const
{
    return m_activeSessions.contains(session);
}

KateSession::Ptr Store::readSession(const QString &id)
{
    const QString file = configFileForId(id);
    KConfig *cnf = new KConfig(file, KConfig::SimpleConfig);
    KateSession *session = new KateSession(id, cnf);
    session->setTimestamp(QFileInfo(file).lastModified());
    return KateSession::Ptr(session);
}

QString Store::configFileForId(const QString &id) const
{
    return m_dir + QDir::separator() + id + QDir::separator() + SessionFileName;
}

KateSession::Ptr Store::getSession(const QString &id)
{
    if (m_sessions.contains(id)) {
        m_activeSessions << m_sessions[id];
        return m_sessions[id];
    } else {
        return KateSession::Ptr(); // FIXME: more or less returning error
    }
}

void Store::releaseSession(KateSession::Ptr session)
{
    m_activeSessions.removeAll(session);
}

KateSession::Ptr Store::getSessionByName(const QString &name)
{
    auto it = std::find_if(m_sessions.begin(), m_sessions.end(), [&](auto s) {
        return s->name() == name;
    });

    if (it == m_sessions.end()) {
        return KateSession::Ptr(); // FIXME: more or less returning error
    } else {
        m_activeSessions << it.value();
        return it.value();
    }
}

KateSession::Ptr Store::getNewSession()
{
    const QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    KConfig *cnf = new KConfig(configFileForId(id), KConfig::SimpleConfig);
    KateSession::Ptr session(new KateSession(id, cnf));
    m_sessions[id] = session;
    m_activeSessions << session;
    Q_EMIT sessionListChanged();
    return session;
}

KateSession::Ptr Store::duplicateSession(KateSession::Ptr session, const QString &name)
{
    const QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    KConfig *cnf = session->config()->copyTo(configFileForId(id));
    KateSession::Ptr _session(new KateSession(id, cnf));
    _session->setName(name);
    m_sessions[id] = _session;
    m_activeSessions << _session;
    Q_EMIT sessionListChanged();
    return _session;
}

void Store::deleteSession(KateSession::Ptr session)
{
    QDir dir(m_dir + QDir::separator() + session->id());
    dir.removeRecursively();
    m_sessions.remove(session->id());
    m_activeSessions.removeAll(session);

    Q_EMIT sessionListChanged();
}

void Store::sync(KateSession::Ptr session)
{
    QDir().mkpath(m_dir + QDir::separator() + session->id());
    session->config()->sync();

    /**
     * try to sync file to disk
     */
    QFile fileToSync(session->config()->name());
    if (fileToSync.open(QIODevice::ReadOnly)) {
#ifndef Q_OS_WIN
// ensure that the file is written to disk
#ifdef HAVE_FDATASYNC
        fdatasync(fileToSync.handle());
#else
        fsync(fileToSync.handle());
#endif
#endif
    }

    session->setTimestamp();
}

bool Store::sessionNameExist(const QString &name) const
{
    auto it = std::find_if(m_sessions.begin(), m_sessions.end(), [&](auto s) {
        return s->name() == name;
    });

    return it != m_sessions.end();
}

void Store::migrateSessions()
{
    QDir dir(m_dir, QStringLiteral("*.katesession"), QDir::Time);

    for (unsigned int i = 0; i < dir.count(); ++i) {
        QString sname = QUrl::fromPercentEncoding(dir[i].chopped(12).toLatin1());
        QString file = dir.filePath(dir[i]);

        KConfig c(file, KConfig::SimpleConfig);

        QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        QDir(m_dir).mkpath(id);

        KConfig nc;
        c.copyTo(configFileForId(id), &nc);
        nc.group("Meta").writeEntry("Name", sname);
        nc.sync();
    }
}
