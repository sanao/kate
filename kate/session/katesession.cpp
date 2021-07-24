/*  SPDX-License-Identifier: LGPL-2.0-or-later

    SPDX-FileCopyrightText: 2005 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "katesession.h"

#include "katedebug.h"
#include "katesessionmanager.h"

#include <KConfig>
#include <KConfigGroup>

#include <QCollator>
#include <QFile>
#include <QFileInfo>
#include <QUuid>

#include "namegenerator.h"

static const QLatin1String opGroupName("Open Documents");
static const QLatin1String keyCount("Count");

KateSession::KateSession(const QString &id, KConfig *config)
    : m_id(id)
    , m_config(config)
{
#if false
    Q_ASSERT(!m_file.isEmpty());

    if (_config) { // copy data from config instead
        m_config.reset(_config->copyTo(m_file));
    } else if (!QFile::exists(m_file)) { // given file exists, use it to load some stuff
        qCDebug(LOG_KATE) << "Warning, session file not found: " << m_file;
        return;
    }

    m_timestamp = QFileInfo(m_file).lastModified();
#endif

    // get the document count
    m_documents = config->group(opGroupName).readEntry(keyCount, 0);

    m_name = config->group(QLatin1String("meta")).readEntry(QLatin1String("Name"), QString());
    if (m_name.isEmpty()) {
        m_name = getRandomName();
    }
}

void KateSession::setDocuments(const unsigned int number)
{
    config()->group(opGroupName).writeEntry(keyCount, number);
    m_documents = number;
}

#if false
void KateSession::setFile(const QString &filename)
{
    if (m_config) {
        KConfig *cfg = m_config->copyTo(filename);
        m_config.reset(cfg);
    }

    m_file = filename;
}
#endif

KConfig *KateSession::config()
{
    return m_config;
}

KateSession::Ptr KateSession::create()
{
    const QString rnd = QUuid::createUuid().toString(QUuid::WithoutBraces);
    KConfig *config = new KConfig();
    return Ptr(new KateSession(rnd, config));
}

KateSession::Ptr KateSession::create(const QString &id, KConfig *config)
{
    return Ptr(new KateSession(id, config));
}

KateSession::Ptr KateSession::duplicate(KateSession::Ptr session, const QString &name)
{
    const QString rnd = QUuid::createUuid().toString();
    // FIXME: dupe kconfig here
    KateSession *s = new KateSession(rnd, session->config());

    return Ptr(s);
}

bool KateSession::compareByName(const KateSession::Ptr &s1, const KateSession::Ptr &s2)
{
    return QCollator().compare(s1->name(), s2->name()) == -1;
}

bool KateSession::compareByTimeDesc(const KateSession::Ptr &s1, const KateSession::Ptr &s2)
{
    return s1->timestamp() > s2->timestamp();
}
