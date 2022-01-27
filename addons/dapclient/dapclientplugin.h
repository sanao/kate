/*
    SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>

    SPDX-License-Identifier: MIT
*/

#ifndef YOURMOMPLUGIN_H
#define YOURMOMPLUGIN_H

#include <QMap>
#include <QUrl>
#include <QVariant>

#include <KTextEditor/Plugin>

class DAPClientPlugin : public KTextEditor::Plugin
{
    Q_OBJECT

public:
    explicit DAPClientPlugin(QObject *parent = nullptr, const QList<QVariant> & = QList<QVariant>());
    ~DAPClientPlugin() override;

    QObject *createView(KTextEditor::MainWindow *mainWindow) override;

    // int configPages() const override;
    // KTextEditor::ConfigPage *configPage(int number = 0, QWidget *parent = nullptr) override;

private:
Q_SIGNALS:
    // signal settings update
    void update() const;
};

#endif
