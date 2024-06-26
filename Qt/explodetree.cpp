﻿#include "explodetree.h"
#include <qevent.h>
#include "explodewidget.h"
#include <last.h>
#include <iostream>
#include <string>
#include <sstream>
#include <regex>

//-----------------------------------------------------------------------------
//
// ---
TreeObjectItem::TreeObjectItem(SceneSegment* pObject, int currentExhangerType)
    : m_pObject(pObject)
{
    QString strName(pObject->GetObjectName().c_str());

    int nameIndex = -1;

    string findName = strName.toStdString();

    // Поиск числа в строке
    regex regex("\\d+");
    smatch match;
    if (regex_search(findName, match, regex)) {
        // Извлечение найденного числа
        string numberStr = match.str();
        // Преобразование строки с числом в целое число
        nameIndex = stoi(numberStr);
    }

    vector<QString> detailNames;

    switch (currentExhangerType)
    {
        case 1:
        {
            detailNames = {
                u8"Труба теплообменная", u8"Труба кожуховая", u8"Опора", u8"Решетка кожуховых труб",
                u8"Камера", u8"Решетка теплообменных труб", u8"Фланец кожуховый", u8"Фланец специальный",
                u8"Труба изогнутая", u8"Соединение", u8"Соединение с выходом", u8"Труба изогнутая соединительная",
                u8"Сборка ТТОР",u8"Итоговая сборочная единица", 
                u8"Болт M8", u8"Шайба M8",  u8"Гайка M8",
                u8"Болт M10", u8"Шайба M10",  u8"Гайка M10",
                u8"Болт M12", u8"Шайба M12",  u8"Гайка M12",
                u8"Болт M16", u8"Шайба M16",  u8"Гайка M16", u8"ТТОР",
            };

            break;
        }
        case 2:
        {
            detailNames = { 
                u8"Теплообменная труба", 
                u8"Труба кожуховая", 
                u8"Камера распределительная", 
                u8"Камера поворотная", 
                u8"Опора", 
                u8"Решетка кожуховых труб", 
                u8"Фланец Тип E ГОСТ 33259-2015 Исполнение 1", 
                u8"Фланец Тип F ГОСТ 33259-2015 Исполнение 1", 
                u8"Гайка шестигранная низкая ГОСТ 15522-70 Исполнение 1", 
                u8"Болт с шестигранной головкой ГОСТ 15589-70 Исполнение 3", 
                u8"Винт установочный ГОСТ 1477-93", 
                u8"Гайка установочная",
                u8"Теплообменная система",
                u8"ТТРМ",
                u8"Болт дренажный",
            };

            break;
        }
        case 3:
        {
            detailNames = {
                u8"Кожух", u8"Распределительная камера", u8"Заглушка фланцевая 7", u8"Заглушка фланцевая 6",
                u8"Заглушка фланцевая 11", u8"Заглушка фланцевая 10", u8"Заглушка фланцевая 9", u8"Заглушка фланцевая 3",
                u8"Заглушка фланцевая 4", u8"Заглушка фланцевая 5", u8"Заглушка фланцевая 1", u8"Опора",
                u8"Решетка неподвижная", u8"Перегородка",
                u8"Трубы", u8"ИУ"
            };

            break;
        }
        case 4:
        {
            detailNames = { u8"Кожух", u8"Распределительная камера", u8"Эллиптическая крышка распределительной камеры", u8"Заглушка фланцевая",
                            u8"Опора", u8"Неподвижная трубная решетка", u8"Перегородка", u8"Подвижная трубная решетка",
                            u8"Кольцо плавающей головки", u8"Эллиптическая крышка плавающей головки", u8"Трубы", u8"ИП" };

            break;
        }
    }

    stringstream(pObject->GetObjectName().c_str()) >> nameIndex;
    
    QString detailName = detailNames.at(nameIndex);

    setText(0, match.empty() ?  strName : detailName);
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
    connect(this, SIGNAL(itemSelectionChanged()), this,
        SLOT(slotTreeSelectionChanged()), Qt::DirectConnection);
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
    const int currentExhangerType = m_pExplodeWidget->m_pCurrentExchandger;

    QString strName(pObject->GetObjectName().c_str());

    if (pParent == NULL)
    {
        TreeObjectItem* pItem = new TreeObjectItem(pObject, currentExhangerType);
        pItem->setIcon(0, QIcon(":res/filterseg24x24.png"));
        m_hashItems.insert(pObject, pItem);
        addTopLevelItem(pItem);
        pItem->setExpanded(true);
    }
    else if (TreeObjectItem* pFindParent = findItemByObject(pParent))
    {
        TreeObjectItem* pItem = new TreeObjectItem(pObject, currentExhangerType);
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

