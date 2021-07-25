/*  SPDX-License-Identifier: LGPL-2.0-or-later

    SPDX-FileCopyrightText: 2005 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "katesession.h"

#include <KConfig>
#include <KConfigGroup>

#include <QCollator>

#include "namegenerator.h"

static const QLatin1String opGroupName("Open Documents");
static const QLatin1String keyCount("Count");
static const QLatin1String metaGroup("Meta");
static const QLatin1String nameKey("Name");

KateSession::KateSession(const QString &id, KConfig *config)
    : m_id(id)
    , m_config(config)
    , m_timestamp(QDateTime::currentDateTime())
{
    m_documents = config->group(opGroupName).readEntry(keyCount, 0);

    m_name = config->group(metaGroup).readEntry(nameKey, QString());
    if (m_name.isEmpty()) {
        setName(getRandomName());
    }
}

void KateSession::setDocuments(const unsigned int number)
{
    m_config->group(opGroupName).writeEntry(keyCount, number);
    m_documents = number;
}

void KateSession::setName(const QString &name)
{
    m_name = name;
    m_config->group(metaGroup).writeEntry(nameKey, name);
}

KConfig *KateSession::config()
{
    return m_config.get();
}

bool KateSession::compareByName(const KateSession::Ptr &s1, const KateSession::Ptr &s2)
{
    return QCollator().compare(s1->name(), s2->name()) == -1;
}

bool KateSession::compareByTimeDesc(const KateSession::Ptr &s1, const KateSession::Ptr &s2)
{
    return s1->timestamp() > s2->timestamp();
}
