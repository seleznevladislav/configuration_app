#include "explodetree.h"
#include <qevent.h>
#include "explodewidget.h"
#include <last.h>

//-----------------------------------------------------------------------------
//
// ---
TreeObjectItem::TreeObjectItem(SceneSegment* pObject)
    : m_pObject(pObject)
{
    QString strName(pObject->GetObjectName().c_str());
//    strName += " ID -";
//    strName += QString::number(pObject->GetUniqueKey().GetKey());
    setText(0, strName);
    setCheckState(0, Qt::Checked);
}

//-----------------------------------------------------------------------------
//
// ---
TreeObjectItem::TreeObjectItem(SceneSegment* pObject, QTreeWidgetItem* parent)
    : QTreeWidgetItem(parent, QStringList())
    , m_pObject(pObject)
{
    QString strName(pObject->GetObjectName().c_str());
//    strName += " ID -";
//    strName += QString::number(pObject->GetUniqueKey().GetKey());
    setText(0, strName);
    setCheckState(0, Qt::Checked);
}

//-----------------------------------------------------------------------------
//
// ---
TreeObjectItem::TreeObjectItem(const TreeObjectItem& other)
    : QTreeWidgetItem(other)
    , m_pObject(other.m_pObject)
{
}

//-----------------------------------------------------------------------------
//
// ---
TreeObjectItem::~TreeObjectItem()
{
}

//-----------------------------------------------------------------------------
//
// ---
SceneSegment* TreeObjectItem::object() const
{
    return m_pObject;
}

//-----------------------------------------------------------------------------
//
// ---
TreeObjectItem& TreeObjectItem::operator = (const TreeObjectItem& other)
{
    if (this != &other)
    {
        QTreeWidgetItem::operator = (other);
        m_pObject = other.m_pObject;
    }
    return *this;
}

/* ExplodeTreeView */
ExplodeTreeView::ExplodeTreeView(ExplodeWidget* pExplodeWidget, QWidget* parent)
    : QTreeWidget(parent)
    , m_pExplodeWidget(pExplodeWidget)
{
    setFrameShape(QFrame::NoFrame);
    setHeaderHidden(true);
    setRootIsDecorated(false);
    setUniformRowHeights(false);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setAutoFillBackground(false);
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(slotTreeSelectionChanged()), Qt::DirectConnection);
}

//---------------------------------------------------------------------------
//
// ---
ExplodeTreeView::~ExplodeTreeView()
{
}

//---------------------------------------------------------------------------
//
// ---
TreeObjectItem* ExplodeTreeView::findItemByObject(const SceneSegment* object) const
{
    if (!object)
        return nullptr;
    TreeObjectItem* item = m_hashItems.value((SceneSegment*)object);
    return item != nullptr ? item : (!m_hashItems.isEmpty() ? m_hashItems.first() : nullptr);
}

//---------------------------------------------------------------------------
//
// ---
void ExplodeTreeView::slotTreeSelectionChanged()
{
    std::vector<uint> vObject;
    QList<QTreeWidgetItem*> selectedObjects = selectedItems();
    for (int i = 0, count = selectedObjects.count(); count > i; i++)
    {
        if (TreeObjectItem* pItem = dynamic_cast<TreeObjectItem*>(selectedObjects.at(i)))
            vObject.push_back(pItem->object()->GetUniqueKey().GetKey());
    }
    emit signalTreeSelectionChanged(vObject);
}

//---------------------------------------------------------------------------
//
// ---
void ExplodeTreeView::slotAppendItem(SceneSegment* pObject, SceneSegment* pParent)
{
    static bool bHasFirst = false;

    QString strName(pObject->GetObjectName().c_str());
//    if (strName == "Instance" /* || strName == "Assembly"*/)
//        return;

    if (pParent == NULL)
    {
        TreeObjectItem* pItem = new TreeObjectItem(pObject);
        pItem->setIcon(0, QIcon(":res/filterseg24x24.png"));
        m_hashItems.insert(pObject, pItem);
        addTopLevelItem(pItem);
        pItem->setExpanded(true);
    }
    else if (TreeObjectItem* pFindParent = findItemByObject(pParent))
    {
        TreeObjectItem* pItem = new TreeObjectItem(pObject);
        pItem->setIcon(0, QIcon(":res/filterbody24x24.png"));

        m_hashItems.insert(pObject, pItem);

        pFindParent->addChild(pItem);
        if (!bHasFirst)
            pItem->setExpanded(true);
    }

    if (!bHasFirst && pParent)
        bHasFirst = true;

    if (this->parentWidget())
        QApplication::postEvent(this->parentWidget(), new QEvent(QEvent::LayoutRequest));
}

//---------------------------------------------------------------------------
//
// ---
void ExplodeTreeView::slotChangedNameObject(const SceneSegment* object)
{
    if (TreeObjectItem* pItem = findItemByObject(object))
        pItem->setText(0, object->GetObjectName().c_str());
}

//---------------------------------------------------------------------------
//
// ---
void ExplodeTreeView::contextMenuEvent(QContextMenuEvent* event)
{
    m_pExplodeWidget->showContextMenu(event->globalPos());
}

//---------------------------------------------------------------------------
//
// ---
void ExplodeTreeView::enterEvent(QEvent* event)
{
    QTreeWidget::enterEvent(event);
    setCursor(QCursor(Qt::ArrowCursor));
}

