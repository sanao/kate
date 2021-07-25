/*  SPDX-License-Identifier: LGPL-2.0-or-later

    SPDX-FileCopyrightText: 2021 Michal Humpula <kde@hudrydum.cz>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef __KATE_SESSION_STORE_H__
#define __KATE_SESSION_STORE_H__

#include "katesession.h"

#include <KDirWatch>
#include <QHash>
#include <QObject>

namespace Kate::Session
{
class Store : public QObject
{
    Q_OBJECT

public:
    Store(const QString &dir = QString());

    /**
     * FIXME: ideally would not allow direct access to session list
     */
    QList<KateSession::Ptr> sessions() const
    {
        return m_sessions.values();
    }

    KateSession::Ptr getSession(const QString &id);
    KateSession::Ptr getSessionByName(const QString &name);
    KateSession::Ptr getNewSession();
    KateSession::Ptr duplicateSession(KateSession::Ptr session, const QString &name = QString());
    void releaseSession(KateSession::Ptr session);

    void deleteSession(KateSession::Ptr session);
    void sync(KateSession::Ptr session);

    bool sessionNameExist(const QString &name) const;

Q_SIGNALS:
    /**
     * Emitted whenever the session list has changed.
     * @see sessionList()
     */
    void sessionListChanged();

private Q_SLOTS:
    /**
     * trigger update of sessions from disk store
     */
    void reread();

private:
    QStringList sessionsOnDisk() const;

    bool isActiveSession(KateSession::Ptr session) const;

    KateSession::Ptr readSession(const QString &id);
    QString configFileForId(const QString &id) const;

private:
    QString m_dir;
    KDirWatch m_dirWatch;
    QHash<QString, KateSession::Ptr> m_sessions;
    QList<KateSession::Ptr> m_activeSessions;
};

}

#endif
