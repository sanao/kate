/*  SPDX-License-Identifier: LGPL-2.0-or-later

    SPDX-FileCopyrightText: 2019 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "session_manager_test.h"
#include "kateapp.h"
#include "katesessionmanager.h"
#include "session/store.h"

#include <KConfig>
#include <KConfigGroup>
#include <KSharedConfig>

#include <QCommandLineParser>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTimer>
#include <QtTestWidgets>

QTEST_MAIN(KateSessionManagerTest)

void KateSessionManagerTest::initTestCase()
{
    /**
     * init resources from our static lib
     */
    Q_INIT_RESOURCE(kate);

    QStandardPaths::setTestModeEnabled(true);
}

void KateSessionManagerTest::init()
{
    m_tempdir = new QTemporaryDir;
    QVERIFY(m_tempdir->isValid());
    m_store = new Kate::Session::Store(m_tempdir->path());
}

void KateSessionManagerTest::cleanup()
{
    QString cnf = QStandardPaths::locate(QStandardPaths::GenericConfigLocation, KSharedConfig::openConfig()->name());
    if (!cnf.isEmpty()) {
        QFile::remove(cnf);
    }

    delete m_tempdir;
    delete m_app;
}

void KateSessionManagerTest::basic()
{
    /**
     * test basic interractions between KateApp and SessionManager
     */
    m_app = new KateApp(arguments(), m_store);
    KateSessionManager *manager = m_app->sessionManager();

    QCOMPARE(manager->sessionList().size(), 0);
    manager->activateNewSession();
    QCOMPARE(manager->sessionList().size(), 1);
    QVERIFY(manager->activeSession());
}

void KateSessionManagerTest::coldInit()
{
    /**
     * Kate should create new session when initiate for the first time
     */
    m_app = new KateApp(arguments(), m_store);
    QVERIFY(m_app->init());

    QCOMPARE(m_store->sessions().size(), 1);
}

void KateSessionManagerTest::secondBoot()
{
    /**
     * we should get the same session after rebooting kate
     */
    m_app = new KateApp(arguments(), m_store);
    m_app->init();
    m_app->sessionManager()->saveActiveSession(true);
    QString id = m_app->sessionManager()->activeSession()->id();
    delete m_app;

    m_store = new Kate::Session::Store(m_tempdir->path());
    QCOMPARE(m_store->sessions().size(), 1);

    m_app = new KateApp(arguments(), m_store);
    QVERIFY(m_app->init());
    QCOMPARE(m_store->sessions().size(), 1);

    QCOMPARE(m_app->sessionManager()->activeSession()->id(), id);
}

void KateSessionManagerTest::optionNewSession()
{
    /**
     * starting kate with new-session option will create new session
     */
    m_app = new KateApp(arguments(), m_store);
    m_app->init();
    m_app->sessionManager()->saveActiveSession(true);
    delete m_app;

    m_store = new Kate::Session::Store(m_tempdir->path());
    QCOMPARE(m_store->sessions().size(), 1);

    m_app = new KateApp(arguments(QStringList(QLatin1String("--new-session"))), m_store);
    QVERIFY(m_app->init());
    QCOMPARE(m_store->sessions().size(), 2);
}

void KateSessionManagerTest::openingFiles()
{
    /**
     * kate should open file in last session
     */
    m_app = new KateApp(arguments(), m_store);
    m_app->init();
    m_app->sessionManager()->saveActiveSession(true);
    QCOMPARE(m_app->activeMainWindow()->views().size(), 1);
    delete m_app;

    m_store = new Kate::Session::Store(m_tempdir->path());
    QTemporaryFile f;
    f.open();
    m_app = new KateApp(arguments(QStringList() << f.fileName()), m_store);
    m_app->init();

    QTest::qWait(0);

    QCOMPARE(m_store->sessions().size(), 1);
    QCOMPARE(m_app->activeMainWindow()->views().size(), 1);
    QCOMPARE(m_app->activeMainWindow()->activeView()->document()->url().toLocalFile(), f.fileName());
}

void KateSessionManagerTest::openFilesInNamedSession()
{
    /**
     * Kate should open files in requested named session
     */
    QString sessionName = QLatin1String("test");

    auto s = m_store->getNewSession();
    s->setName(sessionName);
    m_store->releaseSession(s);

    s = m_store->getNewSession();
    s->setName(QLatin1String("extra"));
    m_store->releaseSession(s);

    QTemporaryFile f;
    f.open();

    QStringList args{QLatin1String("-s"), sessionName, f.fileName()};

    m_app = new KateApp(arguments(args), m_store);
    QVERIFY(m_app->init());

    QCOMPARE(m_app->sessionManager()->activeSession()->name(), sessionName);
}

void KateSessionManagerTest::activateSessionByName()
{
    /**
     * opening session by name works
     */
    const QString sessionName = QStringLiteral("hello_world");

    KateSession::Ptr s = m_store->getNewSession();
    s->setName(sessionName);
    m_store->releaseSession(s);

    s = m_store->getNewSession();
    s->setName(sessionName + QLatin1String("2"));
    m_store->releaseSession(s);

    m_app = new KateApp(arguments(), m_store);
    m_app->init();

    QVERIFY(m_app->sessionManager()->activeSession()->name() != sessionName);
    m_app->sessionManager()->activateSessionByName(sessionName);
    QCOMPARE(m_app->sessionManager()->activeSession()->name(), sessionName);
}

void KateSessionManagerTest::renameSession()
{
    /**
     * session manager is able to rename sessions
     */
    const QString sessionName = QLatin1String("new session name");

    m_app = new KateApp(arguments(), m_store);
    m_app->init();

    QCOMPARE(m_store->sessions().size(), 1);

    KateSessionManager *manager = m_app->sessionManager();

    QVERIFY(manager->activeSession()->name() != sessionName);
    manager->renameSession(manager->activeSession(), sessionName);
    QCOMPARE(manager->activeSession()->name(), sessionName);
}

void KateSessionManagerTest::deleteSession()
{
    /**
     * it's possible to delete session which is not active
     */
    KateSession::Ptr s = m_store->getNewSession();
    s->setName(QLatin1String("Session to delete"));

    m_app = new KateApp(arguments(), m_store);
    m_app->init();

    QCOMPARE(m_store->sessions().size(), 2);
    m_app->sessionManager()->deleteSession(s);
    QCOMPARE(m_store->sessions().size(), 1);
}

void KateSessionManagerTest::deleteLastSession()
{
    /**
     * session manager won't delete last session
     */
    m_app = new KateApp(arguments(), m_store);
    m_app->init();
    KateSession::Ptr s = m_app->sessionManager()->sessionList()[0];
    QVERIFY(!m_app->sessionManager()->deleteSession(s));
    QCOMPARE(m_store->sessions().size(), 1);
}

#if false
/**
 * kateapp won't play nice when destroyed and running event loop afterwards
 * This scenario works but only when executed as single test case in program
 */
void KateSessionManagerTest::deletingSessionFilesUnderRunningApp()
{
    /**
     * kate will be ok when deleting session files under it
     */
    m_app = new KateApp(arguments(), m_store);
    m_app->init();

    QString sid_other = m_app->sessionManager()->activeSession()->id();
    m_app->sessionManager()->saveActiveSession();

    m_app->sessionManager()->activateNewSession();
    QString sid_active = m_app->sessionManager()->activeSession()->id();
    m_app->sessionManager()->saveActiveSession();

    QVERIFY(sid_active != sid_other);
    QDir dir_active(m_tempdir->filePath(sid_active)), dir_other(m_tempdir->filePath(sid_other));

    QVERIFY(dir_active.exists());
    QVERIFY(dir_other.exists());

    dir_other.removeRecursively();
    QTest::qWait(250); // ad hoc wait till the FS notification propagates
    QCOMPARE(m_store->sessions().size(), 1);

    dir_active.removeRecursively();
    QTest::qWait(250);
    QCOMPARE(m_store->sessions().size(), 1);
}
#endif

QCommandLineParser &KateSessionManagerTest::arguments(const QStringList &args)
{
    m_parser.reset(new QCommandLineParser());
    m_parser->addOption(QCommandLineOption(QLatin1String("encoding")));
    m_parser->addOption(QCommandLineOption(QLatin1String("tempfile")));
    m_parser->addOption(QCommandLineOption(QLatin1String("stdin")));
    m_parser->addOption(QCommandLineOption(QStringList({QLatin1String("start"), QLatin1String("s")}), QString(), QLatin1String("session")));
    m_parser->addOption(QCommandLineOption(QLatin1String("startanon")));
    m_parser->addOption(QCommandLineOption(QLatin1String("new-session")));

    m_parser->parse(QStringList(QLatin1String("kate")) + args);
    return *(m_parser.get());
}
