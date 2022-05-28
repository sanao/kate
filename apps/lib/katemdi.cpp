/*
    SPDX-FileCopyrightText: 2005 Christoph Cullmann <cullmann@kde.org>
    SPDX-FileCopyrightText: 2002, 2003 Joseph Wenninger <jowenn@kde.org>

    GUIClient partly based on ktoolbarhandler.cpp: SPDX-FileCopyrightText: 2002 Simon Hausmann <hausmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "katemdi.h"
#include "kateapp.h"

// #include "katedebug.h"

#include <KActionCollection>
#include <KActionMenu>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>
#include <KToolBar>
#include <KWindowConfig>
#include <KXMLGUIFactory>

#include <QContextMenuEvent>
#include <QDomDocument>
#include <QLabel>
#include <QMenu>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QStyle>
#include <QTimer>
#include <QVBoxLayout>

namespace KateMDI
{
// BEGIN TOGGLETOOLVIEWACTION
//
ToggleToolViewAction::ToggleToolViewAction(const QString &text, ToolView *tv, QObject *parent)
    : KToggleAction(text, parent)
    , m_tv(tv)
{
    connect(this, &ToggleToolViewAction::toggled, this, &ToggleToolViewAction::slotToggled);
    connect(m_tv, &ToolView::toolVisibleChanged, this, &ToggleToolViewAction::toolVisibleChanged);

    setChecked(m_tv->toolVisible());
}

void ToggleToolViewAction::toolVisibleChanged(bool)
{
    if (isChecked() != m_tv->toolVisible()) {
        setChecked(m_tv->toolVisible());
    }
}

void ToggleToolViewAction::slotToggled(bool t)
{
    if (t) {
        m_tv->mainWindow()->showToolView(m_tv);
        m_tv->setFocus();
    } else {
        m_tv->mainWindow()->hideToolView(m_tv);
    }
}

// END TOGGLETOOLVIEWACTION

// BEGIN GUICLIENT

static const QString actionListName = QStringLiteral("kate_mdi_view_actions");

GUIClient::GUIClient(MainWindow *mw)
    : QObject(mw)
    , KXMLGUIClient(mw)
    , m_mw(mw)
{
    connect(m_mw->guiFactory(), &KXMLGUIFactory::clientAdded, this, &GUIClient::clientAdded);
    const QString guiDescription = QStringLiteral(
        ""
        "<!DOCTYPE gui><gui name=\"kate_mdi_view_actions\">"
        "<MenuBar>"
        "    <Menu name=\"view\">"
        "        <ActionList name=\"%1\" />"
        "    </Menu>"
        "</MenuBar>"
        "</gui>");

    if (domDocument().documentElement().isNull()) {
        QString completeDescription = guiDescription.arg(actionListName);

        setXML(completeDescription, false /*merge*/);
    }

    m_sidebarButtonsMenu = new KActionMenu(i18n("Sidebar Buttons"), this);
    actionCollection()->addAction(QStringLiteral("kate_mdi_show_sidebar_buttons"), m_sidebarButtonsMenu);

    m_toolMenu = new KActionMenu(i18n("Tool &Views"), this);
    actionCollection()->addAction(QStringLiteral("kate_mdi_toolview_menu"), m_toolMenu);
    m_showSidebarsAction = new KToggleAction(i18n("Show Side&bars"), this);
    actionCollection()->addAction(QStringLiteral("kate_mdi_sidebar_visibility"), m_showSidebarsAction);
    actionCollection()->setDefaultShortcut(m_showSidebarsAction, Qt::CTRL | Qt::ALT | Qt::SHIFT | Qt::Key_F);

    m_showSidebarsAction->setChecked(m_mw->sidebarsVisible());
    connect(m_showSidebarsAction, &KToggleAction::toggled, m_mw, &MainWindow::setSidebarsVisible);

    m_hideToolViews = actionCollection()->addAction(QStringLiteral("kate_mdi_hide_toolviews"), m_mw, &MainWindow::hideToolViews);
    m_hideToolViews->setText(i18n("Hide All Tool Views"));

    m_toolMenu->addAction(m_showSidebarsAction);
    m_toolMenu->addAction(m_hideToolViews);
    QAction *sep_act = new QAction(this);
    sep_act->setSeparator(true);
    m_toolMenu->addAction(sep_act);

    // read shortcuts
    actionCollection()->setConfigGroup(QStringLiteral("Shortcuts"));
    actionCollection()->readSettings();

    actionCollection()->addAssociatedWidget(m_mw);
    const auto actions = actionCollection()->actions();
    for (QAction *action : actions) {
        action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    }

    // hide tool views menu for KWrite mode
    if (KateApp::isKWrite()) {
        m_toolMenu->setVisible(false);
    }
}

void GUIClient::updateSidebarsVisibleAction()
{
    m_showSidebarsAction->setChecked(m_mw->sidebarsVisible());
}

void GUIClient::registerToolView(ToolView *tv)
{
    QString aname = QLatin1String("kate_mdi_toolview_") + tv->id;

    // try to read the action shortcut

    auto shortcutsForActionName = [](const QString &aname) {
        QList<QKeySequence> shortcuts;
        KSharedConfigPtr cfg = KSharedConfig::openConfig();
        const QString shortcutString = cfg->group("Shortcuts").readEntry(aname, QString());
        const auto shortcutStrings = shortcutString.split(QLatin1Char(';'));
        for (const QString &shortcut : shortcutStrings) {
            shortcuts << QKeySequence::fromString(shortcut);
        }
        return shortcuts;
    };

    /** Show ToolView Action **/
    KToggleAction *a = new ToggleToolViewAction(i18n("Show %1", tv->text), tv, this);
    actionCollection()->setDefaultShortcuts(a, shortcutsForActionName(aname));
    actionCollection()->addAction(aname, a);

    m_toolMenu->addAction(a);

    auto &actionsForTool = m_toolToActions[tv];
    actionsForTool.push_back(a);

    /** Show Tab button in sidebar action **/
    aname = QStringLiteral("kate_mdi_show_toolview_button_") + tv->id;
    a = new KToggleAction(i18n("Show %1 Button", tv->text), this);
    a->setChecked(true);
    actionCollection()->setDefaultShortcuts(a, shortcutsForActionName(aname));
    actionCollection()->addAction(aname, a);
    connect(a, &KToggleAction::toggled, this, [toolview = QPointer<ToolView>(tv)](bool checked) {
        if (toolview) {
            const QSignalBlocker b(toolview);
            toolview->sidebar()->showWidgetTab(toolview, checked);
        }
    });
    connect(tv, &ToolView::tabButtonVisibleChanged, a, &QAction::setChecked);

    m_sidebarButtonsMenu->addAction(a);
    actionsForTool.push_back(a);

    updateActions();
}

void GUIClient::unregisterToolView(ToolView *tv)
{
    auto &actionsForTool = m_toolToActions[tv];
    if (actionsForTool.empty())
        return;

    for (auto *a : actionsForTool) {
        delete a;
    }
    m_toolToActions.erase(tv);

    updateActions();
}

void GUIClient::clientAdded(KXMLGUIClient *client)
{
    if (client == this) {
        updateActions();
    }
}

void GUIClient::updateActions()
{
    if (!factory()) {
        return;
    }

    unplugActionList(actionListName);

    QList<QAction *> addList;
    addList.append(m_toolMenu);
    addList.append(m_sidebarButtonsMenu);

    plugActionList(actionListName, addList);
}

// END GUICLIENT

// BEGIN TOOLVIEW

ToolView::ToolView(MainWindow *mainwin, Sidebar *sidebar, QWidget *parent)
    : QFrame(parent)
    , m_mainWin(mainwin)
    , m_sidebar(sidebar)
    , m_toolbar(nullptr)
    , m_toolVisible(false)
    , persistent(false)
{
    // try to fix resize policy
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    policy.setRetainSizeWhenHidden(true);
    setSizePolicy(policy);

    // per default vbox layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);

    // toolbar to collect actions
    m_toolbar = new KToolBar(this);
    m_toolbar->setVisible(false);
    m_toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    // ensure reasonable icons sizes, like e.g. the quick-open and co. icons
    // the normal toolbar sizes are TOO large, e.g. for scaled stuff even more!
    const int iconSize = style()->pixelMetric(QStyle::PM_ButtonIconSize, nullptr, this);
    m_toolbar->setIconSize(QSize(iconSize, iconSize));
}

QSize ToolView::sizeHint() const
{
    return size();
}

QSize ToolView::minimumSizeHint() const
{
    return QSize(160, 160);
}

bool ToolView::tabButtonVisible() const
{
    auto _this = const_cast<ToolView *>(this);
    return _this->sidebar()->tabVisible(_this);
}

ToolView::~ToolView()
{
    m_mainWin->toolViewDeleted(this);
}

void ToolView::setToolVisible(bool vis)
{
    if (m_toolVisible == vis) {
        return;
    }

    m_toolVisible = vis;
    Q_EMIT toolVisibleChanged(m_toolVisible);
}

bool ToolView::toolVisible() const
{
    return m_toolVisible;
}

void ToolView::childEvent(QChildEvent *ev)
{
    // set the widget to be focus proxy if possible
    if ((ev->type() == QEvent::ChildAdded) && qobject_cast<QWidget *>(ev->child())) {
        QWidget *widget = qobject_cast<QWidget *>(ev->child());
        setFocusProxy(widget);
        layout()->addWidget(widget);
    }

    QFrame::childEvent(ev);
}

void ToolView::actionEvent(QActionEvent *event)
{
    QFrame::actionEvent(event);
    if (event->type() == QEvent::ActionAdded) {
        m_toolbar->addAction(event->action());
    } else if (event->type() == QEvent::ActionRemoved) {
        m_toolbar->removeAction(event->action());
    }
    m_toolbar->setVisible(!m_toolbar->actions().isEmpty());
}

// END TOOLVIEW

// BEGIN SIDEBAR

Sidebar::Sidebar(KMultiTabBar::KMultiTabBarPosition pos, MainWindow *mainwin, QWidget *parent)
    : KMultiTabBar(pos, parent)
    , m_mainWin(mainwin)
    , m_splitter(nullptr)
    , m_ownSplit(nullptr)
    , m_lastSize(0)
{
    hide();
}

QSize Sidebar::minimumSizeHint() const
{
    auto size = KMultiTabBar::minimumSizeHint();
    const auto fm = QFontMetrics(font());
    const int height = fm.lineSpacing();
    size.setHeight(height + 4);
    return size;
}

QSize Sidebar::sizeHint() const
{
    return minimumSizeHint();
}

void Sidebar::setSplitter(QSplitter *sp)
{
    // if splitter was set before, disconnect handler for collapse
    if (m_splitter) {
        disconnect(m_splitter, &QSplitter::splitterMoved, this, &Sidebar::handleCollapse);
        const int ownSplitIndex = m_splitter->indexOf(m_ownSplit);
        for (int i : {ownSplitIndex, ownSplitIndex + 1}) {
            if (i > 0 && i < m_splitter->count()) {
                m_splitter->handle(i)->removeEventFilter(this);
            }
        }
    }

    m_splitter = sp;
    m_ownSplit = new QSplitter((position() == KMultiTabBar::Top || position() == KMultiTabBar::Bottom) ? Qt::Horizontal : Qt::Vertical, m_splitter);
    m_ownSplit->setChildrenCollapsible(false);
    m_ownSplit->hide();

    // Add resize placeholder (an empty QLabel) so that sidebar will still be resizable after collapse
    // see Sidebar::handleCollapse
    m_resizePlaceholder = new QLabel();
    m_ownSplit->addWidget(m_resizePlaceholder);
    m_resizePlaceholder->hide();
    m_resizePlaceholder->setMinimumSize(QSize(160, 160)); // Same minimum size set in ToolView::minimumSizeHint

    connect(m_splitter, &QSplitter::splitterMoved, this, &Sidebar::handleCollapse);
}

void Sidebar::updateLastSizeOnResize()
{
    const int splitHandleIndex = qMin(m_splitter->indexOf(m_ownSplit) + 1, m_splitter->count() - 1);
    // this method requires that a splitter handle for resizing the sidebar exists
    Q_ASSERT(splitHandleIndex > 0);
    m_splitter->handle(splitHandleIndex)->installEventFilter(this);
}

ToolView *Sidebar::addWidget(const QIcon &icon, const QString &text, ToolView *widget)
{
    static int id = 0;

    if (widget) {
        if (widget->sidebar() == this) {
            return widget;
        }

        widget->sidebar()->removeWidget(widget);
    }

    int newId = ++id;

    appendTab(icon, newId, text);

    if (!widget) {
        widget = new ToolView(m_mainWin, this, m_ownSplit);
        widget->hide();
        widget->icon = icon;
        widget->text = text;
    } else {
        widget->hide();
        widget->setParent(m_ownSplit);
        widget->m_sidebar = this;
    }

    // save its pos ;)
    widget->persistent = false;

    m_idToWidget.emplace(newId, widget);
    m_widgetToId.emplace(widget, newId);
    m_toolviews.push_back(widget);

    // widget => size, for correct size restoration after hide/show
    // starts with invalid size
    m_widgetToSize.emplace(widget, QSize());

    show();

    connect(tab(newId), SIGNAL(clicked(int)), this, SLOT(tabClicked(int)));
    tab(newId)->installEventFilter(this);
    tab(newId)->setToolTip(QString());

    return widget;
}

bool Sidebar::removeWidget(ToolView *widget)
{
    if (m_widgetToId.find(widget) == m_widgetToId.end()) {
        return false;
    }

    removeTab(m_widgetToId[widget]);

    m_idToWidget.erase(m_widgetToId[widget]);
    m_widgetToId.erase(widget);
    m_widgetToSize.erase(widget);
    m_toolviews.erase(std::remove(m_toolviews.begin(), m_toolviews.end(), widget), m_toolviews.end());

    bool anyVis = false;
    for (const auto &[id, wid] : m_idToWidget) {
        if (wid->isVisible()) {
            anyVis = true;
            break;
        }
    }

    if (m_idToWidget.empty()) {
        m_ownSplit->hide();
        hide();
    } else if (!anyVis) {
        m_ownSplit->hide();
    }

    return true;
}

bool Sidebar::showWidget(ToolView *widget)
{
    if (m_widgetToId.find(widget) == m_widgetToId.end()) {
        return false;
    }

    bool unfixSize = false;
    // hide other non-persistent views
    for (const auto &[id, wid] : m_idToWidget) {
        if (wid != widget && !wid->persistent) {
            hideWidget(wid);
        }
        // lock persistents' size while show/hide reshuffle happens
        // (could also make this behavior per-widget configurable)
        if (wid->persistent) {
            const auto size = wid->size();
            wid->setMaximumSize(size);
            wid->setMinimumSize(size);
            unfixSize = true;
        }
    }

    setTab(m_widgetToId[widget], true);

    /**
     * resize to right size again and show, else artifacts
     */
    if (m_widgetToSize[widget].isValid()) {
        widget->resize(m_widgetToSize[widget]);
    }

    /**
     * resize to right size again and show, else artifacts
     * same as for widget, both needed
     */
    if (m_preHideSize.isValid()) {
        widget->resize(m_preHideSize);
        m_ownSplit->resize(m_preHideSize);
    }
    m_ownSplit->show();
    widget->show();

    // release persistent size again
    // (later on, when all event processing has happened)
    auto func = [this]() {
        auto wsizes = m_ownSplit->sizes();
        for (const auto &[id, widget] : m_idToWidget) {
            Q_UNUSED(id)
            widget->setMinimumSize(QSize(0, 0));
            widget->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
        }
        m_ownSplit->setSizes(wsizes);
    };
    if (unfixSize) {
        QTimer::singleShot(0, this, func);
    }

    // ensure that the sidebar is expanded
    expandSidebar(widget);

    /**
     * we are visible again!
     */
    widget->setToolVisible(true);
    return true;
}

bool Sidebar::hideWidget(ToolView *widget)
{
    if (m_widgetToId.find(widget) == m_widgetToId.end()) {
        return false;
    }

    updateLastSize();

    bool anyVis = false;
    for (const auto &[id, wid] : m_idToWidget) {
        if (wid == widget) {
            if (widget->isVisible()) {
                m_widgetToSize[widget] = widget->size();
            }
        } else if (wid->isVisible()) {
            anyVis = true;
            break;
        }
    }

    widget->hide();

    // lower tab
    setTab(m_widgetToId[widget], false);

    if (!anyVis) {
        if (m_ownSplit->isVisible()) {
            m_preHideSize = m_ownSplit->size();
        }
        m_ownSplit->hide();
    } else {
        // some toolviews are still visible, so we must ensure the sidebar is expanded
        expandSidebar(widget);
    }

    widget->setToolVisible(false);
    return true;
}

void Sidebar::showWidgetTab(ToolView *widget, bool show)
{
    auto it = m_widgetToId.find(widget);
    if (it == m_widgetToId.end()) {
        qWarning() << Q_FUNC_INFO << "Unexpected no id for widget " << widget;
        return;
    }
    int id = it->second;
    auto *tab = this->tab(id);
    if ((!tab->isHidden()) != show) {
        tab->setHidden(!show);
        if (!show && widget->toolVisible()) {
            hideWidget(widget);
        }
        Q_EMIT widget->tabButtonVisibleChanged(show);
    }
}

bool Sidebar::tabVisible(ToolView *tv) const
{
    auto it = m_widgetToId.find(tv);
    if (it == m_widgetToId.end()) {
        qWarning() << Q_FUNC_INFO << "Unexpected no id for toolview " << tv;
        return false;
    }
    return !tab(it->second)->isHidden();
}

bool Sidebar::isCollapsed()
{
    if (!m_splitter) {
        // sidebar still has no splitter set
        return false;
    }

    const int ownSplitIndex = m_splitter->indexOf(m_ownSplit);
    if (ownSplitIndex == -1) {
        // m_ownSplit should already be a child of m_splitter if it is set, but we check here just in case
        return false;
    }

    return m_splitter->sizes().at(ownSplitIndex) == 0;
}

void Sidebar::handleCollapse(int pos, int index)
{
    Q_UNUSED(pos);
    if (!m_splitter) {
        return;
    }

    // Verify that the sidebar really belongs to m_splitter
    // and that we are handling the correct/matching sidebar
    // 0 | 1 | 2  <- ownSplitIndex
    //   1   2    <- index (of splitters, represented by |)
    // ownSplitIndex should only be equal to index or (index - 1)
    const int ownSplitIndex = m_splitter->indexOf(m_ownSplit);
    if (ownSplitIndex == -1 || qAbs(index - ownSplitIndex) > 1) {
        return;
    }

    if (isCollapsed()) {
        if (m_isPreviouslyCollapsed) {
            return;
        }

        // If the sidebar is collapsed, we need to hide the activated plugin-views since they are,
        // visually speaking, hidden from the user but technically isVisible() would still return true
        for (const auto &[id, wid] : m_idToWidget) {
            if (wid->isVisible()) {
                wid->hide();
            }
        }

        // show resize placeholder again, otherwise the sidebar splitter won't be manually resizable/expandable
        m_resizePlaceholder->show();
        m_isPreviouslyCollapsed = true;
    } else if (m_isPreviouslyCollapsed && m_resizePlaceholder->isVisible()) {
        // If the sidebar is manually expanded again, we need to show the activated plugin-views again
        for (const auto &[id, wid] : m_idToWidget) {
            if (isTabRaised(id)) {
                wid->show();
            }
        }

        m_resizePlaceholder->hide();
        m_isPreviouslyCollapsed = false;
    }
}

void Sidebar::expandSidebar(ToolView *widget)
{
    if (m_widgetToId.find(widget) == m_widgetToId.end()) {
        return;
    }

    // If the sidebar is collapsed, we need to resize it so that it does not become "stuck" in the collapsed state
    // see BUG: 439535
    // NOTE: Even if the sidebar is expanded, this does not ensure that it is visible (might be hidden with hide() or setVisible(false))
    if (m_isPreviouslyCollapsed && isCollapsed()) {
        QList<int> wsizes = m_splitter->sizes();
        const int ownSplitIndex = m_splitter->indexOf(m_ownSplit);
        if (m_lastSize > 2) { // use last size if available (see updateLastSize())
            wsizes[ownSplitIndex] = m_lastSize;
        } else if (m_splitter->orientation() == Qt::Vertical) {
            wsizes[ownSplitIndex] = qMax(widget->minimumSizeHint().height(), m_widgetToSize[widget].height());
        } else {
            wsizes[ownSplitIndex] = qMax(widget->minimumSizeHint().width(), m_widgetToSize[widget].width());
        }

        // when the sidebar was collapsed, the activated widgets were hidden, so we need to show them again
        // see Sidebar::handleCollapse
        for (const auto &[id, wid] : m_idToWidget) {
            if (isTabRaised(id)) {
                wid->show();
            }
        }

        m_resizePlaceholder->hide();
        m_isPreviouslyCollapsed = false;
        m_splitter->setSizes(wsizes);
    }
}

void Sidebar::tabClicked(int i)
{
    ToolView *w = m_idToWidget[i];

    if (!w) {
        return;
    }

    if (isTabRaised(i) || isCollapsed()) {
        showWidget(w);
        w->setFocus();
        // When sidebar is collapsed and the toolview was pressed, we only expand
        // the toolview/sidebar, so the tab must remain activated
        setTab(m_widgetToId[w], true);
    } else {
        hideWidget(w);
    }
}

bool Sidebar::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::ContextMenu) {
        QContextMenuEvent *e = static_cast<QContextMenuEvent *>(ev);
        KMultiTabBarTab *bt = dynamic_cast<KMultiTabBarTab *>(obj);
        if (bt) {
            // qCDebug(LOG_KATE) << "Request for popup";

            m_popupButton = bt->id();

            ToolView *w = m_idToWidget[m_popupButton];

            if (w) {
                QMenu menu(this);

                if (!w->plugin.isNull()) {
                    if (w->plugin.data()->configPages() > 0) {
                        menu.addAction(i18n("Configure ..."))->setData(20);
                    }
                }

                menu.addSection(QIcon::fromTheme(QStringLiteral("view_remove")), i18n("Behavior"));

                menu.addAction(w->persistent ? QIcon::fromTheme(QStringLiteral("view-restore")) : QIcon::fromTheme(QStringLiteral("view-fullscreen")),
                               w->persistent ? i18n("Make Non-Persistent") : i18n("Make Persistent"))
                    ->setData(10);

                menu.addAction(i18n("Hide Button"))->setData(11);

                menu.addSection(QIcon::fromTheme(QStringLiteral("move")), i18n("Move To"));

                if (position() != 0) {
                    menu.addAction(QIcon::fromTheme(QStringLiteral("go-previous")), i18n("Left Sidebar"))->setData(0);
                }

                if (position() != 1) {
                    menu.addAction(QIcon::fromTheme(QStringLiteral("go-next")), i18n("Right Sidebar"))->setData(1);
                }

                if (position() != 2) {
                    menu.addAction(QIcon::fromTheme(QStringLiteral("go-up")), i18n("Top Sidebar"))->setData(2);
                }

                if (position() != 3) {
                    menu.addAction(QIcon::fromTheme(QStringLiteral("go-down")), i18n("Bottom Sidebar"))->setData(3);
                }

                connect(&menu, &QMenu::triggered, this, &Sidebar::buttonPopupActivate);

                menu.exec(e->globalPos());

                return true;
            }
        }
    } else if (ev->type() == QEvent::MouseButtonRelease) {
        // The sidebar's splitter handle handle is release, so we update the sidebar's size. See Sidebar::updateLastSizeOnResize
        QMouseEvent *e = static_cast<QMouseEvent *>(ev);
        if (e->button() == Qt::LeftButton) {
            updateLastSize();
        }
    }

    return false;
}

void Sidebar::setVisible(bool visible)
{
    // visible==true means show-request
    if (visible && (m_idToWidget.empty() || !m_mainWin->sidebarsVisible())) {
        return;
    }

    KMultiTabBar::setVisible(visible);
}

void Sidebar::buttonPopupActivate(QAction *a)
{
    int id = a->data().toInt();
    ToolView *w = m_idToWidget[m_popupButton];

    if (!w) {
        return;
    }

    // move ids
    if (id < 4) {
        // move + show ;)
        m_mainWin->moveToolView(w, static_cast<KMultiTabBar::KMultiTabBarPosition>(id));
        m_mainWin->showToolView(w);
    }

    // toggle persistent
    if (id == 10) {
        w->persistent = !w->persistent;
    }

    // configure actionCollection
    if (id == 20) {
        if (!w->plugin.isNull()) {
            if (w->plugin.data()->configPages() > 0) {
                Q_EMIT sigShowPluginConfigPage(w->plugin.data(), 0);
            }
        }
    }

    if (id == 11) {
        showWidgetTab(w, false);
    }
}

void Sidebar::updateLastSize()
{
    QList<int> s = m_splitter->sizes();

    int i = 0;
    if ((position() == KMultiTabBar::Right || position() == KMultiTabBar::Bottom)) {
        i = 2;
    }

    // little threshold
    if (s[i] > 2) {
        m_lastSize = s[i];
    }
}

class TmpToolViewSorter
{
public:
    ToolView *tv;
    unsigned int pos;
};

void Sidebar::restoreSession(KConfigGroup &config)
{
    // get the last correct placed toolview
    int firstWrong = 0;
    const int toolViewsCount = (int)m_toolviews.size();
    for (; firstWrong < toolViewsCount; ++firstWrong) {
        ToolView *tv = m_toolviews[firstWrong];

        int pos = config.readEntry(QStringLiteral("Kate-MDI-ToolView-%1-Sidebar-Position").arg(tv->id), firstWrong);

        if (pos != firstWrong) {
            break;
        }
    }

    // we need to reshuffle, ahhh :(
    if (firstWrong < toolViewsCount) {
        // first: collect the items to reshuffle
        std::vector<TmpToolViewSorter> toSort;
        for (int i = firstWrong; i < toolViewsCount; ++i) {
            TmpToolViewSorter s;
            s.tv = m_toolviews[i];
            s.pos = config.readEntry(QStringLiteral("Kate-MDI-ToolView-%1-Sidebar-Position").arg(m_toolviews[i]->id), i);
            toSort.push_back(s);
        }

        // now: sort the stuff we need to reshuffle
        std::sort(toSort.begin(), toSort.end(), [](const TmpToolViewSorter &l, const TmpToolViewSorter &r) {
            return l.pos < r.pos;
        });

        // then: remove this items from the button bar
        // do this backwards, to minimize the relayout efforts
        for (int i = toolViewsCount - 1; i >= firstWrong; --i) {
            removeTab(m_widgetToId[m_toolviews[i]]);
        }

        // insert the reshuffled things in order :)
        for (int i = 0; i < (int)toSort.size(); ++i) {
            ToolView *tv = toSort[i].tv;

            m_toolviews[firstWrong + i] = tv;

            // readd the button
            int newId = m_widgetToId[tv];
            appendTab(tv->icon, newId, tv->text);
            connect(tab(newId), &KMultiTabBarTab::clicked, this, &Sidebar::tabClicked);
            tab(newId)->installEventFilter(this);
            tab(newId)->setToolTip(QString());

            // reshuffle in splitter: move to last
            m_ownSplit->addWidget(tv);
        }
    }

    // update last size if needed
    updateLastSize();

    // restore the own splitter sizes
    QList<int> s = config.readEntry(QStringLiteral("Kate-MDI-Sidebar-%1-Splitter").arg(position()), QList<int>());
    m_ownSplit->setSizes(s);

    // show only correct toolviews, remember persistent values ;)
    bool anyVis = false;
    for (auto tv : m_toolviews) {
        tv->persistent = config.readEntry(QStringLiteral("Kate-MDI-ToolView-%1-Persistent").arg(tv->id), false);
        tv->setToolVisible(config.readEntry(QStringLiteral("Kate-MDI-ToolView-%1-Visible").arg(tv->id), false));

        if (!anyVis) {
            anyVis = tv->toolVisible();
        }

        auto id = m_widgetToId[tv];
        setTab(id, tv->toolVisible());

        if (tv->toolVisible()) {
            tv->show();
        } else {
            tv->hide();
        }
        bool showTab = config.readEntry(QStringLiteral("Kate-MDI-ToolView-%1-Show-Button-In-Sidebar").arg(tv->id), true);
        showWidgetTab(tv, showTab);
    }

    if (anyVis) {
        m_ownSplit->show();
        // if there are activated plugin-views but sidebar is collapsed, we must set m_isPreviouslyCollapsed to true so that it can be expanded
        if (isCollapsed()) {
            m_isPreviouslyCollapsed = true;
        }
    } else {
        m_ownSplit->hide();
    }
}

void Sidebar::saveSession(KConfigGroup &config)
{
    // store the own splitter sizes
    QList<int> s = m_ownSplit->sizes();
    config.writeEntry(QStringLiteral("Kate-MDI-Sidebar-%1-Splitter").arg(position()), s);

    // store the data about all toolviews in this sidebar ;)
    for (int i = 0; i < (int)m_toolviews.size(); ++i) {
        ToolView *tv = m_toolviews[i];

        config.writeEntry(QStringLiteral("Kate-MDI-ToolView-%1-Position").arg(tv->id), int(tv->sidebar()->position()));
        config.writeEntry(QStringLiteral("Kate-MDI-ToolView-%1-Sidebar-Position").arg(tv->id), i);
        config.writeEntry(QStringLiteral("Kate-MDI-ToolView-%1-Visible").arg(tv->id), tv->toolVisible());
        config.writeEntry(QStringLiteral("Kate-MDI-ToolView-%1-Persistent").arg(tv->id), tv->persistent);
        auto it = m_widgetToId.find(tv);
        if (it == m_widgetToId.end()) {
            qWarning() << Q_FUNC_INFO << "Unexpected no tab id for toolview: " << tv;
            continue;
        }
        config.writeEntry(QStringLiteral("Kate-MDI-ToolView-%1-Show-Button-In-Sidebar").arg(tv->id), tab(it->second)->isVisible());
    }
}

// END SIDEBAR

// BEGIN MAIN WINDOW

MainWindow::MainWindow(QWidget *parentWidget)
    : KParts::MainWindow(parentWidget, Qt::Window)
    , m_guiClient(new GUIClient(this))
{
    // central frame for all stuff
    QFrame *hb = new QFrame(this);
    setCentralWidget(hb);

    // top level vbox for all stuff + bottom bar
    QVBoxLayout *toplevelVBox = new QVBoxLayout(hb);
    toplevelVBox->setContentsMargins(0, 0, 0, 0);
    toplevelVBox->setSpacing(0);

    // hbox for all splitters and other side bars
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);
    toplevelVBox->addLayout(hlayout);

    m_sidebars[KMultiTabBar::Left] = std::make_unique<Sidebar>(KMultiTabBar::Left, this, hb);
    hlayout->addWidget(m_sidebars[KMultiTabBar::Left].get());

    m_hSplitter = new QSplitter(Qt::Horizontal, hb);
    hlayout->addWidget(m_hSplitter);

    m_sidebars[KMultiTabBar::Left]->setSplitter(m_hSplitter);

    QFrame *vb = new QFrame(m_hSplitter);
    QVBoxLayout *vlayout = new QVBoxLayout(vb);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);

    m_hSplitter->setCollapsible(m_hSplitter->indexOf(vb), false);
    m_hSplitter->setStretchFactor(m_hSplitter->indexOf(vb), 1);

    m_sidebars[KMultiTabBar::Top] = std::make_unique<Sidebar>(KMultiTabBar::Top, this, vb);
    vlayout->addWidget(m_sidebars[KMultiTabBar::Top].get());

    m_vSplitter = new QSplitter(Qt::Vertical, vb);
    vlayout->addWidget(m_vSplitter);

    m_sidebars[KMultiTabBar::Top]->setSplitter(m_vSplitter);

    m_centralWidget = new QWidget(m_vSplitter);
    m_centralWidget->setLayout(new QVBoxLayout);
    m_centralWidget->layout()->setSpacing(0);
    m_centralWidget->layout()->setContentsMargins(0, 0, 0, 0);

    m_vSplitter->setCollapsible(m_vSplitter->indexOf(m_centralWidget), false);
    m_vSplitter->setStretchFactor(m_vSplitter->indexOf(m_centralWidget), 1);

    m_sidebars[KMultiTabBar::Right] = std::make_unique<Sidebar>(KMultiTabBar::Right, this, hb);
    hlayout->addWidget(m_sidebars[KMultiTabBar::Right].get());
    m_sidebars[KMultiTabBar::Right]->setSplitter(m_hSplitter);

    // bottom side bar spans full windows, include status bar, too
    m_sidebars[KMultiTabBar::Bottom] = std::make_unique<Sidebar>(KMultiTabBar::Bottom, this, vb);
    auto bottomHBoxLaout = new QHBoxLayout;
    bottomHBoxLaout->addWidget(m_sidebars[KMultiTabBar::Bottom].get());
    m_statusBarStackedWidget = new QStackedWidget(this);
    m_statusBarStackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    bottomHBoxLaout->addWidget(m_statusBarStackedWidget);
    bottomHBoxLaout->setStretch(0, 100);
    toplevelVBox->addLayout(bottomHBoxLaout);
    m_sidebars[KMultiTabBar::Bottom]->setSplitter(m_vSplitter);

    for (const auto &sidebar : qAsConst(m_sidebars)) {
        connect(sidebar.get(), &Sidebar::sigShowPluginConfigPage, this, &MainWindow::sigShowPluginConfigPage);
        // all sidebar splitter handles are instantiated, so we can now safely call this
        sidebar->updateLastSizeOnResize();
    }
}

MainWindow::~MainWindow()
{
    // cu toolviews
    qDeleteAll(m_toolviews);

    // seems like we really should delete this by hand ;)
    delete m_centralWidget;
}

QWidget *MainWindow::centralWidget() const
{
    return m_centralWidget;
}

ToolView *MainWindow::createToolView(KTextEditor::Plugin *plugin,
                                     const QString &identifier,
                                     KMultiTabBar::KMultiTabBarPosition pos,
                                     const QIcon &icon,
                                     const QString &text)
{
    if (m_idToWidget[identifier]) {
        return nullptr;
    }

    // try the restore config to figure out real pos
    if (m_restoreConfig && m_restoreConfig->hasGroup(m_restoreGroup)) {
        KConfigGroup cg(m_restoreConfig, m_restoreGroup);
        pos = static_cast<KMultiTabBar::KMultiTabBarPosition>(cg.readEntry(QStringLiteral("Kate-MDI-ToolView-%1-Position").arg(identifier), int(pos)));
    }

    ToolView *v = m_sidebars[pos]->addWidget(icon, text, nullptr);
    v->id = identifier;
    v->plugin = plugin;

    m_idToWidget.emplace(identifier, v);
    m_toolviews.push_back(v);

    // register for menu stuff
    m_guiClient->registerToolView(v);

    return v;
}

ToolView *MainWindow::toolView(const QString &identifier) const
{
    auto it = m_idToWidget.find(identifier);
    if (it != m_idToWidget.end()) {
        return it->second;
    }
    return nullptr;
}

void MainWindow::toolViewDeleted(ToolView *widget)
{
    if (!widget) {
        return;
    }

    if (widget->mainWindow() != this) {
        return;
    }

    // unregister from menu stuff
    m_guiClient->unregisterToolView(widget);

    widget->sidebar()->removeWidget(widget);

    m_idToWidget.erase(widget->id);

    m_toolviews.erase(std::remove(m_toolviews.begin(), m_toolviews.end(), widget), m_toolviews.end());
}

void MainWindow::setSidebarsVisibleInternal(bool visible, bool noWarning)
{
    bool old_visible = m_sidebarsVisible;
    m_sidebarsVisible = visible;

    m_sidebars[0]->setVisible(visible);
    m_sidebars[1]->setVisible(visible);
    m_sidebars[2]->setVisible(visible);
    m_sidebars[3]->setVisible(visible);

    m_guiClient->updateSidebarsVisibleAction();

    // show information message box, if the users hides the sidebars
    if (!noWarning && old_visible && (!m_sidebarsVisible)) {
        KMessageBox::information(this,
                                 i18n("<qt>You are about to hide the sidebars. With "
                                      "hidden sidebars it is not possible to directly "
                                      "access the tool views with the mouse anymore, "
                                      "so if you need to access the sidebars again "
                                      "invoke <b>View &gt; Tool Views &gt; Show Sidebars</b> "
                                      "in the menu. It is still possible to show/hide "
                                      "the tool views with the assigned shortcuts.</qt>"),
                                 QString(),
                                 QStringLiteral("Kate hide sidebars notification message"));
    }
}

bool MainWindow::sidebarsVisible() const
{
    return m_sidebarsVisible;
}

void MainWindow::setToolViewStyle(KMultiTabBar::KMultiTabBarStyle style)
{
    m_sidebars[0]->setStyle(style);
    m_sidebars[1]->setStyle(style);
    m_sidebars[2]->setStyle(style);
    m_sidebars[3]->setStyle(style);
}

KMultiTabBar::KMultiTabBarStyle MainWindow::toolViewStyle() const
{
    // all sidebars have the same style, so just take Top
    return m_sidebars[KMultiTabBar::Top]->tabStyle();
}

bool MainWindow::moveToolView(ToolView *widget, KMultiTabBar::KMultiTabBarPosition pos)
{
    if (!widget || widget->mainWindow() != this) {
        return false;
    }

    // try the restore config to figure out real pos
    if (m_restoreConfig && m_restoreConfig->hasGroup(m_restoreGroup)) {
        KConfigGroup cg(m_restoreConfig, m_restoreGroup);
        pos = static_cast<KMultiTabBar::KMultiTabBarPosition>(cg.readEntry(QStringLiteral("Kate-MDI-ToolView-%1-Position").arg(widget->id), int(pos)));
    }

    m_sidebars[pos]->addWidget(widget->icon, widget->text, widget);

    return true;
}

bool MainWindow::showToolView(ToolView *widget)
{
    if (!widget || widget->mainWindow() != this) {
        return false;
    }

    // skip this if happens during restoring, or we will just see flicker
    if (m_restoreConfig && m_restoreConfig->hasGroup(m_restoreGroup)) {
        return true;
    }

    return widget->sidebar()->showWidget(widget);
}

bool MainWindow::hideToolView(ToolView *widget)
{
    if (!widget || widget->mainWindow() != this) {
        return false;
    }

    // skip this if happens during restoring, or we will just see flicker
    if (m_restoreConfig && m_restoreConfig->hasGroup(m_restoreGroup)) {
        return true;
    }

    const bool ret = widget->sidebar()->hideWidget(widget);
    m_centralWidget->setFocus();
    return ret;
}

void MainWindow::hideToolViews()
{
    for (const auto &tv : m_toolviews) {
        if (tv) {
            tv->sidebar()->hideWidget(tv);
        }
    }
    m_centralWidget->setFocus();
}

void MainWindow::startRestore(KConfigBase *config, const QString &group)
{
    // first save this stuff
    m_restoreConfig = config;
    m_restoreGroup = group;

    if (!m_restoreConfig || !m_restoreConfig->hasGroup(m_restoreGroup)) {
        // if no config around, set already now sane default sizes
        // otherwise, set later in ::finishRestore(), since it does not work
        // if set already now (see bug #164438)
        QList<int> hs = (QList<int>() << 200 << 100 << 200);
        QList<int> vs = (QList<int>() << 150 << 100 << 200);

        m_sidebars[0]->setLastSize(hs[0]);
        m_sidebars[1]->setLastSize(hs[2]);
        m_sidebars[2]->setLastSize(vs[0]);
        m_sidebars[3]->setLastSize(vs[2]);

        m_hSplitter->setSizes(hs);
        m_vSplitter->setSizes(vs);
        return;
    }

    // apply size once, to get sizes ready ;)
    KConfigGroup cg(m_restoreConfig, m_restoreGroup);
    KWindowConfig::restoreWindowSize(windowHandle(), cg);

    setToolViewStyle(static_cast<KMultiTabBar::KMultiTabBarStyle>(cg.readEntry("Kate-MDI-Sidebar-Style", static_cast<int>(toolViewStyle()))));
    // after reading m_sidebarsVisible, update the GUI toggle action
    m_sidebarsVisible = cg.readEntry("Kate-MDI-Sidebar-Visible", true);
    m_guiClient->updateSidebarsVisibleAction();
}

void MainWindow::finishRestore()
{
    if (!m_restoreConfig) {
        return;
    }

    if (m_restoreConfig->hasGroup(m_restoreGroup)) {
        // apply all settings, like toolbar pos and more ;)
        KConfigGroup cg(m_restoreConfig, m_restoreGroup);
        applyMainWindowSettings(cg);

        // reshuffle toolviews only if needed
        for (const auto tv : m_toolviews) {
            KMultiTabBar::KMultiTabBarPosition newPos = static_cast<KMultiTabBar::KMultiTabBarPosition>(
                cg.readEntry(QStringLiteral("Kate-MDI-ToolView-%1-Position").arg(tv->id), int(tv->sidebar()->position())));

            if (tv->sidebar()->position() != newPos) {
                moveToolView(tv, newPos);
            }
        }

        // restore the sidebars
        for (auto &sidebar : qAsConst(m_sidebars)) {
            sidebar->restoreSession(cg);
        }

        // restore splitter sizes
        QList<int> hs = (QList<int>() << 200 << 100 << 200);
        QList<int> vs = (QList<int>() << 150 << 100 << 200);

        // get main splitter sizes ;)
        hs = cg.readEntry("Kate-MDI-H-Splitter", hs);
        vs = cg.readEntry("Kate-MDI-V-Splitter", vs);

        m_sidebars[0]->setLastSize(hs[0]);
        m_sidebars[1]->setLastSize(hs[2]);
        m_sidebars[2]->setLastSize(vs[0]);
        m_sidebars[3]->setLastSize(vs[2]);

        m_hSplitter->setSizes(hs);
        m_vSplitter->setSizes(vs);
    }

    // clear this stuff, we are done ;)
    m_restoreConfig = nullptr;
    m_restoreGroup.clear();
}

void MainWindow::saveSession(KConfigGroup &config)
{
    saveMainWindowSettings(config);

    // save main splitter sizes ;)
    QList<int> hs = m_hSplitter->sizes();
    QList<int> vs = m_vSplitter->sizes();

    if (hs[0] <= 2 && !m_sidebars[0]->splitterVisible()) {
        hs[0] = m_sidebars[0]->lastSize();
    }
    if (hs[2] <= 2 && !m_sidebars[1]->splitterVisible()) {
        hs[2] = m_sidebars[1]->lastSize();
    }
    if (vs[0] <= 2 && !m_sidebars[2]->splitterVisible()) {
        vs[0] = m_sidebars[2]->lastSize();
    }
    if (vs[2] <= 2 && !m_sidebars[3]->splitterVisible()) {
        vs[2] = m_sidebars[3]->lastSize();
    }

    config.writeEntry("Kate-MDI-H-Splitter", hs);
    config.writeEntry("Kate-MDI-V-Splitter", vs);

    // save sidebar style
    config.writeEntry("Kate-MDI-Sidebar-Style", static_cast<int>(toolViewStyle()));
    config.writeEntry("Kate-MDI-Sidebar-Visible", m_sidebarsVisible);

    // save the sidebars
    for (auto &sidebar : qAsConst(m_sidebars)) {
        sidebar->saveSession(config);
    }
}

// END MAIN WINDOW

} // namespace KateMDI
