//
// Description: Widget for configuring build targets
//
// SPDX-FileCopyrightText: 2011-2014 Kåre Särs <kare.sars@iki.fi>
//
//  SPDX-License-Identifier: LGPL-2.0-only

#include "targets.h"
#include <KLocalizedString>
#include <QCoreApplication>
#include <QDebug>
#include <QEvent>
#include <QIcon>
#include <QKeyEvent>
#include <qnamespace.h>

TargetsUi::TargetsUi(QObject *view, QWidget *parent)
    : QWidget(parent)
{
    proxyModel.setSourceModel(&targetsModel);

    targetLabel = new QLabel(i18n("Active target-set:"));
    targetCombo = new QComboBox(this);
    targetCombo->setToolTip(i18n("Select active target set"));
    targetCombo->setModel(&proxyModel);
    targetLabel->setBuddy(targetCombo);

    targetFilterEdit = new QLineEdit(this);
    targetFilterEdit->setPlaceholderText(i18n("Filter targets"));
    targetFilterEdit->setClearButtonEnabled(true);

    newTarget = new QToolButton(this);
    newTarget->setToolTip(i18n("Create new set of targets"));
    newTarget->setIcon(QIcon::fromTheme(QStringLiteral("document-new")));

    copyTarget = new QToolButton(this);
    copyTarget->setToolTip(i18n("Copy command or target set"));
    copyTarget->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));

    deleteTarget = new QToolButton(this);
    deleteTarget->setToolTip(i18n("Delete current target or current set of targets"));
    deleteTarget->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete")));

    addButton = new QToolButton(this);
    addButton->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    addButton->setToolTip(i18n("Add new target"));

    buildButton = new QToolButton(this);
    buildButton->setIcon(QIcon::fromTheme(QStringLiteral("run-build")));
    buildButton->setToolTip(i18n("Build selected target"));

    targetsView = new QTreeView(this);
    targetsView->setAlternatingRowColors(true);

    targetsView->setModel(&proxyModel);
    m_delegate = new TargetHtmlDelegate(view);
    targetsView->setItemDelegate(m_delegate);
    targetsView->setSelectionBehavior(QAbstractItemView::SelectItems);
    targetsView->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    targetsView->expandAll();
    targetsView->resizeColumnToContents(0);

    QHBoxLayout *tLayout = new QHBoxLayout();

    tLayout->addWidget(targetLabel);
    tLayout->addWidget(targetCombo);
    tLayout->addWidget(targetFilterEdit);
    tLayout->addWidget(buildButton);
    tLayout->addSpacing(20);
    tLayout->addWidget(addButton);
    tLayout->addWidget(newTarget);
    tLayout->addWidget(copyTarget);
    tLayout->addWidget(deleteTarget);
    tLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(tLayout);
    layout->addWidget(targetsView);

    connect(targetCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &TargetsUi::targetSetSelected);
    connect(targetsView->selectionModel(), &QItemSelectionModel::currentChanged, this, &TargetsUi::targetActivated);
    // connect(targetsView, SIGNAL(clicked(QModelIndex)), this, SLOT(targetActivated(QModelIndex)));

    connect(targetFilterEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        proxyModel.setFilter(text);
        targetsView->expandAll();
    });

    targetsView->installEventFilter(this);
    targetFilterEdit->installEventFilter(this);
}

void TargetsUi::targetSetSelected(int index)
{
    // qDebug() << index;
    targetsView->collapseAll();
    QModelIndex rootItem = proxyModel.index(index, 0);

    targetsView->setExpanded(rootItem, true);
    targetsView->setCurrentIndex(proxyModel.index(0, 0, rootItem));
}

void TargetsUi::targetActivated(const QModelIndex &index)
{
    // qDebug() << index;
    if (!index.isValid()) {
        return;
    }
    QModelIndex rootItem = index;
    if (rootItem.parent().isValid()) {
        rootItem = rootItem.parent();
    }

    targetCombo->setCurrentIndex(rootItem.row());
}

bool TargetsUi::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (obj == targetsView) {
            if (((keyEvent->key() == Qt::Key_Return) || (keyEvent->key() == Qt::Key_Enter)) && m_delegate && !m_delegate->isEditing()) {
                Q_EMIT enterPressed();
                return true;
            }
        }
        if (obj == targetFilterEdit) {
            switch (keyEvent->key()) {
            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_PageUp:
            case Qt::Key_PageDown:
            case Qt::Key_Return:
            case Qt::Key_Enter:
                QCoreApplication::sendEvent(targetsView, event);
                return true;
            case Qt::Key_Left:
            case Qt::Key_Right:
            case Qt::Key_F2:
                // NOTE: I failed to find a generic "platform edit key" shortcut, but it seems
                // Key_F2 is hard-coded on non-OSX and Return/Enter on OSX in:
                // void QAbstractItemView::keyPressEvent(QKeyEvent *event)
                if (targetFilterEdit->text().isEmpty()) {
                    QCoreApplication::sendEvent(targetsView, event);
                    return true;
                }
                break;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}
