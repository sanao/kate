/*
    SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "dapclientplugin.h"

DAPClientPlugin::DAPClientPlugin(QObject *parent, const QList<QVariant> &)
    : KTextEditor::Plugin(parent)
{
}
DAPClientPlugin::~DAPClientPlugin()
{
}

QObject *DAPClientPlugin::createView(KTextEditor::MainWindow *mainWindow)
{
    return nullptr;
}
