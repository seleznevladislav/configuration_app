#ifndef __VSN_EXPLODEWIDGET_H
#define __VSN_EXPLODEWIDGET_H

// C3D
#include <model_entity.h>
#include <scenegenerator.h>
#include <vsn_prcamerazoombox.h>
#include <vsn_mathgroupgeometry.h>
#include <vsn_mathgrouprepresentation.h>
#include <model.h>
#include <model_item.h>

// QT
#include <qt_openglwidget.h>
#include <QListWidget>
#include <QColorDialog>
#include <QResizeEvent>
#include <QActionGroup>
#include <QPushButton>
#include <QTableWidget>
#include <QFile>
#include <QRadioButton>
#include <QTimer>
#include <QTime>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>

// USERS
#include "explodetree.h"
#include "controller.h"

struct ParamsParser;
class ExplodeManager;
class QPushButton;
class QCheckBox;
class QRadioButton;
class QGroupBox;
class QLabel;

VSN_USE_NAMESPACE

class PrBase;
class AssemblySolver;

using AssemblySolverPtr = std::shared_ptr<AssemblySolver>;
using AssemblySolverConstPtr = std::shared_ptr<const AssemblySolver>;
using namespace std;


/* Properties */
class Properties : public QGroupBox
{
    Q_OBJECT
public:
    explicit Properties(const QString& strTitle, DimensionRep* dimensionRep, QWidget* parent = nullptr);
    virtual ~Properties();
Q_SIGNALS:
    void updateView();
public Q_SLOTS:
    void textOrientationChanged(int);
    void vTextOrientationChanged(int);
    void hTextOrientationChanged(int);
protected:
    DimensionRep* m_dimensionRep;
    QComboBox* m_cbTextOrientation;
    QComboBox* m_cbTextVPosition;
    QComboBox* m_cbTextHPosition;
};

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

/*ColorButton*/
class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    ColorButton(QWidget* parent);
public Q_SLOTS:
    void chooseColor();
    void setColor(const QColor& clr);
Q_SIGNALS:
    void colorChanged(const QColor& color);
private:
    QColor m_color;
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
    enum State
    {
        Selection,
        LinearDimension,
        AngleDimension,
        DiameterDimension,
        RadialDimension,
    };
    enum Commands
    {
        None,
        Open,
        Save,
        Select,
        Cancel,
        Comfirm,
        // Input processes.
        InsertItem,
        FixItem,
        Coincident,
        Coaxial,
        Parallel,
        Perpendicular,
        Distance,
        Angular,
        Rotation,
    };
    enum Exhanchares
    {
        NoneOfOne,
        TTOR,
        TTRM,
        IU,
        IP,
    };
public:
    void setMainLayout(QVBoxLayout* mainLayout);
    void updateButtons();
    void removePropertiesWidget();
    QColor highlightColor() const;
    QColor selectionColor() const;

    SPtr<MbModel> model() { return m_pModel; }
    AssemblySolverConstPtr solver() const { return m_pSolver; }
    AssemblySolverPtr solver() { return m_pSolver; }
    SceneSegment* modelSegment() const { return m_pModelSeg; }
    SceneSegment* assemblySegment() const; // Get the main assembly node.
    SelectionManagerPtr selectManager() const { return m_ptrSelectManager; }
    ExplodeManager* getExplodeManager() const { return m_pExplodeManager; }

    QGroupBox* createGroupExplode(QWidget& widget, const int heightButton, const std::string& mainTabName);
    void showContextMenu(const QPoint& pos);
    ExplodeTreeView* createGeometryList(QWidget* parent);
    inline void setGroupFilter(QActionGroup* groupFilter);
    int m_pCurrentExchandger;
public Q_SLOTS:
    void stopBuildDimensions();
    void createLinearDimensions();
    void createAngleDimensions();
    void createDiameterDimensions();
    void createRadialDimensions();
    void viewCommands(Commands cmd = None);
    void viewCommandsHeats(Exhanchares cmd = NoneOfOne);
    void openModel();
    void setRenderingMode();
    void configureModel(int index);
    void slotFilterTriggered(QAction*);
    void slotRenderTriggered(QAction*);
    void slotToggleVisibility(bool checked, QGroupBox* groupBox);
    void slotHighlightColor(const QColor& clr);
    void slotSelectionColor(const QColor& clr);
    void slotDynamicHighlighting(int state);
    QString openSelectModel();
    void saveModel();
    void loadModel();
    void enableChanged(int visible);
    void closeChanged(int visible);
    void invertChanged(int visible);
    void togglePlaneCuttingVisibility(bool checked, QGroupBox* groupBox);
    double GetAngle1() const { return m_angle1; }
    double GetAngle2() const { return m_angle2; }
    double GetOffset() const { return m_offset; }
    void controllerChanged(int state);
    void getParameters();
    void updateState();
    void offsetChanged(double val);
    void angleA1Changed(double val);
    void angleA2Changed(double val);
    void freeSectionChanged(bool state);
    void offsetSectionChanged(bool state);
    void animationSwitchSlot();
    void animation();
    void animationSlot();
    void toggleCuttingState();
public:
    VSN_SLOT(Public, slotUpdateCommands, void slotUpdateCommands());
protected:
    void slotItemSelectModified();
    void slotCurrentItemsModified(std::list<SelectionItem*>& oldItems, std::list<SelectionItem*>& newItems);
    void updateActionCheckFilter();
    void clearProperties();
    MbCartPoint3D mapPointOnPlane(int x, int y, const MbPlane& pl) const;
private:
    virtual void initializeGL();
    void loadFiles(const QStringList& files);
    void createScene();
    void createSceneZarubin();
    void createSceneIP();
    void createSceneIU();
    void fillGeometryList();
    void hideSelectedObjects();
    void showAllObjects();
    void clearSelectionObjects();
Q_SIGNALS:
    void setCheckedSelect(bool);
    void setCheckedLinear(bool);
    void setCheckedAngle(bool);
    void setCheckedDiameter(bool);
    void setCheckedRadial(bool);
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
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event) override;
private:
    AssemblySolverPtr m_pSolver;
    SceneSegment* m_pModelSeg;
    PrBase* m_pCurrentProcess;
    SPtr<MbModel> m_pModel;
    SceneSegment* m_pSegmModel;
    SceneGenerator* m_pSceneGenerator;
    ExplodeManager* m_pExplodeManager;
    SelectionManagerPtr m_ptrSelectManager;
    ExplodeTreeView* m_pTreeWidget;
    QActionGroup* m_pGroupFilter;
    std::string m_modelPath;

    QCursor m_curEdge;
    QCursor m_curFace;
    QCursor m_curVertex;
    QCursor m_curPoint;

    QString m_fileName;
    SectionPlaneId m_curIdPlane;
    Controller::ControllerViews defaultView = Controller::ControllerViews::directCtrl;
    Controller* m_controller;

    QTimer* timer;
    double         m_angle1;
    double         m_angle2;
    double         m_offset;

    /// <summary>
    /// params for dimensions
    /// </summary>
    State m_state;
    bool m_startBuildDimension;
    QVBoxLayout* m_mainLayout;
    Properties* m_properties;
    MbModel* m_model;
    MbCartPoint3D* m_point1;
    MbCartPoint3D* m_point2;
    MbCartPoint3D* m_point3;
    DimensionRep* m_dimensionRep;
    double m_lineHeight = 60;
private:
    Q_DISABLE_COPY(ExplodeWidget)
};

inline void ExplodeWidget::setGroupFilter(QActionGroup* groupFilter)
{
    m_pGroupFilter = groupFilter;
}

#endif // __VSN_EXPLODEWIDGET_H
