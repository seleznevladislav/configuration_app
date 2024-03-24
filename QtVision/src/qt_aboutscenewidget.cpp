#include <qt_aboutscenewidget.h>
#include <qt_openglwidget.h>
#include <last.h>

VSN_BEGIN_NAMESPACE

namespace QtVision {

static int vsn_margin = 5;

#define VSN_NOACTIVATED_ITEM        QObject::tr("No activated")
#define VSN_RENDER_ITEM             QObject::tr("Render:")
#define VSN_RENDER_FPS_ITEM         QObject::tr("Render Fps")
#define VSN_RENDER_BODIES_ITEM      QObject::tr("Render Bodies:")
#define VSN_RENDER_TRIANGLES_ITEM   QObject::tr("Render Triangles:")
#define VSN_LOADED_BODIES_ITEM      QObject::tr("Loaded Bodies:")

// ----------------------------------------------------------------------------
// Default constructor.
// ---
QtAboutSceneWidgetItem::QtAboutSceneWidgetItem()
{
}

// ----------------------------------------------------------------------------
// Destructor.
// ---
QtAboutSceneWidgetItem::~QtAboutSceneWidgetItem()
{
}


// ----------------------------------------------------------------------------
// Default constructor.
// ---
QtAboutSceneWidget::QtAboutSceneWidget(QWidget* pParent)
    : QObject(pParent)
    , m_dirty(false)
    , m_maxHeightItem(0)
    , m_maxFirstColumnWidth(0)
    , m_maxSecondColumnWidth(0)
    , m_frames(0)
    , m_countTest(10)
    , m_fAngle(0.0)
{
    Q_ASSERT(pParent != Q_NULLPTR);
    m_font = pParent->font();
    m_font.setBold(true);
    m_font.setPointSize(m_font.pointSize() * 1.15);
    initTableContents();
}

// ----------------------------------------------------------------------------
// Destructor.
// ---
QtAboutSceneWidget::~QtAboutSceneWidget()
{
    clearRowItems();
}

// ----------------------------------------------------------------------------
//
// ---
void QtAboutSceneWidget::setRowCount(int rows)
{
    clearRowItems();
    for (int i = 0; rows > i; ++i)
        createRowItems();
    m_dirty = true;
}

// ----------------------------------------------------------------------------
//
// ---
QtAboutSceneWidgetItem* QtAboutSceneWidget::item(int row, int col) const
{
    Q_ASSERT(m_firstColumnChildren.size() == m_secondColumnChildren.size());
    if (col == 0)
        return m_firstColumnChildren.at(row);
    else if (col == 1)
        return m_secondColumnChildren.at(row);
    else if (col == 2)
        return m_secondColumnChildren.at(row);
    return Q_NULLPTR;
}


// ----------------------------------------------------------------------------
//
// ---
QSize QtAboutSceneWidget::sizeHint() const
{ return m_sizeHint; }

// ----------------------------------------------------------------------------
//
// ---
void QtAboutSceneWidget::setPosition(const QPoint& pnt)
{
    m_rect.setTopLeft(pnt);
    m_rect.setSize(m_sizeHint);
}

// ----------------------------------------------------------------------------
//
// ---
void QtAboutSceneWidget::paintStatistics(QPainter* p)
{
    if (m_rect.isNull())
        return;
    QRect rect = m_rect;
    rect.adjust(0, 0, -1, -1);

    QBrush br(QColor(255, 255, 255, 125));
    p->setBrush(br);
    p->drawRoundedRect(m_rect, 5, 5);

    const int margin = vsn_margin;
    rect.adjust(margin, 0, 0, 0);

    int y = rect.top();
    for (int i = 0; i < m_firstColumnChildren.size(); ++i)
    {
        QFont saveFont = p->font();
        QtAboutSceneWidgetItem* item = m_firstColumnChildren.at(i);
        p->setFont(item->font());

        QSize sz = item->sizeHint();
        y += m_maxHeightItem;
        p->drawText(rect.left(), y + margin, item->text());
        p->setFont(saveFont);
    }

    y = rect.top();
    for (int i = 0; i < m_secondColumnChildren.size(); ++i)
    {
        QFont saveFont = p->font();
        QtAboutSceneWidgetItem* item = m_secondColumnChildren.at(i);
        p->setFont(item->font());

        y += m_maxHeightItem;
        p->drawText(rect.left() + m_maxFirstColumnWidth + margin * 2, y + margin, item->text());
        p->setFont(saveFont);
    }
}

// ----------------------------------------------------------------------------
//
// ---
void QtAboutSceneWidget::updateStatistics()
{
    if (RenderStatistics::IsActivated())
    {
        qreal fps = 0.0;
        if (const int elapsed = m_time.elapsed())
            fps = m_frames * 1000.0 / elapsed;

        QtAboutSceneWidgetItem* item = QtAboutSceneWidget::item(iRenderFps, 1);
//        item->setText(QString::number(qRound(fps)));
        item->setText(QString::number(fps, 'f', 2));

        item = QtAboutSceneWidget::item(iRenderBodies, 1);
        item->setText(QString::number(RenderStatistics::GetRenderBodiesCounter()));

        item = QtAboutSceneWidget::item(iRenderTriangles, 1);
        item->setText(QString::number(RenderStatistics::GetRenderTriangleCount()));

        item = QtAboutSceneWidget::item(iLoadedBodies, 1);
        item->setText(QString::number(RenderStatistics::GetLoadedBodiesCounter()));
        if (!(m_frames % 100))
        {
            m_time.start();
            m_frames = 0;
            m_fAngle = 0;
            m_countTest++;
        }

        m_fAngle += 1.0f;
        m_frames++;
    }
}

// ----------------------------------------------------------------------------
//
// ---
void QtAboutSceneWidget::createRowItems()
{
    QtAboutSceneWidgetItem* item = new QtAboutSceneWidgetItem();
    item->setFont(m_font);
    m_firstColumnChildren.append(item);
    item = new QtAboutSceneWidgetItem();
    item->setFont(m_font);
    m_secondColumnChildren.append(item);
}

// ----------------------------------------------------------------------------
//
// ---
void QtAboutSceneWidget::clearRowItems()
{
    for (int i = 0; i < m_firstColumnChildren.size(); ++i)
    {
        QtAboutSceneWidgetItem* item = m_firstColumnChildren.at(i);
        delete item;
    }
    m_firstColumnChildren.clear();

    for (int i = 0; i < m_secondColumnChildren.size(); ++i)
    {
        QtAboutSceneWidgetItem* item = m_secondColumnChildren.at(i);
        delete item;
    }
    m_secondColumnChildren.clear();
}

// ----------------------------------------------------------------------------
//
// ---
void QtAboutSceneWidget::initTableContents()
{
    setRowCount(iCount);
    item(iRenderFps, 0)->setText(VSN_RENDER_FPS_ITEM);
    item(iRenderFps, 1)->setText(VSN_NOACTIVATED_ITEM);

    item(iRenderBodies, 0)->setText(VSN_RENDER_BODIES_ITEM);
    item(iRenderBodies, 1)->setText(VSN_NOACTIVATED_ITEM);

    item(iRenderTriangles, 0)->setText(VSN_RENDER_TRIANGLES_ITEM);
    item(iRenderTriangles, 1)->setText(VSN_NOACTIVATED_ITEM);

    item(iLoadedBodies, 0)->setText(VSN_LOADED_BODIES_ITEM);
    item(iLoadedBodies, 1)->setText(VSN_NOACTIVATED_ITEM);
    m_dirty = true;
    calcMaxHeightWidth();
}

// ----------------------------------------------------------------------------
//
// ---
void QtAboutSceneWidget::calcMaxHeightWidth()
{
    if (!m_dirty)
        return;

    int margin = vsn_margin;

    QSize sizeHint;
    sizeHint.setHeight(margin);
    for (int i = 0; i < m_firstColumnChildren.size(); ++i)
    {
        QtAboutSceneWidgetItem* item = m_firstColumnChildren.at(i);
        QSize sz = item->sizeHint();
        m_maxHeightItem = qMax(m_maxHeightItem, sz.height());
        m_maxFirstColumnWidth = qMax(m_maxFirstColumnWidth, sz.width());
        sizeHint.setHeight(sizeHint.height() + m_maxHeightItem + margin);
    }

    for (int i = 0; i < m_secondColumnChildren.size(); ++i)
    {
        QtAboutSceneWidgetItem* item = m_secondColumnChildren.at(i);
        QSize sz = item->sizeHint();
        m_maxSecondColumnWidth = qMax(m_maxSecondColumnWidth, sz.width());
    }

    QFontMetrics fm(m_font);
    m_maxSecondColumnWidth = qMax(m_maxSecondColumnWidth, fm.boundingRect("00000000000").width() + margin);
    sizeHint.setWidth(m_maxFirstColumnWidth + margin * 2 + m_maxSecondColumnWidth);

    m_sizeHint = sizeHint;
    m_dirty = false;
}

// ----------------------------------------------------------------------------
//
// ---
bool QtAboutSceneWidget::event(QEvent* event)
{
    if (event->type() == QEvent::FontChange)
        calcMaxHeightWidth();
    return QObject::event(event);
}


} // namespace QtVision

VSN_END_NAMESPACE