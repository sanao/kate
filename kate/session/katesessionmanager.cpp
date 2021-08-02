/*  SPDX-License-Identifier: LGPL-2.0-or-later

    SPDX-FileCopyrightText: 2005 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "katesessionmanager.h"

#include "katesessionmanagedialog.h"

#include "kateapp.h"
#include "katepluginmanager.h"
#include "katerunninginstanceinfo.h"

#include <KConfigGroup>
#include <KDesktopFile>
#include <KLocalizedString>
#include <KMessageBox>
#include <KService>
#include <KSharedConfig>
#include <KShell>

#include <QCryptographicHash>
#include <QDBusConnectionInterface>
#include <QDBusReply>
#include <QInputDialog>
#include <QUrl>

using namespace Kate::Session;

// BEGIN KateSessionManager

KateSessionManager::KateSessionManager(QObject *parent, Store *store)
    : QObject(parent)
    , m_store(store)
{
    if (!m_store) {
        m_store.reset(new Store());
    }

    connect(m_store.get(), &Store::sessionListChanged, this, &KateSessionManager::updateSessionList);
}

void KateSessionManager::updateSessionList()
{
    updateJumpListActions();
    Q_EMIT sessionListChanged();
}

bool KateSessionManager::activateSession(KateSession::Ptr session, const bool closeAndSaveLast, const bool loadNew)
{
    if (activeSession() == session) {
        return true;
    }

    // check if the requested session is already open in another instance
    KateRunningInstanceMap instances;
    if (!fillinRunningKateAppInstances(&instances)) {
        KMessageBox::error(nullptr, i18n("Internal error: there is more than one instance open for a given session."));
        return false;
    }

    if (instances.find(session->id()) != instances.end()) {
        if (KMessageBox::questionYesNo(nullptr,
                                       i18n("Session '%1' is already opened in another kate instance, change there instead of reopening?", session->name()),
                                       QString(),
                                       KStandardGuiItem::yes(),
                                       KStandardGuiItem::no(),
                                       QStringLiteral("katesessionmanager_switch_instance"))
            == KMessageBox::Yes) {
            instances[session->id()].dbus_if->call(QStringLiteral("activate"));
            return false;
        }
    }

    // try to close and save last session
    if (closeAndSaveLast) {
        if (KateApp::self()->activeKateMainWindow()) {
            if (!KateApp::self()->activeKateMainWindow()->queryClose_internal()) {
                return true;
            }
        }

        // save last session or not?
        saveActiveSession();

        // really close last
        KateApp::self()->documentManager()->closeAllDocuments();
    }

    // set the new session
    m_store->releaseSession(m_activeSession);
    m_activeSession = session;

    // there is one case in which we don't want the restoration and that is
    // when restoring session from session manager.
    // In that case the restore is handled by the caller
    if (loadNew) {
        loadSession(session);
    }

    Q_EMIT sessionChanged();
    return true;
}

void KateSessionManager::loadSession(const KateSession::Ptr &session, bool loadDocs) const
{
    // open the new session
    KSharedConfigPtr sharedConfig = KSharedConfig::openConfig();
    KConfig *sc = session->config();

    // if we have no session config object, try to load the default
    // (anonymous/unnamed sessions)
    // load plugin config + plugins
    KateApp::self()->pluginManager()->loadConfig(sc);

    if (loadDocs) {
        KateApp::self()->documentManager()->restoreDocumentList(sc);
    }

    // window config
    KConfigGroup c(sharedConfig, "General");

    KConfig *cfg = sc;
    bool delete_cfg = false;

    if (c.readEntry("Restore Window Configuration", true)) {
        int wCount = cfg->group("Open MainWindows").readEntry("Count", 1);

        for (int i = 0; i < wCount; ++i) {
            if (i >= KateApp::self()->mainWindowsCount()) {
                KateApp::self()->newMainWindow(cfg, QStringLiteral("MainWindow%1").arg(i));
            } else {
                KateApp::self()->mainWindow(i)->readProperties(KConfigGroup(cfg, QStringLiteral("MainWindow%1").arg(i)));
            }

            KateApp::self()->mainWindow(i)->restoreWindowConfig(KConfigGroup(cfg, QStringLiteral("MainWindow%1 Settings").arg(i)));
        }

        // remove mainwindows we need no longer...
        if (wCount > 0) {
            while (wCount < KateApp::self()->mainWindowsCount()) {
                delete KateApp::self()->mainWindow(KateApp::self()->mainWindowsCount() - 1);
            }
        }
    } else {
        // load recent files for all existing windows, see bug 408499
        for (int i = 0; i < KateApp::self()->mainWindowsCount(); ++i) {
            KateApp::self()->mainWindow(i)->loadOpenRecent(cfg);
        }
    }

    // ensure we have at least one window, always! load recent files for it, too, see bug 408499
    if (KateApp::self()->mainWindowsCount() == 0) {
        auto w = KateApp::self()->newMainWindow();
        w->loadOpenRecent(cfg);
    }

    if (delete_cfg) {
        delete cfg;
    }

    // we shall always have some existing windows here!
    Q_ASSERT(KateApp::self()->mainWindowsCount() > 0);
}

bool KateSessionManager::activateSessionByName(const QString &name)
{
    KateSession::Ptr s = m_store->getSessionByName(name);

    if (!s) {
        return false;
    }

    return activateSession(s, false);
}

bool KateSessionManager::activateSessionById(const QString &id, const bool closeAndSaveLast, const bool loadNew)
{
    KateSession::Ptr s = m_store->getSession(id);

    if (!s) {
        return false;
    }

    return activateSession(s, closeAndSaveLast, loadNew);
}

void KateSessionManager::activateNewSession()
{
    // FIXME: load some defaults
    m_store->releaseSession(m_activeSession);
    m_activeSession = m_store->getNewSession();

    KateApp::self()->documentManager()->closeAllDocuments();
    loadSession(m_activeSession, false);
    Q_EMIT sessionChanged();
}

void KateSessionManager::activateNewSessionFrom(const KateSession::Ptr &session)
{
    const QString name(suggestNewSessionName(session->name()));
    activateSession(m_store->duplicateSession(session, name));
}

bool KateSessionManager::deleteSession(KateSession::Ptr session)
{
    if (sessionIsActive(session)) {
        return false;
    }

    KConfigGroup c(KSharedConfig::openConfig(), "General");
    if (c.readEntry("Last Session") == session->id()) {
        c.writeEntry("Last Session", QString());
        c.sync();
    }

    m_store->deleteSession(session);

    return true;
}

QString KateSessionManager::copySession(const KateSession::Ptr &session, const QString &newName)
{
    Q_ASSERT(session);

    const QString name = askForNewSessionName(session, newName);

    if (name.isEmpty()) {
        return name;
    }

    KateSession::Ptr ns = m_store->duplicateSession(session, name);
    m_store->sync(ns);

    return name;
}

QString KateSessionManager::renameSession(KateSession::Ptr session, const QString &newName)
{
    Q_ASSERT(session);

    const QString name = askForNewSessionName(session, newName);
    session->setName(name);
    m_store->sync(session);
    return name;
}

void KateSessionManager::saveSessionTo(KConfig *sc) const
{
    // Clear the session file to avoid to accumulate outdated entries
    for (const auto &group : sc->groupList()) {
        // Don't delete groups for loaded documents that have
        // ViewSpace config in session but do not have any views.
        if (isViewLessDocumentViewSpaceGroup(group)) {
            sc->deleteGroup(group);
        }
    }

    // save plugin configs and which plugins to load
    KateApp::self()->pluginManager()->writeConfig(sc);

    // save document configs + which documents to load
    KateApp::self()->documentManager()->saveDocumentList(sc);

    sc->group("Open MainWindows").writeEntry("Count", KateApp::self()->mainWindowsCount());

    // save config for all windows around ;)
    bool saveWindowConfig = KConfigGroup(KSharedConfig::openConfig(), "General").readEntry("Restore Window Configuration", true);
    for (int i = 0; i < KateApp::self()->mainWindowsCount(); ++i) {
        KConfigGroup cg(sc, QStringLiteral("MainWindow%1").arg(i));
        // saveProperties() handles saving the "open recent" files list
        KateApp::self()->mainWindow(i)->saveProperties(cg);
        if (saveWindowConfig) {
            KateApp::self()->mainWindow(i)->saveWindowConfig(KConfigGroup(sc, QStringLiteral("MainWindow%1 Settings").arg(i)));
        }
    }
}

bool KateSessionManager::saveActiveSession(bool rememberAsLast)
{
    KateSession::Ptr session = activeSession();

    if (!session) {
        return false;
    }

    saveSessionTo(session->config());
    m_store->sync(session);

    if (rememberAsLast) {
        KSharedConfigPtr c = KSharedConfig::openConfig();
        c->group("General").writeEntry("Last Session", session->id());
        c->sync();
    }
    return true;
}

bool KateSessionManager::chooseSession()
{
    const KConfigGroup c(KSharedConfig::openConfig(), "General");

    // get last used session, default to default session
    const QString lastSession(c.readEntry("Last Session", QString()));
    const QString sesStart(c.readEntry("Startup Session", QString()));

    if (sesStart == QLatin1String("new")) {
        activateNewSession();
        return true;
    } else if (sesStart == QLatin1String("manual")) {
        return QScopedPointer<KateSessionManageDialog>(new KateSessionManageDialog(nullptr, lastSession))->exec();
    } else {
        /* default behaviour is to restore the session if possible */
        if (!lastSession.isEmpty()) {
            KateSession::Ptr s = m_store->getSession(lastSession);
            if (s) {
                return activateSession(s, false);
            } else {
                activateNewSession();
                return true;
            }
        } else {
            activateNewSession();
            return true;
        }
    }
}

void KateSessionManager::sessionNew()
{
    activateNewSession();
}

void KateSessionManager::sessionSave()
{
    saveActiveSession();
}

void KateSessionManager::sessionSaveAs()
{
    KateSession::Ptr session = activeSession();
    const QString newName = askForNewSessionName(session);

    if (newName.isEmpty()) {
        return;
    }

    m_store->sync(session);
    KateSession::Ptr ns = m_store->duplicateSession(session, newName);
    m_store->releaseSession(session);

    m_activeSession = ns;
    saveActiveSession();

    Q_EMIT sessionChanged();
}

QString KateSessionManager::askForNewSessionName(KateSession::Ptr session, const QString &newName)
{
    const QString messagePrompt = i18n("Session name:");
    const KLocalizedString messageExist = ki18n(
        "There is already an existing session with your chosen name: %1\n"
        "Please choose a different one.");
    const QString messageEmpty = i18n("To save a session, you must specify a name.");

    QString messageTotal = messagePrompt;
    QString name = newName;

    while (true) {
        QString preset = name;

        if (name.isEmpty()) {
            preset = suggestNewSessionName(session->name());
            messageTotal = messageEmpty + QLatin1String("\n\n") + messagePrompt;

        } else if (m_store->sessionNameExist(name)) {
            preset = suggestNewSessionName(name);
            if (preset.isEmpty()) {
                // Very unlikely, but as fall back we keep users input
                preset = name;
            }
            messageTotal = messageExist.subs(name).toString() + QLatin1String("\n\n") + messagePrompt;

        } else {
            return name;
        }

        QInputDialog dlg(KateApp::self()->activeKateMainWindow());
        dlg.setInputMode(QInputDialog::TextInput);
        dlg.setWindowTitle(i18n("Specify a new name for session: %1", session->name()));
        dlg.setLabelText(messageTotal);
        dlg.setTextValue(preset);
        dlg.resize(900, 100); // FIXME Calc somehow a proper size
        bool ok = dlg.exec();
        name = dlg.textValue();

        if (!ok) {
            return QString();
        }
    }
}

QString KateSessionManager::suggestNewSessionName(const QString &target) const
{
    const QString mask = QStringLiteral("%1 %2");
    QString name;

    for (int i = 2; i < 1000000; i++) { // Should be enough to get an unique name
        name = mask.arg(target).arg(i);

        if (!m_store->sessionNameExist(name)) {
            return name;
        }
    }

    return QString();
}

void KateSessionManager::sessionManage()
{
    QScopedPointer<KateSessionManageDialog>(new KateSessionManageDialog(KateApp::self()->activeKateMainWindow()))->exec();
}

bool KateSessionManager::sessionIsActive(const KateSession::Ptr session)
{
    // Try to avoid unneed action
    if (activeSession() == session) {
        return true;
    }

    QDBusConnectionInterface *i = QDBusConnection::sessionBus().interface();
    if (!i) {
        return false;
    }

    // look up all running kate instances and there sessions
    QDBusReply<QStringList> servicesReply = i->registeredServiceNames();
    QStringList services;
    if (servicesReply.isValid()) {
        services = servicesReply.value();
    }

    for (const QString &s : qAsConst(services)) {
        if (!s.startsWith(QLatin1String("org.kde.kate-"))) {
            continue;
        }

        KateRunningInstanceInfo rii(s);
        if (rii.valid && rii.sessionId == session->id()) {
            return true;
        }
    }

    return false;
}

KateSessionList KateSessionManager::sessionList()
{
    return m_store->sessions();
}

void KateSessionManager::updateJumpListActions() const
{
    QList<KateSession::Ptr> slist = m_store->sessions();
    std::sort(slist.begin(), slist.end(), KateSession::compareByTimeDesc);

    QStringList sessionList;
    std::transform(slist.begin(), slist.end(), std::back_inserter(sessionList), [](auto s) {
        return s->name();
    });

    KService::Ptr service = KService::serviceByStorageId(qApp->desktopFileName());
    if (!service) {
        return;
    }

    std::unique_ptr<KDesktopFile> df(new KDesktopFile(service->entryPath()));

    QStringList newActions = df->readActions();

    // try to keep existing custom actions intact, only remove our "Session" actions and add them back later
    newActions.erase(std::remove_if(newActions.begin(),
                                    newActions.end(),
                                    [](const QString &action) {
                                        return action.startsWith(QLatin1String("Session "));
                                    }),
                     newActions.end());

    // Limit the number of list entries we like to offer
    const int maxEntryCount = std::min(sessionList.count(), 10);

    // sessionList is ordered by time, but we like it alphabetical to avoid even more a needed update
    QStringList sessionSubList = sessionList.mid(0, maxEntryCount);
    sessionSubList.sort();

    // we compute the new group names in advance so we can tell whether we changed something
    // and avoid touching the desktop file leading to an expensive ksycoca recreation
    QStringList sessionActions;
    sessionActions.reserve(maxEntryCount);

    for (int i = 0; i < maxEntryCount; ++i) {
        sessionActions
            << QStringLiteral("Session %1").arg(QString::fromLatin1(QCryptographicHash::hash(sessionSubList.at(i).toUtf8(), QCryptographicHash::Md5).toHex()));
    }

    newActions += sessionActions;

    // nothing to do
    if (df->readActions() == newActions) {
        return;
    }

    const QString &localPath = service->locateLocal();
    if (service->entryPath() != localPath) {
        df.reset(df->copyTo(localPath));
    }

    // remove all Session action groups first to not leave behind any cruft
    for (const QString &action : df->readActions()) {
        if (action.startsWith(QLatin1String("Session "))) {
            // TODO is there no deleteGroup(KConfigGroup)?
            df->deleteGroup(df->actionGroup(action).name());
        }
    }

    for (int i = 0; i < maxEntryCount; ++i) {
        const QString &action = sessionActions.at(i); // is a transform of sessionSubList, so count and order is identical
        const QString &session = sessionSubList.at(i);

        KConfigGroup grp = df->actionGroup(action);
        grp.writeEntry(QStringLiteral("Name"), session);
        grp.writeEntry(QStringLiteral("Exec"), QStringLiteral("kate -s %1").arg(KShell::quoteArg(session))); // TODO proper executable name?
    }

    df->desktopGroup().writeXdgListEntry("Actions", newActions);
}

bool KateSessionManager::isViewLessDocumentViewSpaceGroup(const QString &group)
{
    if (!group.startsWith(QStringLiteral("MainWindow"))) {
        return false;
    }

    static const QRegularExpression re(QStringLiteral("^MainWindow\\d+\\-ViewSpace\\s\\d+\\s(.*)$"));
    QRegularExpressionMatch match = re.match(group);
    if (match.hasMatch()) {
        QUrl url(match.captured(1));
        auto *docMan = KateApp::self()->documentManager();
        auto *doc = docMan->findDocument(url);
        if (doc && doc->views().empty() && docMan->documentList().contains(doc)) {
            return true;
        }
    }
    return false;
}

// END KateSessionManager
