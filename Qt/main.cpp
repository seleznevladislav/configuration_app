#include "explodewidget.h"
#include "explodemanager.h"

#include <vsn_application.h>
#include <QApplication>
#include <QSurfaceFormat>
#include <QFormLayout>
#include <QCommandLinkButton>
#include <QBoxLayout>
#include <QComboBox>
#include <QProgressBar>
#include <QGroupBox>
#include <QToolButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QDockWidget>
#include <QCheckBox>
#include <QRadioButton>
#include <QToolButton>
#include <QTabWidget>
#include <QMenu>
#include <QMenuBar>
#include <QWidgetAction>
#include <QCheckBox>
#include <iostream>

#include <last.h>

static QSize sizeIcons = QSize(32, 32);

static void createCommandActions(QWidget* pOpenScene, QToolBar* commandBar, QWidget* parent)
{
    /*QAction* pOpenAction = new QAction(QIcon(":/res/save_file32.png"), QStringLiteral("Открыть"), parent);
    pOpenAction->setProperty("Commands", QVariant((int)ExplodeWidget::Open));
    parent->addAction(pOpenAction);
    commandBar->addAction(pOpenAction);
    QObject::connect(pOpenAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    commandBar->addSeparator();*/

    QAction* TTORAction = new QAction(QIcon(":/res/iconsForHeat Exchangers/TTOR.png"), QStringLiteral("ТТОР"), parent);
    TTORAction->setProperty("CommandsHeatExhanger", QVariant((int)ExplodeWidget::TTOR));
    parent->addAction(TTORAction);
    commandBar->addAction(TTORAction);
    QObject::connect(TTORAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommandsHeats()));

    QAction* TTRMAction = new QAction(QIcon(":/res/iconsForHeat Exchangers/TTRM.png"), QStringLiteral("ТТРМ"), parent);
    TTRMAction->setProperty("CommandsHeatExhanger", QVariant((int)ExplodeWidget::TTRM));
    parent->addAction(TTRMAction);
    commandBar->addAction(TTRMAction);
    QObject::connect(TTRMAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommandsHeats()));
    
    QAction* IPAction = new QAction(QIcon(":/res/iconsForHeat Exchangers/IP.png"), QStringLiteral("ИП"), parent);
    IPAction->setProperty("CommandsHeatExhanger", QVariant((int)ExplodeWidget::IP));
    parent->addAction(IPAction);
    commandBar->addAction(IPAction);
    QObject::connect(IPAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommandsHeats()));

    QAction* IUAction = new QAction(QIcon(":/res/iconsForHeat Exchangers/IU.png"), QStringLiteral("ИУ"), parent);
    IUAction->setProperty("CommandsHeatExhanger", QVariant((int)ExplodeWidget::IU));
    parent->addAction(IUAction);
    commandBar->addAction(IUAction);
    QObject::connect(IUAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommandsHeats()));

    commandBar->addSeparator();

    QAction* pRotate360Action = new QAction(QIcon(":/res/Rotate360.png"), QStringLiteral("Вращение 360"), parent);
    pRotate360Action->setProperty("Commands", QVariant((int)ExplodeWidget::Rotation));
    parent->addAction(pRotate360Action);
    commandBar->addAction(pRotate360Action);
    pRotate360Action->setCheckable(true);
    QObject::connect(pRotate360Action, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

    //QAction* pFixingAction = new QAction(QIcon(":/res/constraints/proc_fix_32.png"), QStringLiteral("Фиксирование"), parent);
    //pFixingAction->setProperty("Commands", QVariant((int)ExplodeWidget::FixItem));
    //// (int)ExplodeWidget::FixItem
    //parent->addAction(pFixingAction);
    //commandBar->addAction(pFixingAction);
    //pFixingAction->setCheckable(true);
    //QObject::connect(pFixingAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));


    //QAction* pCoincidenceAction = new QAction(QIcon(":/res/constraints/proc_coin_32.png"), QStringLiteral("Совмещение"), parent);
    //pCoincidenceAction->setProperty("Commands", QVariant((int)ExplodeWidget::Coincident));
    //parent->addAction(pCoincidenceAction);
    //commandBar->addAction(pCoincidenceAction);
    //pCoincidenceAction->setCheckable(true);
    //QObject::connect(pCoincidenceAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

}

static QAction* createButton(const QString& fileName, QGroupBox* groupFilter, QHBoxLayout* fGroupLayout)
{
    QToolButton* filterBut = new QToolButton(groupFilter);
    filterBut->setIconSize(QSize(24, 24));
    QAction* actFilter = new QAction(QIcon(fileName), "", groupFilter);
    actFilter->setObjectName(QString("ID_") + fileName);
    actFilter->setCheckable(true);
    filterBut->setDefaultAction(actFilter);
    fGroupLayout->addWidget(filterBut);
    return actFilter;
}

static QWidget* labelEdit(QVBoxLayout* vGroupLayout, const QString& str, bool doubleEdit, bool readOnly, int strMaxWidth)
{
    QHBoxLayout* hGroupLayout = new QHBoxLayout();
    QLabel* labelKey = new QLabel(str);
    QWidget* lineEdit = nullptr;
    if (doubleEdit)
        lineEdit = new QDoubleSpinBox();
    else
    {
        lineEdit = new QLineEdit();
        ((QLineEdit*)lineEdit)->setReadOnly(readOnly);
    }
    labelKey->setBuddy(lineEdit);
    if (strMaxWidth != -1)
        labelKey->setMinimumWidth(strMaxWidth);
    hGroupLayout->addWidget(labelKey);
    hGroupLayout->addWidget(lineEdit);
    if (vGroupLayout)
        vGroupLayout->addLayout(hGroupLayout);
    return lineEdit;
}
// ---
int main(int argc, char** argv)
{
#if defined(WIN32) /*&& defined(HUNT_FOR_MEM_LEAKS)*/
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    Math::SetMultithreadedMode(mtm_Off);

    Application vapp;
    QApplication app(argc, argv);
    app.setApplicationName("Exchangers parametrization");
    app.setOrganizationName("Moscow Polytechnic University");

    if (!QtVision::activateLicense())
        return 0;

    QMainWindow mainWindow;

    mainWindow.setWindowTitle(QStringLiteral("Электронная модель теплообменника"));

    ExplodeWidget* pOpenScene = new ExplodeWidget();
    auto explodeManager = pOpenScene->getExplodeManager();
    QtVision::QtAutoHideWindow* pAutoHideWindow = pOpenScene->createHeadToolbar();
    pAutoHideWindow->toolBar()->setAllowedAreas(Qt::RightToolBarArea);

    QDockWidget* pDockGeometryList = new QDockWidget(QStringLiteral("Дерево компонентов"));
    pDockGeometryList->setFeatures(QDockWidget::NoDockWidgetFeatures);
    QWidget* pGeometryList = pOpenScene->createGeometryList(pDockGeometryList);
    pDockGeometryList->setWidget(pGeometryList);
    mainWindow.addDockWidget(Qt::LeftDockWidgetArea, pDockGeometryList);

    QWidget& widget = *new QWidget();
    mainWindow.setCentralWidget(&widget);

    QHBoxLayout* hLayout = new QHBoxLayout(&widget);
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->setMargin(2);

    hLayout->addWidget(pOpenScene, 1);
    hLayout->addLayout(vLayout);


    ///////////////////////////////////////////////////////////////////////////
    // Toolbar left position
    ///////////////////////////////////////////////////////////////////////////
    QToolBar* commandBar = new QToolBar();
    commandBar->setBaseSize(sizeIcons);
    ::createCommandActions(pOpenScene, commandBar, &mainWindow);
    mainWindow.addToolBar(Qt::LeftToolBarArea, commandBar);

    ///////////////////////////////////////////////////////////////////////////
    // Explode
    QFontMetrics fm(pOpenScene->font());
    QRect rcFont = fm.boundingRect('X');
    int heightButton = (rcFont.height() + rcFont.width()) + 4;
    QGroupBox* groupExpl = pOpenScene->createGroupExplode(widget, heightButton, u8"Сцена");
    
    ///////////////////////////////////////////////////////////////////////////
    // Add all containers in right menu
    ///////////////////////////////////////////////////////////////////////////
    vLayout->addWidget(groupExpl, 0, Qt::AlignTop);

    ///////////////////////////////////////////////////////////////////////////
    // Create menu bar and menus
    ///////////////////////////////////////////////////////////////////////////
    QMenuBar* menuBar = new QMenuBar(&mainWindow);
    mainWindow.setMenuBar(menuBar);

    QMenu* fileMenu = menuBar->addMenu(QStringLiteral("Файл"));
    QMenu* viewMenu = menuBar->addMenu(QStringLiteral("Вид"));
    QMenu* optionsMenu = menuBar->addMenu(QStringLiteral("Опции"));

    // Add actions in menu file
    QAction* open3dFile = new QAction(QStringLiteral("Открыть файл"), fileMenu);
    fileMenu->addAction(open3dFile);

    QObject::connect(open3dFile, &QAction::triggered, pOpenScene, [=]() {
        pOpenScene->loadModel();
        });

    QAction* save3dFile = new QAction(QStringLiteral("Сохранить файл"), fileMenu);
    fileMenu->addAction(save3dFile);

    QObject::connect(save3dFile, &QAction::triggered, pOpenScene, [=]() {
        pOpenScene->saveModel();
        });
    
    // Add actions in menu options
    QAction* showUnshowExploding = new QAction(QStringLiteral("Скрыть/показать разлёт"), optionsMenu);
    showUnshowExploding->setCheckable(true);
    showUnshowExploding->setChecked(true);
    QObject::connect(showUnshowExploding, &QAction::triggered, pOpenScene, [=]() {
        pOpenScene->slotToggleVisibility(showUnshowExploding->isChecked(), explodeManager->getExplodingGroupBox());
        });

    QAction* showUnshowColors = new QAction(QStringLiteral("Скрыть/показать фильтры"), optionsMenu);
    showUnshowColors->setCheckable(true);
    showUnshowColors->setChecked(true);
    QObject::connect(showUnshowColors, &QAction::triggered, pOpenScene, [=]() {
        pOpenScene->slotToggleVisibility(showUnshowColors->isChecked(), explodeManager->getFiltersGroupBox());
        });

    QAction* showUnshowSelectors = new QAction(QStringLiteral("Скрыть/показать селектирование"), optionsMenu);
    showUnshowSelectors->setCheckable(true);
    showUnshowSelectors->setChecked(true);
    QObject::connect(showUnshowSelectors, &QAction::triggered, pOpenScene, [=]() {
        pOpenScene->slotToggleVisibility(showUnshowSelectors->isChecked(), explodeManager->getSelectionsGroupBox());
        });
    
    QAction* showUnshowRenders = new QAction(QStringLiteral("Скрыть/показать рендеринг"), optionsMenu);
    showUnshowRenders->setCheckable(true);
    showUnshowRenders->setChecked(true);
    QObject::connect(showUnshowRenders, &QAction::triggered, pOpenScene, [=]() {
        pOpenScene->slotToggleVisibility(showUnshowRenders->isChecked(), explodeManager->getRenderingGroupBox());
        });
    
    QAction* showUnshowPlaneCutting= new QAction(QStringLiteral("Скрыть/показать сечение"), optionsMenu);
    showUnshowPlaneCutting->setCheckable(true);
    showUnshowPlaneCutting->setChecked(false);
    QObject::connect(showUnshowPlaneCutting, &QAction::triggered, pOpenScene, [=]() {
        pOpenScene->togglePlaneCuttingVisibility(showUnshowPlaneCutting->isChecked(), explodeManager->getPlaneCuttingGroupBox());
        });
    

    optionsMenu->addAction(showUnshowExploding);
    optionsMenu->addAction(showUnshowColors);
    optionsMenu->addAction(showUnshowSelectors);
    optionsMenu->addAction(showUnshowRenders);
    optionsMenu->addAction(showUnshowPlaneCutting);

    // Show window
    QtVision::setWindowPosition(mainWindow);
    mainWindow.show();
    return app.exec();
}
