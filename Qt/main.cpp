#include "explodewidget.h"

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
#include <iostream>

#include <last.h>

static QSize sizeIcons = QSize(32, 32);

static void createCommandActions(QWidget* pOpenScene, QToolBar* commandBar, QWidget* parent)
{
    QAction* pOpenAction = new QAction(QIcon(":/res/save_file32.png"), QObject::tr("&Open"), parent);
    pOpenAction->setProperty("Commands", QVariant((int)ExplodeWidget::Open));
    //  (int)ExplodeWidget::Open
    parent->addAction(pOpenAction);
    commandBar->addAction(pOpenAction);
    QObject::connect(pOpenAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));


    commandBar->addSeparator();

    QAction* pFixingAction = new QAction(QIcon(":/res/rotate.png"), QObject::tr("&Fix component"), parent);
    pFixingAction->setProperty("Commands", QVariant((int)ExplodeWidget::FixItem));
    // (int)ExplodeWidget::FixItem
    parent->addAction(pFixingAction);
    commandBar->addAction(pFixingAction);
    pFixingAction->setCheckable(true);
    QObject::connect(pFixingAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));


    QAction* pCoincidenceAction = new QAction(QIcon(":/res/rotate.png"), QObject::tr("&Coincident"), parent);
    pCoincidenceAction->setProperty("Commands", QVariant((int)ExplodeWidget::Coincident));
    parent->addAction(pCoincidenceAction);
    commandBar->addAction(pCoincidenceAction);
    pCoincidenceAction->setCheckable(true);
    QObject::connect(pCoincidenceAction, SIGNAL(triggered()), pOpenScene, SLOT(viewCommands()));

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

QToolButton* createToolButton(const QIcon& icon, QLayout* layout, bool checkable = true, const char* nameProp = nullptr, int value = 0)
{
    QToolButton* button = new QToolButton();
    button->setCheckable(checkable);
    button->setIconSize(sizeIcons);
    button->setIcon(icon);
    if (nameProp)
        button->setProperty(nameProp, QVariant(value));
    layout->addWidget(button);
    return button;
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
    app.setApplicationName("Group Geometry");
    app.setOrganizationName("C3DLabs");

    if (!QtVision::activateLicense())
        return 0;

    QMainWindow mainWindow;
    mainWindow.setWindowTitle(QObject::tr("Explode Scene"));

    ExplodeWidget* pOpenScene = new ExplodeWidget();
    pOpenScene->createHeadToolbar();

    QDockWidget* pDockGeometryList = new QDockWidget(QObject::tr("Scene segments"));
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
    // Create control widgets
    ///////////////////////////////////////////////////////////////////////////
    // File 
    QGroupBox* groupFile = new QGroupBox(&widget);
    groupFile->setTitle(QObject::tr("File"));
    QFormLayout* fileLayout = new QFormLayout(groupFile);
    fileLayout->setMargin(2); fileLayout->setSpacing(2);

    QHBoxLayout* rowLayoutFile = new QHBoxLayout();
    fileLayout->addRow(rowLayoutFile);
    QCommandLinkButton* openFullScene = new QCommandLinkButton();
    openFullScene->setText(QObject::tr(u8"Отобразить модель"));
    openFullScene->setIcon(QIcon(":/res/proc_ok_32.png"));
    openFullScene->setIconSize(QSize(32, 32));
    rowLayoutFile->addWidget(openFullScene);
    QObject::connect(openFullScene, SIGNAL(released()), pOpenScene, SLOT(openModel()));
    fileLayout->addRow(rowLayoutFile);

    ///////////////////////////////////////////////////////////////////////////
    // Explode
    QFontMetrics fm(pOpenScene->font());
    QRect rcFont = fm.boundingRect('X');
    int heightButton = (rcFont.height() + rcFont.width()) + 4;
    QGroupBox* groupExpl = pOpenScene->createGroupExplode(widget, heightButton, "View");

    // Create commandBar
    QToolBar* commandBar = new QToolBar();
    commandBar->setBaseSize(sizeIcons);
    ::createCommandActions(pOpenScene, commandBar, &mainWindow);
    mainWindow.addToolBar(Qt::LeftToolBarArea, commandBar);
    
    
    ///////////////////////////////////////////////////////////////////////////
    // add
    vLayout->addWidget(groupFile, 0, Qt::AlignTop);
    vLayout->addWidget(groupExpl, 0, Qt::AlignTop);

    // Show window
    QtVision::setWindowPosition(mainWindow);
    mainWindow.show();
    return app.exec();
}
