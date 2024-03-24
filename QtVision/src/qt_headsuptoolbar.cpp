#include <qt_headsuptoolbar.h>

#include <QPainter>
#include <QEvent>
#include <QStyle>

#include <last.h>

VSN_BEGIN_NAMESPACE

namespace QtVision {

/* QtAutoHideWindow */
QtAutoHideWindow::QtAutoHideWindow(QWidget* pParent)
    : QWidget(pParent)
    , m_pToolBar(nullptr)
{
    pParent->installEventFilter(this);
}

//-----------------------------------------------------------------------------
// 
// ---
QtAutoHideWindow::~QtAutoHideWindow()
{
    parent()->removeEventFilter(this);
}

//-----------------------------------------------------------------------------
// 
// ---
void QtAutoHideWindow::setToolBar(QToolBar* pToolBar) 
{
    if (pToolBar == nullptr)
        return;
    if (m_pToolBar != pToolBar)
        delete m_pToolBar;

    m_pToolBar = pToolBar;
    m_pToolBar->setParent(this);
    m_pToolBar->setAllowedAreas(Qt::TopToolBarArea);

    QObject::connect(m_pToolBar, &QToolBar::allowedAreasChanged, this, &QtAutoHideWindow::allowedAreasChanged);
}

//-----------------------------------------------------------------------------
// 
// ---
bool QtAutoHideWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == parent())
    {
        if (event->type() == QEvent::Resize || event->type() == QEvent::Show)
            reposition();
    }
    return QWidget::eventFilter(watched, event);
}

//-----------------------------------------------------------------------------
// 
// ---
void QtAutoHideWindow::reposition()
{
    if (m_pToolBar == nullptr)
        return;
    if (QWidget* pParent = qobject_cast<QWidget*>(parent()))
    {
        QRect rc = m_pToolBar->rect();
        QRect rcParent = pParent->rect();
        if (m_pToolBar->allowedAreas() == Qt::TopToolBarArea)
        {
            m_pToolBar->setOrientation(Qt::Horizontal);
            int val = style()->pixelMetric(QStyle::PM_LayoutTopMargin);
            move((rcParent.width() / 2) - (rc.width() / 2) - rc.x(), rc.y() + val);
        }
        if (m_pToolBar->allowedAreas() == Qt::BottomToolBarArea)
        {
            m_pToolBar->setOrientation(Qt::Horizontal);
            int val = style()->pixelMetric(QStyle::PM_LayoutBottomMargin);
            move((rcParent.width() / 2) - (rc.width() / 2) - rc.x(), (rcParent.height() - rc.height()) - val);
        }
        else if (m_pToolBar->allowedAreas() == Qt::RightToolBarArea)
        {
            m_pToolBar->setOrientation(Qt::Vertical);
            int val = style()->pixelMetric(QStyle::PM_LayoutRightMargin);
            move((rcParent.width() - rc.width()) - val, (rcParent.height() / 2) - (rc.height() / 2) - rc.y());
        }
        else if (m_pToolBar->allowedAreas() == Qt::LeftToolBarArea)
        {
            m_pToolBar->setOrientation(Qt::Vertical);
            int val = style()->pixelMetric(QStyle::PM_LayoutLeftMargin);
            move(rc.left() + val, (rcParent.height() / 2) - (rc.height() / 2) - rc.y());
        }
        resize(rc.size());
    }
}

//-----------------------------------------------------------------------------
// 
// ---
void QtAutoHideWindow::paintEvent(QPaintEvent* event)
{
}

//-----------------------------------------------------------------------------
// 
// ---
void QtAutoHideWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    reposition();
}

//-----------------------------------------------------------------------------
// 
// ---
void QtAutoHideWindow::allowedAreasChanged(Qt::ToolBarAreas allowedAreas)
{
    Q_UNUSED(allowedAreas);
    reposition();
}


} // namespace QtVision

VSN_END_NAMESPACE
