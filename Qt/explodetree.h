#ifndef __EXPLODETREE_H
#define __EXPLODETREE_H

#include <QTreeWidget>
#include <qt_openglwidget.h>

VSN_USE_NAMESPACE

/* TreeObjectItem */
class TreeObjectItem : public QTreeWidgetItem
{
public:
    TreeObjectItem(SceneSegment* object);
    TreeObjectItem(SceneSegment* object, QTreeWidgetItem* parent);
    TreeObjectItem(const TreeObjectItem& other);
    virtual ~TreeObjectItem();
    static bool m_bItemChanged;
public:
    SceneSegment* object() const;
    TreeObjectItem& operator = (const TreeObjectItem& other);
protected:
    SceneSegment* m_pObject;
};

class ExplodeWidget;
/* ExplodeTreeView */
class ExplodeTreeView : public QTreeWidget, public Object 
{
    Q_OBJECT
    VSN_OBJECT(ExplodeTreeView)
public:
    ExplodeTreeView(ExplodeWidget* pExplodeWidget, QWidget* parent = nullptr);
    virtual ~ExplodeTreeView();
public:
    TreeObjectItem* findItemByObject(const SceneSegment* object) const;
Q_SIGNALS:
    void signalTreeSelectionChanged(const std::vector<uint>& listObject);
    void signalTreeShowHideChanged(const std::list<SceneSegment*>& listObject);
public Q_SLOTS:
    void slotTreeSelectionChanged();
    void slotAppendItem(SceneSegment* pObject, SceneSegment* pParent);
    void slotChangedNameObject(const SceneSegment* object);
protected:
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
    virtual void enterEvent(QEvent* event) override;
protected:
    ExplodeWidget* m_pExplodeWidget;
    QMap<SceneSegment*, TreeObjectItem*> m_hashItems;
private:
    Q_DISABLE_COPY(ExplodeTreeView)
};


#endif // __EXPLODETREE_H
