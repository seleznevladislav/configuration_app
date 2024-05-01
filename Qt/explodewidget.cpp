#include <QFileDialog>
#include <QToolButton>
#include <QMenu>
#include <qevent.h>

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
//-----------------------------------------------------------------------------
// ---
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

//-----------------------------------------------------------------------------
// ---
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
    , m_curPoint(QtVision::findCursor(16)/*QCursor(QPixmap(":/res/point_cur.cur"))*/)
    , m_curEdge(QCursor(QPixmap(":/res/cursors/edge.cur")))
    , m_curFace(QCursor(QPixmap(":/res/cursors/face.cur")))
    , m_curVertex(QCursor(QPixmap(":/res/cursors/vertex.cur")))
{
    m_pModel = new MbModel;
    m_pSolver = std::make_shared<AssemblySolver>(UniteToMainAssembly(m_pModel));
    QtVision::ProcessTypes ptTypes = QtVision::pt_Pan | QtVision::pt_Zoom | QtVision::pt_Rotate;
    QtVision::createProcessesCameraControls(this, ptTypes);
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

    // Render Statistics
    setAboutSceneWidget(new QtVision::QtAboutSceneWidget(this));
    RenderStatistics::SetActivation(true);

    GlobalFlags::SetPixelCullingEnabled(true);

    GlobalFlags::SetFaceCulling(CullFaces::Back);
    viewport()->GetCamera()->SetViewOrientation(Orientation::IsoXYZ);
    // viewport()->SetGradientBackgroundColour(QtVision::topColor, QtVision::bottomColor);
    // Для Рисунков
    viewport()->SetGradientBackgroundColour(Color(255, 255, 255), Color(255, 255, 255));
    //viewport()->SetGradientBackgroundColour(Color(0, 255, 0), Color(0, 255, 0));
    viewport()->SetPixelCullingSize(35);
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
                pAction->setEnabled(assm != nullptr && assm->ItemsCount() > 0);
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
    //case ExplodeWidget::Rotation:
    //{
    //    if (m_pCurrentProcess != nullptr)
    //        m_pCurrentProcess->CancelObject();
    //    VSN_DELETE_AND_NULL(m_pCurrentProcess);
    //    bool bRuning = false;
    //    m_pCurrentProcess = new PrRotationAboutAxis(value, this, bRuning);
    //    if (!bRuning)
    //        viewCommands(AssmSolverSceneWidget::Select);
    //    break;
    //}
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
    int currentIndexofExchanger = action->property("CommandsHeatExhanger").toInt();

    if (m_pCurrentExchandger != currentIndexofExchanger) {
        m_pExplodeManager->m_comboConfigure->clear();
    }

    if (cmd == NoneOfOne) 
    {
        m_pCurrentExchandger = currentIndexofExchanger;
        value = static_cast<Exhanchares>(currentIndexofExchanger);
    }


   
    QStringList values;

    switch (value)
    {
    case ExplodeWidget::TTOR:
    {
        int index = m_pExplodeManager->m_comboConfigure->currentIndex();


        for (const auto& config : m_pExplodeManager->dataTTOR) {
            values.append(QString::fromStdString(config.name));
        }

        BuildParamsForHeatExchangerTTOR config = m_pExplodeManager->dataTTOR[index > 0 ? index : 0];

    
        m_pModel = ParametricModelCreator::CreatePneymocylinderModelZarubin(config);
        openModel();
        break;
    }case ExplodeWidget::TTRM:
    {
        int index = m_pExplodeManager->m_comboConfigure->currentIndex();

        for (const auto& config : m_pExplodeManager->configuration) {
            values.append(QString::fromStdString(config.name));
        }

        ConfigParams config = m_pExplodeManager->configuration[index > 0 ? index : 0];
        m_pModel = ParametricModelCreator::CreatePneymocylinderModel(config);
        openModel();
        break;
    }
    default:
        break;
    };

    m_pExplodeManager->m_comboConfigure->addItems(values);

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
    m_pModel.reset();
    m_pTreeWidget->clear();
    sceneContent()->Clear();

    // TOZO: Завести переменную для определения сборки и добавить кнопки в main toolbar left srea
    // TOZO: Не работает дерево, хз почему
    m_pModel = ParametricModelCreator::CreatePneymocylinderModelZarubin(BuildParamsForHeatExchangerTTOR());

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