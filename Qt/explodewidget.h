#ifndef __VSN_EXPLODEWIDGET_H
#define __VSN_EXPLODEWIDGET_H

#include <QListWidget>
#include <model_entity.h>
#include <scenegenerator.h>
#include <vsn_prcamerazoombox.h>
#include <vsn_mathgroupgeometry.h>
#include <vsn_mathgrouprepresentation.h>
#include <qt_openglwidget.h>
#include "explodetree.h"

struct ParamsParser;
class ExplodeManager;
class QPushButton;
class QCheckBox;
class QRadioButton;
class QGroupBox;
class QLabel;

VSN_USE_NAMESPACE

/* ListGroupGeometryItem */
class ListGroupGeometryItem : public QListWidgetItem
{
public:
    explicit ListGroupGeometryItem(SceneSegment* pSegModel, MathGroupItem* pData)
        : QListWidgetItem()
        , m_pSegModel(pSegModel)
        , m_pData(pData)
    {
        WString str = m_pData->GetText();
        if (str.IsEmpty())
            str = WString("Solid");
        setText(QString::fromWCharArray(str.c_str()));
        setIcon(QIcon(":/res/filterbody24x24.png"));
        setCheckState(Qt::Checked);
    }
public:
    SceneSegment* segmentModel() const 
    { return m_pSegModel; }
    MathGroupItem* dataItem() const
    { return m_pData; }
protected:
    SceneSegment* m_pSegModel;
    MathGroupItem* m_pData;
};

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
class ExplodeWidget : public QtVision::QtOpenGLSceneWidget
{
    Q_OBJECT
    VSN_OBJECT(ExplodeWidget)
public:
    explicit ExplodeWidget(QWidget* parent = nullptr);
    virtual ~ExplodeWidget();
public:
    QGroupBox* createGroupExplode(QWidget& widget, const int heightButton, const std::string& mainTabName);
    void showContextMenu(const QPoint& pos);
    ExplodeTreeView* createGeometryList(QWidget* parent);
public Q_SLOTS:
    void openModel();
    void setRenderingMode();
protected:
    void slotItemSelectModified();
    void slotCurrentItemsModified(std::list<SelectionItem*>& oldItems, std::list<SelectionItem*>& newItems);
private:
    virtual void initializeGL();
    void loadFiles(const QStringList& files);
    void createScene();
    void fillGeometryList();
    void hideSelectedObjects();
    void showAllObjects();
    void clearSelectionObjects();
Q_SIGNALS:
    void setVisibleProgress(bool);
    void buildProgress(int);
    void setProgressRange(int minimum, int maximum);
    void resetProgress();
protected Q_SLOTS:
    void currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void itemChanged(QTreeWidgetItem* item);
public:
    VSN_SLOT(Public, slotFinishBuildRep, void slotFinishBuildRep())
protected:
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
private:
    SPtr<MbModel> m_pModel;
    SceneSegment* m_pSegmModel;
    SceneGenerator* m_pSceneGenerator;
    ExplodeManager* m_pExplodeManager;
    SelectionManagerPtr m_ptrSelectManager;
    ExplodeTreeView* m_pTreeWidget;
    std::string m_modelPath;
private:
    Q_DISABLE_COPY(ExplodeWidget)
};

#endif // __VSN_EXPLODEWIDGET_H
