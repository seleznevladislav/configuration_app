#include "controller.h"

#include <math.h>
#include <cur_line3d.h>
#include <mb_axis3d.h>

#include <vsn_coordlocator.h>
#include <vsn_prchangewidget.h>
#include <vsn_widgetmodels.h>
#include <surf_plane.h>
#include <vsn_widgets.h>

#include <last.h>

enum IdProcess { IdNone, IdZlocalMove, IdXRotate, IdYRotate };

VSN_USE_NAMESPACE

std::shared_ptr<Widget3DModel> createBaseModel()
{
    std::shared_ptr<Widget3DModel> model(new Widget3DModel());

    float l = 40.0f;
    float lc = l / 8.0f;
    float lb = l * 1.1f;
    float ld = l * 0.4f;
    float lw = l / 14.3f;
    float la = l / 6.5f;

    model->AddCylinder(IdZlocalMove, {}, { 0, 0, lb }, lw, Color(20, 140, 200));
    model->AddCone(IdZlocalMove, { 0, 0, lb }, { 0, 0, lb + ld }, la, Color(20, 140, 200));
    model->AddCube(IdZlocalMove, { -lc, -lc ,-lc }, { lc, lc, lc }, Color(220, 220, 220));

    model->AddTorus(IdXRotate, {}, { l, 0, 0 }, lw, Color(140, 200, 20), 0.3, (M_PI / 2.0 - 0.3));
    model->AddCone(IdXRotate, { 0, l - lw, ld }, { 0, l, 0 }, lc, Color(140, 200, 20));
    model->AddCone(IdXRotate, { 0, ld, l - lw }, { 0, 0, l }, lc, Color(140, 200, 20));
    model->AddDisk(IdNone, Color(220, 220, 220),  MbPlacement3D({ 0, l, 0 }), lc, 0.5);

    model->AddTorus(IdYRotate, {}, { 0, l, 0 }, lw, Color(200, 20, 140), - 0.3, ( - M_PI / 2.0 + 0.3));
    model->AddCone(IdYRotate, { l - lw, 0, ld }, { l, 0, 0 }, lc, Color(200, 20, 140));
    model->AddCone(IdYRotate, { ld, 0, l - lw }, { 0, 0, l }, lc, Color(200, 20, 140));
    model->AddDisk(IdNone, Color(220, 220, 220), MbPlacement3D({ l, 0, 0 }), lc, 0.5);

    return model;
}

std::shared_ptr<Widget3DModel> createDirectModel()
{
    std::shared_ptr<Widget3DModel> model(new Widget3DModel());

    float l = 40.0f;
    float ln = l * 1.4f;
    float lc = l / 8.0f;
    float lw = l / 14.3f;
    float la = l / 6.5f;

    model->AddCylinder(IdZlocalMove, {}, { 0, 0, l }, lw, Color(20, 140, 200));
    model->AddCone(IdZlocalMove, { 0, 0, l }, { 0, 0, ln }, la, Color(20, 140, 200));
    model->AddCube(IdZlocalMove, { -lc, -lc, -lc }, { lc, lc, lc }, Color(220, 220, 220));

    return model;
}

std::shared_ptr<WidgetProcess> createSpaceMoveProcess()
{
    std::shared_ptr<MoveWidgetProcess> process(new MoveWidgetProcess(nullptr));
    process->SetLocator(new ModelCoordLocator(process.get()));

    return process;
}

std::shared_ptr<WidgetProcess> createCurveMoveProcess(SPtr<const MbCurve3D> curve, bool local = true)
{
    std::shared_ptr<MoveWidgetProcess> process(new MoveWidgetProcess(nullptr));
    process->SetLocator(new CurveScreenCoordLocator(curve, process.get()));
    process->SetConvertPlacement(local ? WidgetConvertPlacements::Widget : WidgetConvertPlacements::Scene);

    return process;
}

std::shared_ptr<WidgetProcess> createSurfaceMoveProcess(SPtr<const MbSurface> surface, bool local = true)
{
    std::shared_ptr<MoveWidgetProcess> process(new MoveWidgetProcess(nullptr));

    process->SetLocator(new SurfaceScreenCoordLocator(surface, process.get()));
    process->SetConvertPlacement(local ? WidgetConvertPlacements::Widget : WidgetConvertPlacements::Scene);

    return process;
}

std::shared_ptr<WidgetProcess> createRotateProcess(const MbAxis3D & axis, bool local = true)
{
    std::shared_ptr<RotateWidgetProcess> process(new RotateWidgetProcess(axis));

    return process;
}

Controller::Controller(GraphicsScene * pScene)
    : m_pScene(pScene)
{
    // creation of a three - axis controller and binding to it the processes of movement and rotation
    m_xyzWidget = new SceneWidget(pScene, DefaultControllers::xyzController);
    m_xyzWidget->AssignProcess(IdZlocalMove, DefaultProcesses::zlocalMove.get());
    m_xyzWidget->AssignProcess(IdXRotate, DefaultProcesses::xRotate.get());
    m_xyzWidget->AssignProcess(IdYRotate, DefaultProcesses::yRotate.get());
    m_xyzWidget->SetCustomKey(NodeKey(0));

    Object::Connect(m_xyzWidget, &SceneWidget::OnModified, this, &Controller::slotModify);

    // creation of a axial - arrow controller and binding to it the processes of movement by z - axis
    m_directionWidget = new SceneWidget(pScene, DefaultControllers::directController);
    m_directionWidget->AssignProcess(IdZlocalMove, DefaultProcesses::zlocalMove.get());
    m_directionWidget->SetCustomKey(NodeKey(0));

    Object::Connect(m_directionWidget, &SceneWidget::OnModified, this, &Controller::slotModify);
}

void Controller::SetSectionPlaneID(SectionPlaneId sectionPlaneID)
{
    m_curIdPlane = sectionPlaneID;
    UpdateControllerWidget();
}

void Controller::Show()
{
    switch (currentControllerView)
    {
    case Controller::NoneCtrl:
        m_xyzWidget->SetVisible(false);
        m_directionWidget->SetVisible(false);
        break;
    case Controller::xyzCtrl:
        m_xyzWidget->SetVisible(true);
        m_directionWidget->SetVisible(false);
        break;
    case Controller::directCtrl:
        m_xyzWidget->SetVisible(false);
        m_directionWidget->SetVisible(true);
        break;
    }
}

void Controller::Hide()
{
    m_xyzWidget->SetVisible(false);
    m_directionWidget->SetVisible(false);
}

void Controller::SetControllerView(ControllerViews view)
{
    currentControllerView = view;
    UpdateControllerWidget();
    Show();
    emit signalUpdate();
}

void Controller::slotModify(SceneWidget* item)
{
    if (currentControllerView != NoneCtrl)
    {       
        //changing the position of the section plane by changing the position of the controller
        MbPlacement3D placement(item->GetTransform());
        if (!m_pScene->GetCuttingTool()->IsInverted(m_curIdPlane))
            placement.Invert();
        m_pScene->GetCuttingTool()->Init(m_curIdPlane, placement);
    }
}

void Controller::UpdateControllerWidget()
{
    switch (currentControllerView)
    {
    case Controller::NoneCtrl:
        break;
    case Controller::xyzCtrl:
        m_xyzWidget->Init(GetControllerMatrix());
        break;
    case Controller::directCtrl:     
        m_directionWidget->Init(GetControllerMatrix());
        break;
    }
}

MbMatrix3D Controller::GetControllerMatrix()
{
    //getting the center of the section plane for controller position
    auto tool = m_pScene->GetCuttingTool();
    auto & placement = tool->GetPlacement(m_curIdPlane);
    MbCube cube = m_pScene->GetGlobalBoundingBox();
    MbRect rect;
    cube.ProjectionRect(placement, rect);
    MbCartPoint center;
    rect.GetCenter(center);
    MbCartPoint3D center3D;
    placement.PointOn(center, center3D);
    MbVector3D controllerVector = placement.Normal();
    if (!tool->IsInverted(m_curIdPlane))
        controllerVector.Invert();
    MbPlacement3D normalCenter3D(center3D, controllerVector);
    MbMatrix3D controllerMatrix = normalCenter3D.GetMatrixFrom();

    return controllerMatrix;
}

Controller::~Controller()
{
}


const std::shared_ptr<Widget3DModel> DefaultControllers::xyzController = createBaseModel();
const std::shared_ptr<Widget3DModel> DefaultControllers::directController = createDirectModel();

const std::shared_ptr<WidgetProcess> DefaultProcesses::spaceMove = createSpaceMoveProcess();
const std::shared_ptr<WidgetProcess> DefaultProcesses::xlocalMove = createCurveMoveProcess(SPtr<MbCurve3D>(new MbLine3D({}, MbCartPoint3D{ 1,0,0 })));
const std::shared_ptr<WidgetProcess> DefaultProcesses::ylocalMove = createCurveMoveProcess(SPtr<MbCurve3D>(new MbLine3D({}, MbCartPoint3D{ 0,1,0 })));
const std::shared_ptr<WidgetProcess> DefaultProcesses::zlocalMove = createCurveMoveProcess(SPtr<MbCurve3D>(new MbLine3D({}, MbCartPoint3D{ 0,0,1 })));

const std::shared_ptr<WidgetProcess> DefaultProcesses::xylocalMove = createSurfaceMoveProcess(SPtr<MbSurface>(new MbPlane()));
const std::shared_ptr<WidgetProcess> DefaultProcesses::xRotate = createRotateProcess(MbAxis3D::xAxis);
const std::shared_ptr<WidgetProcess> DefaultProcesses::yRotate = createRotateProcess(MbAxis3D::yAxis);
const std::shared_ptr<WidgetProcess> DefaultProcesses::zRotate = createRotateProcess(MbAxis3D::zAxis);