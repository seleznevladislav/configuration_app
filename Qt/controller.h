#ifndef __CONTROLLER_H
#define __CONTROLLER_H

#include <QObject>

#include "vsn_prchangewidget.h"
#include "vsn_widgetmodels.h"
#include "vsn_graphicsscene.h"
#include "mb_cube.h"

VSN_BEGIN_NAMESPACE

class DefaultControllers
{
public:
    static const std::shared_ptr<Widget3DModel> xyzController;
    static const std::shared_ptr<Widget3DModel> directController;
};

class DefaultProcesses
{
public:
    static const std::shared_ptr<WidgetProcess> spaceMove;
    static const std::shared_ptr<WidgetProcess> xlocalMove;
    static const std::shared_ptr<WidgetProcess> ylocalMove;
    static const std::shared_ptr<WidgetProcess> zlocalMove;

    static const std::shared_ptr<WidgetProcess> xylocalMove;
    static const std::shared_ptr<WidgetProcess> xRotate;
    static const std::shared_ptr<WidgetProcess> yRotate;
    static const std::shared_ptr<WidgetProcess> zRotate;
};


//class SceneWidget;

class Controller : public  VSN::Object
{
    VSN_OBJECT(Controller);
public:
    Controller(GraphicsScene * pScene);
    virtual ~Controller();
public:
    enum ControllerViews { NoneCtrl, xyzCtrl, directCtrl };
    void SetSectionPlaneID(SectionPlaneId sectionPlaneID);
    void Show();
    void Hide();
    void SetControllerView(ControllerViews view);
    void UpdateControllerWidget();
public:
    VSN_SLOT(Public, slotModify, void slotModify(SceneWidget*));
    VSN_SIGNAL(Public, signalUpdate, void signalUpdate());
private:
    SceneWidget* m_xyzWidget = nullptr;
    SceneWidget* m_directionWidget = nullptr;
    ControllerViews currentControllerView = NoneCtrl;
    GraphicsScene* m_pScene = nullptr;
    SectionPlaneId m_curIdPlane; // current section plane id for edit
    MbMatrix3D GetControllerMatrix();
};

VSN_END_NAMESPACE
#endif
