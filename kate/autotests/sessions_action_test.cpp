/*  SPDX-License-Identifier: LGPL-2.0-or-later

    SPDX-FileCopyrightText: 2019 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sessions_action_test.h"
#include "kateapp.h"
#include "katesessionmanager.h"
#include "katesessionsaction.h"
#include "session/store.h"

#include <KSharedConfig>
#include <QActionGroup>
#include <QCommandLineParser>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QtTestWidgets>

QTEST_MAIN(KateSessionsActionTest)

void KateSessionsActionTest::initTestCase()
{
    /**
     * init resources from our static lib
     */
    Q_INIT_RESOURCE(kate);

    QStandardPaths::setTestModeEnabled(true);
}

void KateSessionsActionTest::cleanupTestCase()
{
    QString cnf = QStandardPaths::locate(QStandardPaths::GenericConfigLocation, KSharedConfig::openConfig()->name());
    if (!cnf.isEmpty()) {
        QFile::remove(cnf);
    }
}

void KateSessionsActionTest::init()
{
    m_tempdir = new QTemporaryDir;
    QVERIFY(m_tempdir->isValid());

    // we need an application object, as session loading will trigger modifications to that
    m_store = new Kate::Session::Store(m_tempdir->path());
    m_app = new KateApp(QCommandLineParser(), m_store);
    m_manager = m_app->sessionManager();
    m_manager->activateNewSession();

    m_ac = new KateSessionsAction(QStringLiteral("menu"), this, m_manager);
}

void KateSessionsActionTest::cleanup()
{
    delete m_app;
    delete m_ac;
    delete m_tempdir;
}
void KateSessionsActionTest::basic()
{
    m_ac->slotAboutToShow();
    QCOMPARE(m_ac->isEnabled(), true);

    QList<QAction *> actions = m_ac->sessionsGroup->actions();
    QCOMPARE(actions.size(), 1);
}

void KateSessionsActionTest::limit()
{
    for (int i = 0; i < 14; i++) {
        m_manager->activateNewSession();
    }

    QCOMPARE(m_manager->sessionList().size(), 15);
    QCOMPARE(m_ac->isEnabled(), true);

    m_ac->slotAboutToShow();

    QList<QAction *> actions = m_ac->sessionsGroup->actions();
    QCOMPARE(actions.size(), 10);
}

void KateSessionsActionTest::timesorted()
{
    QDateTime now = QDateTime::currentDateTime();
    m_manager->sessionList()[0]->setName(QLatin1String("first"));

    for (int i = 1; i < 3; i++) {
        auto s = m_store->getNewSession();
        s->setTimestamp(now.addDays(-i * 24));
        s->setName(QString(QLatin1String("s %1")).arg(i));
    }

    QCOMPARE(m_manager->sessionList().size(), 3);

    m_ac->slotAboutToShow();

    QList<QAction *> actions = m_ac->sessionsGroup->actions();
    QCOMPARE(actions.size(), 3);

    QCOMPARE(m_store->getSession(actions[0]->data().toString())->name(), QLatin1String("first"));
    QCOMPARE(m_store->getSession(actions[1]->data().toString())->name(), QLatin1String("s 1"));
    QCOMPARE(m_store->getSession(actions[2]->data().toString())->name(), QLatin1String("s 2"));
}
