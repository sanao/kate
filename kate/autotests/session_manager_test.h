/*  SPDX-License-Identifier: LGPL-2.0-or-later

    SPDX-FileCopyrightText: 2019 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KATE_SESSION_MANAGER_TEST_H
#define KATE_SESSION_MANAGER_TEST_H

#include <QCommandLineParser>
#include <QObject>
#include <memory>

namespace Kate::Session
{
class Store;
}

class KateSessionManagerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void initTestCase();

    void basic();
    void coldInit();
    void secondBoot();

    void optionNewSession();
    void openingFiles();
    void openFilesInNamedSession();

    void activateSessionByName();
    void renameSession();

    void deleteSession();
    void deleteLastSession();

    // void deletingSessionFilesUnderRunningApp();

private:
    QCommandLineParser &arguments(const QStringList &args = QStringList());
    std::unique_ptr<QCommandLineParser> m_parser;

    class QTemporaryDir *m_tempdir;
    class KateApp *m_app;
    class Kate::Session::Store *m_store;
};

#endif
