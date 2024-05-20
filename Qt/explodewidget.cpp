#include <windows.h>
#include <QFileDialog>
#include <QToolButton>
#include <QMenu>
#include <qevent.h>
#include <vsn_brush.h>

#include <attr_product.h>
#include <instance.h>
#include <conv_predefined.h>
#include <conv_model_exchange.h>
#include <conv_exchange_settings.h>
#include <vsn_postprocessing.h>
#include <qt_aboutscenewidget.h>
#include <qpushbutton.h>

#include "BuildMathModel.h"
#include "BuildParams.h"
#include "explodewidget.h"
#include "solver/assemblysolver.h"
#include "solver/prbase.h"
#include "solver/prbinconstraint.h"
#include "explodemanager.h"

#include <last.h>

using namespace BuildMathModel;

static void _selectSegment(SelectionManagerPtr ptrSelectManager, SceneSegment* pSegment)
{
    if (pSegment->GetRepresentation() == nullptr)
    {
        auto listItem = pSegment->GetChildSegments();
        for (auto it = listItem.begin(); it != listItem.end(); ++it)
        {
            SceneSegment* pSegmentItem = (*it);
            if (pSegmentItem->GetRepresentation() == nullptr)
                _selectSegment(ptrSelectManager, pSegmentItem);
            else
                ptrSelectManager->Select(pSegmentItem->GetUniqueKey());
        }
    }
}

static void fillSegmentList(ExplodeTreeView* pTreeWidget, SceneSegment* pSegment, SceneSegment* pPrentSegment)
{
    pTreeWidget->slotAppendItem(pSegment, pPrentSegment);
    auto listItem = pSegment->GetChildSegments();
    for (auto it = listItem.begin(); it != listItem.end(); ++it)
    {
        SceneSegment* pSegmentItem = (*it);
        fillSegmentList(pTreeWidget, pSegmentItem, pSegment);
    }
}

/* ExplodeWidget */
ExplodeWidget::ExplodeWidget(QWidget* pParent)
    : QtOpenGLSceneWidget(pParent)
    , m_pModel(nullptr)
    , m_pSegmModel(nullptr)
    , m_pSceneGenerator(new SceneGenerator())
    , m_pExplodeManager(new ExplodeManager(this))
    , m_ptrSelectManager(std::make_shared<SelectionManager>())
    , m_pTreeWidget(nullptr)
    , m_pSolver(nullptr)
    , m_pModelSeg(nullptr)
    , m_pCurrentProcess(nullptr)
    , m_pGroupFilter(nullptr)
    , m_state(ExplodeWidget::Selection)
    , m_startBuildDimension(false)
    , m_mainLayout(nullptr)
    , m_properties(nullptr)
    , m_point1(nullptr)
    , m_point2(nullptr)
    , m_point3(nullptr)
    , m_dimensionRep(nullptr)
    , m_curPoint(QtVision::findCursor(16)/*QCursor(QPixmap(":/res/point_cur.cur"))*/)
    , m_curEdge(QCursor(QPixmap(":/res/cursors/edge.cur")))
    , m_curFace(QCursor(QPixmap(":/res/cursors/face.cur")))
    , m_curVertex(QCursor(QPixmap(":/res/cursors/vertex.cur")))
{
    m_pModel = new MbModel;
    m_pSolver = std::make_shared<AssemblySolver>(UniteToMainAssembly(m_pModel));
    QtVision::ProcessTypes ptTypes = QtVision::pt_Pan | QtVision::pt_Zoom | QtVision::pt_Rotate;
    QtVision::createProcessesCameraControls(this, ptTypes);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(animationSlot()));
}

ExplodeWidget::~ExplodeWidget()
{
    VSN_DELETE_AND_NULL(m_pSceneGenerator);
    VSN_DELETE_AND_NULL(m_pExplodeManager);
}

QGroupBox* ExplodeWidget::createGroupExplode(QWidget& widget, const int heightButton, const std::string& mainTabName)
{
    return m_pExplodeManager->createGroupExplode(widget, heightButton, mainTabName);
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeWidget::initializeGL()
{
    QtOpenGLSceneWidget::initializeGL();

    Light* pLight = graphicsScene()->CreateExtraLight();
    pLight->SetType(Light::DirectionLight);
    pLight->SetPosition(-100.0, -1.0, -1.0);
    pLight->SetDoubleSided(false);

    GlobalFlags::SetPixelCullingEnabled(true);

    GlobalFlags::SetFaceCulling(CullFaces::Back);
    viewport()->GetCamera()->SetViewOrientation(Orientation::IsoXYZ);
    // viewport()->SetGradientBackgroundColour(QtVision::topColor, QtVision::bottomColor);
    // Для Рисунков
    viewport()->SetGradientBackgroundColour(Color(255, 255, 255), Color(255, 255, 255));
    //viewport()->SetGradientBackgroundColour(Color(0, 255, 0), Color(0, 255, 0));
    viewport()->SetPixelCullingSize(10);
    mainLight()->SetDoubleSided(false);
    graphicsView()->SetSmoothTransition(true);

    // надо написать manager для процессов
    ObjectPickSelection* pPickSelection = objectPickSelection();
    m_ptrSelectManager->SetSceneContent(sceneContent());
    m_ptrSelectManager->SetObjectPickSelection(pPickSelection);
    m_ptrSelectManager->SetSelectionMode(SelectionManager::MultiSelection);
    m_ptrSelectManager->SetDynamicHighlighting(true);
    m_ptrSelectManager->SetBodySelectionEnabled(true);

    Object::Connect(m_ptrSelectManager.get(), &SelectionManager::signalCurrentItemsModified, this, &ExplodeWidget::slotCurrentItemsModified);
    Object::Connect(m_ptrSelectManager.get(), &SelectionManager::signalItemSelectModified, this, &ExplodeWidget::slotItemSelectModified);
    Object::Connect(m_ptrSelectManager.get(), &SelectionManager::signalStateModified, this, &QtOpenGLWidget::updateWidget);

    OrientationMarker* pMarker = graphicsView()->GetOrientationMarker();
    if (pMarker) pMarker->SetStyle(OrientationMarker::Style::BoxMarker);
    if (pMarker) pMarker->SetEnabled(true);
}

void ExplodeWidget::setMainLayout(QVBoxLayout* mainLayout)
{
    m_mainLayout = mainLayout;
}

// ---
void ExplodeWidget::updateButtons()
{
    emit setCheckedSelect(m_state == ExplodeWidget::Selection);
    emit setCheckedLinear(m_state == ExplodeWidget::LinearDimension);
    emit setCheckedAngle(m_state == ExplodeWidget::AngleDimension);
    emit setCheckedDiameter(m_state == ExplodeWidget::DiameterDimension);
    emit setCheckedRadial(m_state == ExplodeWidget::RadialDimension);
}

// ---
void ExplodeWidget::removePropertiesWidget()
{
    if (m_properties)
    {
        m_mainLayout->removeWidget(m_properties);
        delete m_properties;
        m_properties = nullptr;
    }
}

// ---
void ExplodeWidget::clearProperties()
{
    removePropertiesWidget();
    m_state = ExplodeWidget::Selection;
    updateButtons();
}

// ---
MbCartPoint3D ExplodeWidget::mapPointOnPlane(int x, int y, const MbPlane& pl) const
{
    MbCartPoint3D retPnt;
    Point3DF worldPoint;
    MbCartPoint logPos(x, y);
    viewport()->ConvertScreenPointToParallelScreenPlane(logPos, MbCartPoint3D(), worldPoint);
    double u, v;
    if (pl.NearDirectPointProjection(worldPoint, viewport()->GetCamera()->GetForward(), u, v, true))
        pl.PointOn(u, v, retPnt);
    return retPnt;
}

// ---
void ExplodeWidget::stopBuildDimensions()
{
    clearProperties();
}

// ---
void ExplodeWidget::createLinearDimensions()
{
    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    LinearDimensionRep* pLinearDimensionRep = GeometryFactory::Instance()->CreateLinearDimension();
    pLinearDimensionRep->SetExtensionLineHeight(m_lineHeight / graphicsView()->GetViewport()->GetScale());
    SceneSegment* pSegment = new SceneSegment(pLinearDimensionRep);
    pTopSegment->AddSegment(pSegment);
    m_dimensionRep = pLinearDimensionRep;
    removePropertiesWidget();

    const char* test = u8"";
    m_properties = new Properties(test, m_dimensionRep);
    QObject::connect(m_properties, SIGNAL(updateView()), this, SLOT(update()));
    m_mainLayout->addWidget(m_properties);
    m_state = ExplodeWidget::LinearDimension;
    updateButtons();
}

// ---
void ExplodeWidget::createAngleDimensions()
{
    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    AngleDimensionRep* pAngleDimensionRep = GeometryFactory::Instance()->CreateAngleDimension();
    pAngleDimensionRep->SetExtensionLineHeight(m_lineHeight / graphicsView()->GetViewport()->GetScale());
    SceneSegment* pSegment = new SceneSegment(pAngleDimensionRep);
    pTopSegment->AddSegment(pSegment);
    m_dimensionRep = pAngleDimensionRep;
    removePropertiesWidget();
    const char* test = u8"";
    m_properties = new Properties(test, m_dimensionRep);
    QObject::connect(m_properties, SIGNAL(updateView()), this, SLOT(update()));
    m_mainLayout->addWidget(m_properties);
    m_state = ExplodeWidget::AngleDimension;

    pAngleDimensionRep->SetFontSize(20);
    pAngleDimensionRep->SetLabelColor(Color(200, 0, 0));
    pAngleDimensionRep->SetDimensionColor(Color(200, 160, 0));

    updateButtons();
}

//---
void ExplodeWidget::createDiameterDimensions()
{
    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    DiameterDimensionRep* pDiameterDimensionRep = GeometryFactory::Instance()->CreateDiameterDimension();
    SceneSegment* pSegment = new SceneSegment(pDiameterDimensionRep);
    pTopSegment->AddSegment(pSegment);
    m_dimensionRep = pDiameterDimensionRep;
    removePropertiesWidget();
    const char* test = u8"";
    m_properties = new Properties(test, m_dimensionRep);
    QObject::connect(m_properties, SIGNAL(updateView()), this, SLOT(update()));
    m_mainLayout->addWidget(m_properties);
    m_state = ExplodeWidget::DiameterDimension;
    updateButtons();
}

//---
void ExplodeWidget::createRadialDimensions()
{
    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    RadialDimensionRep* pRadialDimensionRep = GeometryFactory::Instance()->CreateRadialDimension();
    SceneSegment* pSegment = new SceneSegment(pRadialDimensionRep);
    pTopSegment->AddSegment(pSegment);
    m_dimensionRep = pRadialDimensionRep;
    removePropertiesWidget();
    const char* test = u8"";
    m_properties = new Properties(test, m_dimensionRep);
    QObject::connect(m_properties, SIGNAL(updateView()), this, SLOT(update()));
    m_mainLayout->addWidget(m_properties);
    m_state = ExplodeWidget::RadialDimension;
    updateButtons();
}

// TODO: неиспользуемая функция
void ExplodeWidget::configureModel(int index)
{
    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);
    const QString lastUserPath;
    QStringList filters = QtVision::openSaveFilters();
    QString oneLineFilters = filters.join("\n");

    createScene();
    fillGeometryList();
}

void ExplodeWidget::slotUpdateCommands()
{
    if (m_pCurrentProcess == nullptr)
        return;
    QWidget* parentWidget = qobject_cast<QWidget*>(parent());
    QList<QAction*> lstAction = parentWidget->actions();
    for (QAction* pAction : lstAction)
    {
        Commands value = static_cast<Commands>(pAction->property("Commands").toInt());

        if (m_pCurrentProcess != nullptr)
        {
            const MbAssembly* assm = GetMainAssembly(m_pModel);  // The main assembly.
            VSN_ASSERT(assm != nullptr);
            switch (value)
            {
            case Save:
            case FixItem:
            case Rotation:
                pAction->setEnabled(m_pCurrentProcess->IsReady());
                break;
            case Cancel:
                pAction->setEnabled(m_pCurrentProcess->IsReady());
                break;
            case Comfirm:
                pAction->setEnabled(m_pCurrentProcess->IsReady());
                break;
            case InsertItem:
                pAction->setEnabled(m_pCurrentProcess != nullptr &&
                    m_pCurrentProcess->GetCustomKey() == NodeKey(Select));
                break;
            case Coincident:
            case Coaxial:
            case Parallel:
            case Perpendicular:
            case Angular:
            case Distance:
                pAction->setEnabled(assm != nullptr && (assm->ItemsCount() > 1));
                break;
                /*
                case Rotation:
                    const bool bEnabledAxis = (assm != nullptr) && (assm->ItemsCount() > 1) &&
                         PrRotationAboutAxis::SelectedAxialGeom(this,nullptr).SubGeomRecord().type != GCM_NULL_GTYPE;
                    pAction->setEnabled(bEnabledAxis);
                    break;
                */
            }
        }

        if (pAction->isCheckable())
        {
            if (value == static_cast<Commands>(m_pCurrentProcess->GetCustomKey().GetKey()))
                pAction->setChecked(true);
            else
                pAction->setChecked(false);
        }
    }
}


//---------------------------------------------------------------------------------------
//
// ---
void ExplodeWidget::viewCommands(Commands cmd)
{
    Commands value = Select;
    if (cmd == None)
    {
        QObject* action = sender();
        value = static_cast<Commands>(action->property("Commands").toInt());
    }

    switch (value)
    {
    case ExplodeWidget::Open:
    {
        viewCommands(ExplodeWidget::Select);
        createSceneZarubin();
        // loadModel();
        break;
    }
    case ExplodeWidget::Save:
    {
        viewCommands(ExplodeWidget::Select);
        // saveModel();
        break;
    }
    case ExplodeWidget::Select:
    {
        if (m_pCurrentProcess != nullptr)
            m_pCurrentProcess->CancelObject();
        VSN_DELETE_AND_NULL(m_pCurrentProcess);
        m_pCurrentProcess = new PrBase(this);
        m_pCurrentProcess->SetCustomKey(NodeKey(ExplodeWidget::Select));
        break;
    }
    /*
            case AssmSolverSceneWidget::Cancel:
            {
                if (m_pCurrentProcess != nullptr)
                    m_pCurrentProcess->CancelObject();
                viewCommands(AssmSolverSceneWidget::Select);
                break;
            }
    */
    /*case ExplodeWidget::Comfirm:
    {
        if (m_pCurrentProcess != nullptr)
            m_pCurrentProcess->CreateObject();
        VSN_DELETE_AND_NULL(m_pCurrentProcess);
        viewCommands(AssmSolverSceneWidget::Select);
        break;
    }
    case ExplodeWidget::InsertItem:
    {
        QString fileName = openSelectModel();
        if (!fileName.isEmpty())
        {
            VSN_DELETE_AND_NULL(m_pCurrentProcess);
            bool bRuning = false;
            m_pCurrentProcess = new PrInsertItem(c3d::WToPathstring(fileName.toStdWString()), this, bRuning);
            if (!bRuning)
                viewCommands(ExplodeWidget::Select);
        }
        break;
    }*/
    case ExplodeWidget::FixItem:
    {
        VSN_DELETE_AND_NULL(m_pCurrentProcess);
        m_pCurrentProcess = StartFixingProcess(this);
        if (m_pCurrentProcess == nullptr)
            viewCommands(ExplodeWidget::Select);
        break;
    }

    //// Commands to create assembly mates.
    case ExplodeWidget::Coincident:
    case ExplodeWidget::Coaxial:
    case ExplodeWidget::Parallel:
    case ExplodeWidget::Perpendicular:
    case ExplodeWidget::Angular:
    case ExplodeWidget::Distance:
    {
        if (m_pCurrentProcess != nullptr)
            m_pCurrentProcess->CancelObject();
        VSN_DELETE_AND_NULL(m_pCurrentProcess);
        bool bRuning = false;
        m_pCurrentProcess = new PrBinConstraint(value, this, bRuning);
        if (!bRuning)
            viewCommands(ExplodeWidget::Select);
        break;
    }
    case ExplodeWidget::Rotation:
    {
        Camera* camera = viewport()->GetCamera();
        camera->SetViewOrientation(Orientation::IsoXYZ);
        for (int i = 0; i < 360; ++i) {
            Sleep(1);
            camera->Orbit(M_PI / 180, 0);
            viewport()->RefreshScreen();
        }
        //TOZO: Решить поворот
        //Point3DF cameraPosition = camera->GetPosition();
        //const int x = cameraPosition.x;
        //const int y = cameraPosition.y;
        //const int z = cameraPosition.z;

        //for (int i = 0; i < 360; ++i) {
        //    Sleep(1);

        //    float angle = i * M_PI / 180;

        //    // float newX = z * cos(angle) - x * sin(angle);
        //    // float newY = z * sin(angle) + x * cos(angle);

        //    camera->SetPosition(Point3DF(newX, newY, 0)); // Set the new camera position
        //    viewport()->RefreshScreen();
        //}
    }
    default:
        break;
    };
    slotUpdateCommands();
    update();
}


//---------------------------------------------------------------------------------------
//
// ---
void ExplodeWidget::viewCommandsHeats(Exhanchares cmd)
{
    m_pModel.reset();
    m_pTreeWidget->clear();
    sceneContent()->Clear();
    Exhanchares value = NoneOfOne;

    QObject* action = sender();
    int currentIndexofExchanger = action ? action->property("CommandsHeatExhanger").toInt() : static_cast<int>(cmd);
    const bool hasChangeType = m_pCurrentExchandger != currentIndexofExchanger;

    QFormLayout* warmForm = m_pExplodeManager->m_vLayoutWarmParams->
        findChild<QFormLayout*>("warmForm");

    if (hasChangeType) 
    {
        m_pExplodeManager->m_comboConfigure->clear();
        m_pExplodeManager->m_quantityCombobox->setCurrentIndex(0);

        if (warmForm)
        {
            m_pExplodeManager->m_vLayoutWarmParams->removeItem(warmForm);

            QLayoutItem* child;
            while ((child = warmForm->takeAt(0)) != nullptr) {
                if (child->widget()) {
                    delete child->widget();
                }

                delete child;
            }
           
            delete warmForm;
        }

        m_pExplodeManager->createCalculationTab(currentIndexofExchanger);
        m_pExplodeManager->createParametrizationForm(currentIndexofExchanger, m_pExplodeManager->m_vLayoutWarmParams);
    }

    m_pExplodeManager->m_quantityCombobox->setHidden(!(currentIndexofExchanger == 1 || currentIndexofExchanger == 2));
    
    m_pCurrentExchandger = currentIndexofExchanger;
    value = static_cast<Exhanchares>(currentIndexofExchanger);

    QStringList values;

    switch (value)
    {
        case ExplodeWidget::TTOR:
        {
            int index = m_pExplodeManager->m_comboConfigure->currentIndex();
            m_pExplodeManager->m_reconfigureButton->setProperty("CommandsHeatExhanger", QVariant((int)ExplodeWidget::TTOR));

            for (const auto& config : m_pExplodeManager->dataTTOR) {
                values.append(QString::fromStdString(config.name));
            }

            BuildParamsForHeatExchangerTTOR config = m_pExplodeManager->dataTTOR[index > 0 ? index : 0];
            int configurationQuantity = m_pExplodeManager->m_quantityCombobox->currentIndex();
    
            m_pModel = ParametricModelCreator::CreatePneymocylinderModelZarubin(config, configurationQuantity);
            openModel();
            break;
        }
        case ExplodeWidget::TTRM:
        {
            int index = m_pExplodeManager->m_comboConfigure->currentIndex();
            m_pExplodeManager->m_reconfigureButton->setProperty("CommandsHeatExhanger", QVariant((int)ExplodeWidget::TTRM));

            for (const auto& config : m_pExplodeManager->dataTTRM) {
                values.append(QString::fromStdString(config.name));
            }

            ConfigParams config = m_pExplodeManager->isCheckedManualType 
                ? m_pExplodeManager->manualTTRMParams 
                : m_pExplodeManager->dataTTRM[index > 0 ? index : 0];

            int configurationQuantity = m_pExplodeManager->m_quantityCombobox->currentIndex();

            m_pModel = ParametricModelCreator::CreatePneymocylinderModelTTRM(config, configurationQuantity);
            openModel();
            break;
        }
        case ExplodeWidget::IP: 
        {
            int index = m_pExplodeManager->m_comboConfigure->currentIndex();
            m_pExplodeManager->m_reconfigureButton->setProperty("CommandsHeatExhanger", QVariant((int)ExplodeWidget::IP));

            for (const auto& config : m_pExplodeManager->dataIP) {
                values.append(QString::fromStdString(config.name));
            }

            ConfigParams_IP config = m_pExplodeManager->isCheckedManualType
                ? m_pExplodeManager->manualIPParams
                : m_pExplodeManager->dataIP[index > 0 ? index : 0];

            m_pModel = ParametricModelCreator::CreatePneymocylinderModelFukina(config);
            openModel();
            break;
        }
        case ExplodeWidget::IU: 
        {
            int index = m_pExplodeManager->m_comboConfigure->currentIndex();
            m_pExplodeManager->m_reconfigureButton->setProperty("CommandsHeatExhanger", QVariant((int)ExplodeWidget::IU));

            for (const auto& config : m_pExplodeManager->dataIU) {
                values.append(QString::fromStdString(config.name));
            }

            ConfigParams_IU config = m_pExplodeManager->isCheckedManualType
                ? m_pExplodeManager->manualIUParams
                : m_pExplodeManager->dataIU[index > 0 ? index : 0];

            m_pModel = ParametricModelCreator::CreatePneymocylinderModelVasinkina(config);
            openModel();
            break;
        }
        default:
            break;
    };

    if (hasChangeType)
    {
        m_pExplodeManager->m_comboConfigure->addItems(values);
        m_pExplodeManager->m_vLayoutWarmParams->update();
    }

    update();
}

//---------------------------------------------------------------------------------------
// Get a single assembly node.
// ---
SceneSegment* ExplodeWidget::assemblySegment() const
{
    if (m_pModelSeg == nullptr)
    {
        return nullptr;
    }

    if (const MbAssembly* rootAssm = GetMainAssembly(m_pModel))
    {
        auto childList = m_pModelSeg->GetChildSegments();
        // Single assembly node is required.
        if (childList.size() == 1)
        {
            // Check the assembly segment must have property C3D_PATHITEM_ID.
            if (childList.front()->GetProperty(C3D_PATHITEM_ID) == rootAssm->GetItemName())
                return childList.front();
        }
    }

    return nullptr;
}

//-----------------------------------------------------------------------------
//
// ---
void ExplodeWidget::openModel()
{
    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);

    const QString lastUserPath;
    QStringList filters = QtVision::openSaveFilters();
    QString oneLineFilters = filters.join("\n");

    createScene();
//#ifdef Q_OS_WIN
//    const QStringList files = m_modelPath.empty()
//        ? QFileDialog::getOpenFileNames(this, tr("Select Models"), lastUserPath, oneLineFilters)
//        : QStringList(m_modelPath.c_str());
//#else 
//    const QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Models"), lastUserPath, oneLineFilters, nullptr, QFileDialog::DontUseNativeDialog);
//#endif
//    if (files.count() > 0)
//    {
//        if (files.count() > 1)
//            emit setVisibleProgress(true);
//        if (m_pSegmModel != nullptr)
//        {
//            pTopSegment->RemoveSegment(m_pSegmModel);
//            VSN_DELETE_AND_NULL(m_pSegmModel);
//            m_pTreeWidget->clear();
//        }
//
//        if (m_pModel != nullptr)
//            m_pModel.reset();
//    }
//    else
//        return;
//
//    if (m_pModel != nullptr)
//        Q_ASSERT(false);
//
//    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//    loadFiles(files);
//    if (m_pModel->ItemsCount() == 0)
//    {
//        m_pModel.reset();
//        exit(1);
//    }
//    createScene();
    fillGeometryList();
}

//-----------------------------------------------------------------------------
//
// ---
void ExplodeWidget::showContextMenu(const QPoint& pos)
{
    QMenu menu;
    QAction* hideAction = menu.addAction("Hide");
    hideAction->setEnabled(m_ptrSelectManager->GetSelectionList().size() > 0);
    QAction* showOnlyAction = menu.addAction("Show only");
    QAction* showAllAction = menu.addAction("Show all");
    menu.addSeparator();
    QAction* clearSelectionAction = menu.addAction("Clear selection");
    QAction* selectedAction = menu.exec(pos);

    if (selectedAction == hideAction)
        hideSelectedObjects();
    else if (selectedAction == showOnlyAction)
        showAllObjects();
    else if (selectedAction == clearSelectionAction)
        clearSelectionObjects();
}

//-----------------------------------------------------------------------------
//
// ---
ExplodeTreeView* ExplodeWidget::createGeometryList(QWidget* pParent)
{
    m_pTreeWidget = new ExplodeTreeView(this, pParent);
    QObject::connect(m_pTreeWidget, &QTreeWidget::currentItemChanged, this, &ExplodeWidget::currentItemChanged);
    QObject::connect(m_pTreeWidget, &QTreeWidget::itemChanged, this, &ExplodeWidget::itemChanged);
    return m_pTreeWidget;
}

//-----------------------------------------------------------------------------
//
// ---
void ExplodeWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    Q_UNUSED(previous);
    if (m_pSegmModel == nullptr)
        return;
    m_ptrSelectManager->UnselectAll();
    if (TreeObjectItem* pWidgetItem = dynamic_cast<TreeObjectItem*>(current))
    {
        SceneSegment* pCurrentSegment = pWidgetItem->object();
        if (m_pExplodeManager->isSelectionEnabled())
        {
            if (!m_pExplodeManager->onSelectItem(pCurrentSegment))
            {
                m_ptrSelectManager->UnselectAll();
                return;
            }
            update();
        }
        if (pCurrentSegment->GetRepresentation() == nullptr)
            _selectSegment(m_ptrSelectManager, pCurrentSegment);
        else
            m_ptrSelectManager->Select(pCurrentSegment->GetUniqueKey());
    }
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeWidget::itemChanged(QTreeWidgetItem* item)
{
    if (TreeObjectItem* pWidgetItem = dynamic_cast<TreeObjectItem*>(item))
    {
        SceneSegment* pCurrentSegment = pWidgetItem->object();
        pCurrentSegment->SetVisible(item->checkState(0) == Qt::Checked);
        update();
    }
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeWidget::contextMenuEvent(QContextMenuEvent* event)
{
//    showContextMenu(event->globalPos());
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeWidget::loadFiles(const QStringList& files)
{
    m_pModel = new MbModel();
    emit setProgressRange(0, files.count());
    for (int i = 0, count = files.count(); i < count; ++i)
    {
        QString fileName = files.at(i);
        if (fileName.isEmpty())
            continue;

        MbModel model;
        ConvConvertorProperty3D convProperties;
        convProperties.enableAutostitch = true;
        convProperties.autostitchPrecision = Math::visualSag;
        convProperties.fileName = c3d::WToPathstring(fileName.toStdWString());
        if (c3d::ImportFromFile(model, c3d::WToPathstring(fileName.toStdWString()), &convProperties, nullptr) == cnv_Success)
        {
            for (MbModel::ItemIterator iter = model.Begin(), last = model.End(); iter != last; ++iter)
                m_pModel->AddItem(*(*iter));
        }
        emit buildProgress(i);
    }
    emit resetProgress();
    emit setVisibleProgress(false);
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeWidget::createScene()
{
    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);
    ProgressBuild* pProgressBuild = m_pSceneGenerator->CreateProgressBuild();
    Object::Connect(pProgressBuild, &ProgressBuild::BuildAllCompleted, this, &ExplodeWidget::slotFinishBuildRep);
    m_pSegmModel = m_pSceneGenerator->CreateSceneSegment(m_pModel, pTopSegment, false);
    m_pSceneGenerator->StartBuildGeometry();
}

void ExplodeWidget::createSceneZarubin()
{
    //m_pModel.reset();
    //m_pTreeWidget->clear();
    //sceneContent()->Clear();

    //// TOZO: Завести переменную для определения сборки и добавить кнопки в main toolbar left srea
    //// TOZO: Не работает дерево, хз почему
    //m_pModel = ParametricModelCreator::CreatePneymocylinderModelZarubin(BuildParamsForHeatExchangerTTOR());

    //SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    //Q_ASSERT(pTopSegment != nullptr);
    //ProgressBuild* pProgressBuild = m_pSceneGenerator->CreateProgressBuild();
    //Object::Connect(pProgressBuild, &ProgressBuild::BuildAllCompleted, this, &ExplodeWidget::slotFinishBuildRep);
    //m_pSegmModel = m_pSceneGenerator->CreateSceneSegment(m_pModel, pTopSegment, false);
    //m_pSceneGenerator->StartBuildGeometry();
}

void ExplodeWidget::createSceneIP()
{
    m_pModel.reset();
    m_pTreeWidget->clear();
    sceneContent()->Clear();

    // TOZO: Завести переменную для определения сборки и добавить кнопки в main toolbar left srea
    // TOZO: Не работает дерево, хз почему
    m_pModel = ParametricModelCreator::CreatePneymocylinderModelFukina(ConfigParams_IP());

    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);
    ProgressBuild* pProgressBuild = m_pSceneGenerator->CreateProgressBuild();
    Object::Connect(pProgressBuild, &ProgressBuild::BuildAllCompleted, this, &ExplodeWidget::slotFinishBuildRep);
    m_pSegmModel = m_pSceneGenerator->CreateSceneSegment(m_pModel, pTopSegment, false);
    m_pSceneGenerator->StartBuildGeometry();
}

void ExplodeWidget::createSceneIU()
{
    m_pModel.reset();
    m_pTreeWidget->clear();
    sceneContent()->Clear();

    // TOZO: Завести переменную для определения сборки и добавить кнопки в main toolbar left srea
    // TOZO: Не работает дерево, хз почему
    m_pModel = ParametricModelCreator::CreatePneymocylinderModelVasinkina(ConfigParams_IU());

    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);
    ProgressBuild* pProgressBuild = m_pSceneGenerator->CreateProgressBuild();
    Object::Connect(pProgressBuild, &ProgressBuild::BuildAllCompleted, this, &ExplodeWidget::slotFinishBuildRep);
    m_pSegmModel = m_pSceneGenerator->CreateSceneSegment(m_pModel, pTopSegment, false);
    m_pSceneGenerator->StartBuildGeometry();
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeWidget::fillGeometryList()
{
    m_pTreeWidget->setCurrentItem(nullptr);
    m_pTreeWidget->clear();
    if (m_pSegmModel != nullptr)
        fillSegmentList(m_pTreeWidget, m_pSegmModel, nullptr);
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeWidget::hideSelectedObjects()
{
    auto lst = m_ptrSelectManager->GetSelectionList();
    for (SelectionItem* item : lst)
    {
        InstSelectionItem* pItem = static_cast<InstSelectionItem*>(item);
        TreeObjectItem* treeItem = m_pTreeWidget->findItemByObject(sceneContent()->GetSegment(pItem->GetNodeKey()));
        treeItem->setCheckState(0, Qt::Unchecked);
    }
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeWidget::showAllObjects()
{
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeWidget::clearSelectionObjects()
{
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeWidget::slotFinishBuildRep()
{
    QApplication::restoreOverrideCursor();
    viewport()->ZoomToFit(sceneContent()->GetBoundingBox());
    m_pExplodeManager->init(m_pSegmModel, m_pTreeWidget);
    update();
}

//-----------------------------------------------------------------------------
//
// ---
void ExplodeWidget::setRenderingMode()
{
    if (QToolButton* pToolButton = qobject_cast<QToolButton*>(sender()))
    {
        if (pToolButton->objectName() == QLatin1String("Perspective"))
            viewport()->SetOrthographicProjection(!pToolButton->isChecked());
    }
    update();
}

//-----------------------------------------------------------------------------
//
// ---
void ExplodeWidget::slotItemSelectModified()
{
}

//-----------------------------------------------------------------------------
//
// ---
void ExplodeWidget::slotCurrentItemsModified(std::list<SelectionItem*>& oldItems, std::list<SelectionItem*>& newItems)
{
    std::vector<const SceneSegment*> oldSegments;
    if (oldItems.size() == newItems.size())
    {
        for (auto pSelectionItem : oldItems)
        {
            InstSelectionItem* pItem = static_cast<InstSelectionItem*>(pSelectionItem);
            const SceneSegment* pSegm = pItem->GetSceneSegment();
            oldSegments.push_back(pSegm);
        }
    }
    VSN_UNUSED(oldItems);
    if (!newItems.empty())
    {
        for (auto pSelectionItem : newItems)
        {
            InstSelectionItem* pItem = static_cast<InstSelectionItem*>(pSelectionItem);
            const SceneSegment* pSegm = pItem->GetSceneSegment();
            TreeObjectItem* treeItem = m_pTreeWidget->findItemByObject(pSegm);

            const bool isNewItem = std::find(oldSegments.cbegin(), oldSegments.cend(), pSegm) == oldSegments.cend();
            if (isNewItem)
            {
                if (pSegm->HasRep())
                    m_pExplodeManager->onSelectItem(pSegm);
            }
            bool blockSig = m_pTreeWidget->blockSignals(true);
            m_pTreeWidget->setCurrentItem(treeItem);
            m_pTreeWidget->blockSignals(blockSig);
        }
    }
    else
    {
        m_pTreeWidget->setCurrentItem(nullptr);
        m_pExplodeManager->onSelectItem(nullptr);
    }
    update();
}

////---------------------------------------------------------------------------
////
//// ---
void ExplodeWidget::slotFilterTriggered(QAction* action)
{
    Filter filer = m_ptrSelectManager->GetFilterObject();
    if (action->objectName() == QString("ID_:/res/filter24x24.png"))
    {
        if (action->isChecked())
            filer = SubPrim;
    }
    else if (action->objectName() == QString("ID_:/res/filterseg24x24.png"))
    {
    }
    else if (action->objectName() == QString("ID_:/res/filterbody24x24.png"))
        m_ptrSelectManager->SetBodySelectionEnabled(action->isChecked());
    else if (action->objectName() == QString("ID_:/res/filterface24x24.png"))
        m_ptrSelectManager->SetFaceSelectionEnabled(action->isChecked());
    else if (action->objectName() == QString("ID_:/res/filteredge24x24.png"))
        m_ptrSelectManager->SetEdgeSelectionEnabled(action->isChecked());
    else if (action->objectName() == QString("ID_:/res/filtervertex24x24.png"))
        m_ptrSelectManager->SetVertexSelectionEnabled(action->isChecked());
    updateActionCheckFilter();
}

//---------------------------------------------------------------------------
//
// ---
void ExplodeWidget::updateActionCheckFilter()
{
    Filter filer = m_ptrSelectManager->GetFilterObject();
    auto lstActions = m_pGroupFilter->actions();
    for (auto it = lstActions.begin(); it != lstActions.end(); ++it)
    {
        QAction* action = (*it);
        if (action->objectName() == QString("ID_:/res/filter24x24.png"))
            action->setChecked(filer.CheckFlag(SubPrim));
        //        else if (action->objectName() == QString("ID_:/res/filterseg24x24.png"))
        //            action->setChecked(filer.checkFlag(SubSegment));
        else if (action->objectName() == QString("ID_:/res/filterbody24x24.png"))
            action->setChecked(filer.CheckFlag(SubBody));
        else if (action->objectName() == QString("ID_:/res/filterface24x24.png"))
            action->setChecked(filer.CheckFlag(SubFace));
        else if (action->objectName() == QString("ID_:/res/filteredge24x24.png"))
            action->setChecked(filer.CheckFlag(SubEdge));
        else if (action->objectName() == QString("ID_:/res/filtervertex24x24.png"))
            action->setChecked(filer.CheckFlag(SubVertex));
    }
}
////---------------------------------------------------------------------------
//// Set a different render mode
//// ---------------------------------------------------------------------------------
void ExplodeWidget::slotRenderTriggered(QAction* action)
{
    if (action->objectName() == QString("ID_:/res/renderMods/dimmedWireframe.png")){
        graphicsView()->SetRenderMode(RenderMode::rm_ShadedWithEdges);
    }else if (action->objectName() == QString("ID_:/res/renderMods/shaded.png")) {
        graphicsView()->SetRenderMode(RenderMode::rm_Shaded);
    }else if (action->objectName() == QString("ID_:/res/renderMods/wirefrm.png")) {
        graphicsView()->SetRenderMode(RenderMode::rm_Wireframe);
    }else if (action->objectName() == QString("ID_:/res/renderMods/hiddenremoved.png")) {
        graphicsView()->SetRenderMode(RenderMode::rm_HiddenLinesRemoved);
    }
    update();
}


//---------------------------------------------------------------------------
//
// ---
QColor ExplodeWidget::highlightColor() const
{
    Color clr = m_ptrSelectManager->GetHighlightColor();
    return QColor(clr.GetRed(), clr.GetGreen(), clr.GetBlue());
}

//---------------------------------------------------------------------------
//
// ---
QColor ExplodeWidget::selectionColor() const
{
    Color clr = m_ptrSelectManager->GetSelectionColor();
    return QColor(clr.GetRed(), clr.GetGreen(), clr.GetBlue());
}

//---------------------------------------------------------------------------
//
// ---
void ExplodeWidget::slotHighlightColor(const QColor& clr)
{
    m_ptrSelectManager->SetHighlightColor(Color(clr.red(), clr.green(), clr.blue()));
}

//---------------------------------------------------------------------------
//
// ---
void ExplodeWidget::slotSelectionColor(const QColor& clr)
{
    m_ptrSelectManager->SetSelectionColor(Color(clr.red(), clr.green(), clr.blue()));
}

//---------------------------------------------------------------------------
//  
// ---
void ExplodeWidget::slotDynamicHighlighting(int state)
{
    m_ptrSelectManager->SetDynamicHighlighting(state == Qt::Checked);
    update();
}

//---------------------------------------------------------------------------
//  Make hidden/show QGroupBox'es
// ---
void ExplodeWidget::slotToggleVisibility(bool checked, QGroupBox* groupBox) {
    // Check if the checkbox is checked
    if (groupBox) {
        groupBox->setVisible(checked); // Set visibility based on the checkbox state
    }
}


//---------------------------------------------------------------------------
//  Change cursor depends on selector
// ---
void ExplodeWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_startBuildDimension && m_dimensionRep)
    {
        if (LinearDimensionRep* linearDimensionRep = vobject_cast<LinearDimensionRep*>(m_dimensionRep))
        {
            MbPlacement3D initPlane;
            MbPlane pl(initPlane); // plane X, Y
            *m_point2 = mapPointOnPlane(event->pos().x(), event->pos().y(), pl);
            linearDimensionRep->SetMeasuredGeometry(*m_point1, *m_point2, pl);
            update();
        }
        else if (DiameterDimensionRep* diameterDimensionRep = vobject_cast<DiameterDimensionRep*>(m_dimensionRep))
        {
            MbPlacement3D initPlane(*m_point1);
            MbPlane pl(initPlane); // plane X, Y
            *m_point2 = mapPointOnPlane(event->pos().x(), event->pos().y(), pl);
            double rad = m_point1->DistanceToPoint(*m_point2);
            MbArc3D arc(initPlane, rad, rad, M_PI2);
            diameterDimensionRep->SetMeasuredGeometry(arc);
            update();
        }
        else if (RadialDimensionRep* radialDimensionRep = vobject_cast<RadialDimensionRep*>(m_dimensionRep))
        {
            MbPlacement3D initPlane(*m_point1);
            MbPlane pl(initPlane); // plane X, Y
            *m_point2 = mapPointOnPlane(event->pos().x(), event->pos().y(), pl);
            double rad = m_point1->DistanceToPoint(*m_point2);
            MbArc3D arc(initPlane, rad, rad, M_PI2);
            radialDimensionRep->SetMeasuredGeometry(arc);
            update();
        }
        else if (AngleDimensionRep* angleDimensionRep = vobject_cast<AngleDimensionRep*>(m_dimensionRep))
        {
            if (!m_point3)
                return;
            MbPlacement3D initPlane;
            MbPlane pl(initPlane); // plane X, Y
            *m_point3 = mapPointOnPlane(event->pos().x(), event->pos().y(), pl);
            angleDimensionRep->SetMeasuredGeometry(*m_point1, *m_point2, *m_point3);
            update();
        }
    }
    else {
        QtVision::QtOpenGLSceneWidget::mouseMoveEvent(event);

        ObjectType type = ObjectType::None;
        if (SelectionItem* item = m_ptrSelectManager->GetHighlightItem())
            type = item->GetType();

        QCursor curCursor;
        switch (type)
        {
        case ObjectType::Vertex: curCursor = m_curVertex; break;
        case ObjectType::Edge:   curCursor = m_curEdge;   break;
        case ObjectType::Face:   curCursor = m_curFace;   break;
        default: curCursor = m_curPoint; break;
        }
        setCursor(curCursor);
    }
}


void ExplodeWidget::mousePressEvent(QMouseEvent* event)
{
    QtOpenGLSceneWidget::mousePressEvent(event);
    if (event->button() != Qt::LeftButton)
        return;

    if (!m_startBuildDimension && vobject_cast<LinearDimensionRep*>(m_dimensionRep))
    {
        MbPlacement3D initPlane;
        MbPlane pl(initPlane); // plane X, Y
        m_point1 = new MbCartPoint3D(mapPointOnPlane(event->pos().x(), event->pos().y(), pl));
        m_point2 = new MbCartPoint3D(*m_point1);
        m_startBuildDimension = true;
    }
    else if (!m_startBuildDimension && vobject_cast<DiameterDimensionRep*>(m_dimensionRep))
    {
        MbPlacement3D initPlane;
        MbPlane pl(initPlane); // plane X, Y
        m_point1 = new MbCartPoint3D(mapPointOnPlane(event->pos().x(), event->pos().y(), pl));
        m_point2 = new MbCartPoint3D(*m_point1);
        m_startBuildDimension = true;
    }
    else if (!m_startBuildDimension && vobject_cast<RadialDimensionRep*>(m_dimensionRep))
    {
        MbPlacement3D initPlane;
        MbPlane pl(initPlane); // plane X, Y
        m_point1 = new MbCartPoint3D(mapPointOnPlane(event->pos().x(), event->pos().y(), pl));
        m_point2 = new MbCartPoint3D(*m_point1);
        m_startBuildDimension = true;
    }
    else if (!m_startBuildDimension && vobject_cast<AngleDimensionRep*>(m_dimensionRep))
    {
        MbPlacement3D initPlane;
        MbPlane pl(initPlane); // plane X, Y
        m_point1 = new MbCartPoint3D(mapPointOnPlane(event->pos().x(), event->pos().y(), pl));
        m_startBuildDimension = true;
    }
    else if (m_startBuildDimension && !m_point2 && vobject_cast<AngleDimensionRep*>(m_dimensionRep))
    {
        MbPlacement3D initPlane;
        MbPlane pl(initPlane); // plane X, Y
        m_point2 = new MbCartPoint3D(mapPointOnPlane(event->pos().x(), event->pos().y(), pl));
        m_point3 = new MbCartPoint3D(*m_point2);
    }
    else if (m_startBuildDimension)
    {
        VSN_DELETE_AND_NULL(m_point1);
        VSN_DELETE_AND_NULL(m_point2);
        VSN_DELETE_AND_NULL(m_point3);
        m_startBuildDimension = false;
        m_dimensionRep = nullptr;
        m_state = ExplodeWidget::Selection;
        updateButtons();
        clearProperties();
    }
}
//---------------------------------------------------------------------------------------
//
// ---
QString ExplodeWidget::openSelectModel()
{
    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);

    const QString lastUserPath;
    QStringList filters;
    filters.append("Files *.c3d");
    filters.append("File C3D (*.c3d)");
    QString oneLineFilters = filters.join("\n");
#ifdef Q_OS_WIN
    const QString file = QFileDialog::getOpenFileName(this, tr("Select Models"), lastUserPath, oneLineFilters);
#else 
    const QString file = QFileDialog::getOpenFileName(this, tr("Select Models"), lastUserPath, oneLineFilters, nullptr, QFileDialog::DontUseNativeDialog);
#endif 
    return file;
}

//---------------------------------------------------------------------------------------
//
// ---
void ExplodeWidget::loadModel()
{
    SceneSegment* pTopSegment = sceneContent()->GetRootSegment();
    Q_ASSERT(pTopSegment != nullptr);

    const QString lastUserPath;
    QStringList filters = QtVision::openSaveFilters();
    QString oneLineFilters = filters.join("\n");

    const QStringList files = m_modelPath.empty()
        ? QFileDialog::getOpenFileNames(this, tr("Select Models"), lastUserPath, oneLineFilters)
        : QStringList(m_modelPath.c_str());

    if (files.count() > 0)
    {
        if (files.count() > 1)
            emit setVisibleProgress(true);
        if (m_pSegmModel != nullptr)
        {
            pTopSegment->RemoveSegment(m_pSegmModel);
            VSN_DELETE_AND_NULL(m_pSegmModel);
            m_pTreeWidget->clear();
        }

        if (m_pModel != nullptr)
            m_pModel.reset();
    }
    else
        return;

    if (m_pModel != nullptr)
        Q_ASSERT(false);

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    loadFiles(files);
    if (m_pModel->ItemsCount() == 0)
    {
        m_pModel.reset();
        exit(1);
    }
    createScene();
    fillGeometryList();
}


//---------------------------------------------------------------------------------------
//
// ---
void ExplodeWidget::saveModel()
{
    if (m_pModel == nullptr)
        return;
    const QString lastUserPath;
    QStringList filters = QtVision::openSaveFilters();
    QString oneLineFilters = filters.join("\n");
#ifdef Q_OS_WIN
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save Model"), lastUserPath, oneLineFilters);
#else 
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save Model"), lastUserPath, oneLineFilters, nullptr, QFileDialog::DontUseNativeDialog);
#endif 
    if (!fileName.isEmpty())
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        MbeConvResType readRes = cnv_Success;
        readRes = c3d::ExportIntoFile(*m_pModel, c3d::WToPathstring(fileName.toStdWString()));
        if (readRes != cnv_Success)
            vsnWarning("Model write error");
        QApplication::restoreOverrideCursor();
    }
}

void ExplodeWidget::togglePlaneCuttingVisibility(bool checked, QGroupBox* groupBox) {
    
    auto tool = graphicsScene()->GetCuttingTool();
    if (checked) {
        m_curIdPlane = tool->AddSectionPlane(MbPlacement3D(MbCartPoint3D(-30, -30, -30), MbVector3D(0, -1, 0)));
        tool->EnableInteractiveMode(m_curIdPlane, true);
        tool->SetEnable(m_curIdPlane, true);
        m_controller = new Controller(graphicsScene());
        m_controller->SetSectionPlaneID(m_curIdPlane);
        m_controller->SetControllerView(defaultView);
        m_controller->Show();
    }
    else
    {
        tool->DeleteSectionPlane(m_curIdPlane);
        m_controller->Hide();
    }

    if (groupBox) {
        groupBox->setVisible(checked);
    }

    update();
    Object::Connect(viewport(), &Viewport::RefreshScreen, this, &QtOpenGLWidget::repaintWidget);
}

//-----------------------------------------------------------------------------
// 
// ---
void ExplodeWidget::enableChanged(int enable)
{
    graphicsScene()->GetCuttingTool()->SetEnable(m_curIdPlane, !!enable);
    update();
}

void ExplodeWidget::closeChanged(int close)
{
    graphicsScene()->GetCuttingTool()->CloseSectionPlane(m_curIdPlane, !!close);
    update();
}

void ExplodeWidget::invertChanged(int invert)
{
    graphicsScene()->GetCuttingTool()->Invert(m_curIdPlane, !!invert);
    m_controller->UpdateControllerWidget();
    update();
}

void ExplodeWidget::freeSectionChanged(bool state)
{
    if (state)
    {
        defaultView = Controller::xyzCtrl;
        m_controller->SetControllerView(defaultView);
    }
}

void ExplodeWidget::offsetSectionChanged(bool state)
{
    if (state)
    {
        defaultView = Controller::directCtrl;
        m_controller->SetControllerView(defaultView);
    }
}

void ExplodeWidget::controllerChanged(int state)
{
    const bool checkOn = !!state;
    QDoubleSpinBox* tst1 = m_pExplodeManager->getM_Poffset();
    QDoubleSpinBox* tst2 = m_pExplodeManager->getM_Pa1();
    QDoubleSpinBox* tst3 = m_pExplodeManager->getM_Pa2();
    tst1->setEnabled(!checkOn);
    tst2->setEnabled(!checkOn);
    tst3->setEnabled(!checkOn);
    m_pExplodeManager->getM_PfreeSection()->setEnabled(checkOn);
    m_pExplodeManager->getM_PoffsetSection()->setEnabled(checkOn);

        auto tool = graphicsScene()->GetCuttingTool();
    if (!!state) {
        m_controller->SetControllerView(defaultView);
    }
    else
    {
        m_controller->SetControllerView(Controller::NoneCtrl);
        getParameters();
    }
}

void ExplodeWidget::getParameters()
{
    auto tool = graphicsScene()->GetCuttingTool();
    tool->GetParams(m_curIdPlane, m_angle1, m_angle2, m_offset);

    QDoubleSpinBox* tst1 = m_pExplodeManager->getM_Poffset();
    QDoubleSpinBox* tst2 = m_pExplodeManager->getM_Pa1();
    QDoubleSpinBox* tst3 = m_pExplodeManager->getM_Pa2();

    tst1->setValue(m_offset);
    tst2->setValue(m_angle1);
    tst3->setValue(m_angle2);
}

void ExplodeWidget::updateState()
{
    MbVector3D normal(1.0, m_angle1 * M_DEGRAD, m_angle2 * M_DEGRAD);
    SphericalToCartesian(normal.x, normal.y, normal.z);

    MbPlacement3D place(MbCartPoint3D(), normal);
    place.Move(place.GetAxisZ() * m_offset);

    graphicsScene()->GetCuttingTool()->Init(m_curIdPlane, place);

    update();
}

void ExplodeWidget::offsetChanged(double val)
{
    m_offset = val;
    updateState();
}

void ExplodeWidget::angleA1Changed(double val)
{
    m_angle1 = val;
    updateState();
}

void ExplodeWidget::angleA2Changed(double val)
{
    m_angle2 = val;
    updateState();
}

/*ColorButton*/
ColorButton::ColorButton(QWidget* parent)
    : QPushButton(parent)
{
    setAutoFillBackground(true);
    QFontMetrics fm(font());
    QRect rc = fm.boundingRect("XXXXXX");
    QSize sz(150, rc.height() + 4);
    setIconSize(sz);
    QObject::connect(this, SIGNAL(released()), this, SLOT(chooseColor()));
}

void ColorButton::chooseColor()
{
    QColor newCol = QColorDialog::getColor(m_color, this);
    if (newCol.isValid())
        setColor(newCol);
}

void ColorButton::setColor(const QColor& clr)
{
    if (m_color != clr)
    {
        m_color = clr;

        QFontMetrics fm(font());
        QRect rc = fm.boundingRect(text());

        QSize sz = iconSize();
        QPixmap px(sz);
        QPainter p(&px);
        p.fillRect(QRect(QPoint(0, 0), sz), m_color);
        QIcon icon; icon.addPixmap(px);
        setIcon(icon);
        emit colorChanged(m_color);
    }
}

void ExplodeWidget::animationSwitchSlot()
{
    if (timer->isActive())
    {
        timer->stop();
    }
    else
    {
        timer->start(100);
    }
}

void ExplodeWidget::animation()
{
    MbCartPoint3D rotationCenter;
    double sign = viewport()->GetCamera()->GetDefaultUpVector() * viewport()->GetCamera()->GetUpVector();
    sign = sign <= 0.0 ? 1.0 : sign / fabs(sign);
    viewport()->GetCamera()->RotateAbout(viewport()->GetCamera()->GetDefaultUpVector(), sign * 0.06, rotationCenter);
    update();
}

void ExplodeWidget::animationSlot()
{
    animation();
}

void ExplodeWidget::toggleCuttingState()
{
    auto tool = graphicsScene()->GetCuttingTool();

    if (tool->IsEnabled(m_curIdPlane))
    {
        tool->SetEnable(m_curIdPlane, false);
        m_controller->Hide();
    }
    else
    {
        tool->SetEnable(m_curIdPlane, true);
        m_controller->Show();
    }

    // TODO: Доделать реализацию 
}


void ExplodeWidget::saveImage()
{
    const QString lastUserPath;
    QString filters = "PNG (*.png)";
#ifdef Q_OS_WIN
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), lastUserPath, filters);
#else 
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), lastUserPath, filters, nullptr, QFileDialog::DontUseNativeDialog);
#endif 
    if (!fileName.isEmpty())
    {
        Color colorBack(255, 255, 255);
        SnapshotSettings settings;
        settings.hideSceneWidgets = false;
        settings.hideViewWidgets = false;
        settings.background = new VSN::SolidBrush(colorBack);
        QImage image = grabFrameImage(graphicsView(), settings);
        image.save(fileName);
        delete  settings.background;
    }
}

// Класс Properties для окна размеров
/* Properties */
Properties::Properties(const QString& strTitle, DimensionRep* dimensionRep, QWidget* parent)
    : QGroupBox(parent)
    , m_dimensionRep(dimensionRep)
{
    setTitle(strTitle);
    QFormLayout* vGroupLayout = new QFormLayout(this);
    m_cbTextOrientation = new QComboBox(this);
    m_cbTextOrientation->addItem("Заморозить", QVariant(DimensionGeometry::to_Freeze));    //В плоскости размера. \en In size plane. \~
    m_cbTextOrientation->addItem("Скриншот", QVariant(DimensionGeometry::to_ScreenOnly));  //В плоскости экрана. \en In screen plane. \~
    m_cbTextOrientation->addItem("Читаемый", QVariant(DimensionGeometry::to_Readable));    //В плоскости размера с поворотом по вектору взгляда. \en In dimension plane with a turn by view vector. \~
    m_cbTextOrientation->setCurrentIndex(static_cast<int>(m_dimensionRep->GetTextOrientation()));
    connect(m_cbTextOrientation, SIGNAL(currentIndexChanged(int)), this, SLOT(textOrientationChanged(int)));

    m_cbTextVPosition = new QComboBox(this);
    m_cbTextVPosition->addItem("Над", QVariant(DimensionGeometry::vpos_Above));
    m_cbTextVPosition->addItem("Снизу", QVariant(DimensionGeometry::vpos_Below));
    m_cbTextVPosition->addItem("По центру", QVariant(DimensionGeometry::vpos_Center));
    m_cbTextVPosition->setCurrentIndex(static_cast<int>(m_dimensionRep->GetTextVPosition()));
    connect(m_cbTextVPosition, SIGNAL(currentIndexChanged(int)), this, SLOT(vTextOrientationChanged(int)));

    m_cbTextHPosition = new QComboBox(this);
    m_cbTextHPosition->addItem("Слева", QVariant(DimensionGeometry::hpos_Left));
    m_cbTextHPosition->addItem("Справа", QVariant(DimensionGeometry::hpos_Right));
    m_cbTextHPosition->addItem("По центру", QVariant(DimensionGeometry::hpos_Center));
    m_cbTextHPosition->addItem("Авто", QVariant(DimensionGeometry::hpos_Auto));
    m_cbTextHPosition->setCurrentIndex(static_cast<int>(m_dimensionRep->GetTextHPosition()));
    connect(m_cbTextHPosition, SIGNAL(currentIndexChanged(int)), this, SLOT(hTextOrientationChanged(int)));

    vGroupLayout->addRow("Тип размера", m_cbTextOrientation);
    vGroupLayout->addRow("Позиционирование размера", m_cbTextVPosition);
    vGroupLayout->addRow("Расположение размера", m_cbTextHPosition);
}

// ---
Properties::~Properties()
{
}

// ---
void Properties::textOrientationChanged(int index)
{
    QVariant var = m_cbTextOrientation->itemData(index);
    m_dimensionRep->SetTextOrientation((DimensionGeometry::TextOrientation)var.toUInt());
    emit updateView();
}

// ---
void Properties::vTextOrientationChanged(int index)
{
    QVariant var = m_cbTextVPosition->itemData(index);
    m_dimensionRep->SetTextVPosition((DimensionGeometry::TextVPosition)var.toUInt());
    emit updateView();
}

// ---
void Properties::hTextOrientationChanged(int index)
{
    QVariant var = m_cbTextHPosition->itemData(index);
    m_dimensionRep->SetTextHPosition((DimensionGeometry::TextHPosition)var.toUInt());
    emit updateView();
}