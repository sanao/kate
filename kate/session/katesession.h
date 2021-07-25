/*  SPDX-License-Identifier: LGPL-2.0-or-later

    SPDX-FileCopyrightText: 2005 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef __KATE_SESSION_H__
#define __KATE_SESSION_H__

#include "katetests_export.h"

#include <QDateTime>
#include <QExplicitlySharedDataPointer>
#include <QString>

#include <memory>

class KConfig;

namespace Kate::Session
{
class Store;
}

class KATE_TESTS_EXPORT KateSession : public QSharedData
{
public:
    /**
     * Define a Shared-Pointer type
     */
    typedef QExplicitlySharedDataPointer<KateSession> Ptr;

public:

    /**
     * session name
     * @return name for this session
     */
    const QString &name() const
    {
        return m_name;
    }

    void setName(const QString &name);

    /**
     * session config
     * on first access, will create the config object, delete will be done automagic
     * return 0 if we have no file to read config from atm
     * @return correct KConfig, never null
     * @note never delete configRead(), because the return value might be
     *       KSharedConfig::openConfig(). Only delete the member variables directly.
     */
    KConfig *config();

    /**
     * count of documents in this session
     * @return documents count
     */
    unsigned int documents() const
    {
        return m_documents;
    }

    /**
     * update \p number of opened documents in session
     */
    void setDocuments(const unsigned int number);

    /**
     * returns last save time of this session
     */
    const QDateTime &timestamp() const
    {
        return m_timestamp;
    }

    const QString &id() const
    {
        return m_id;
    }

    /**
     * Factories
     */
public:
    static bool compareByName(const KateSession::Ptr &s1, const KateSession::Ptr &s2);
    static bool compareByTimeDesc(const KateSession::Ptr &s1, const KateSession::Ptr &s2);

private:
    friend class Kate::Session::Store;

    /**
     * create a session from given @file
     * @param id id of session
     * @param config if specified, the session will copy configuration from the KConfig instead of opening the file
     */
    KateSession(const QString &id, KConfig *config);

    void setTimestamp(const QDateTime &ts = QDateTime::currentDateTime())
    {
        m_timestamp = ts;
    }

private:
    QString m_id;
    QString m_name;
    unsigned int m_documents;
    std::unique_ptr<KConfig> m_config;
    QDateTime m_timestamp;
};

#endif
