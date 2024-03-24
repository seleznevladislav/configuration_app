#include <qt_openglwidgetprivate.h>
#include <QApplication>
#include <QScreen>
#include <QWindow>
#include <QMessageBox>
#include <QThread>
#include <QDesktopServices>
#include <QProcessEnvironment>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOffscreenSurface>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QOpenGLFramebufferObject>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
#include <QScreen>
#else
#include <QDesktopWidget>
#endif

#include <vsn_window.h>
#include <vsn_openglfbo.h>
#include <vsn_graphicsview.h>
#include <vsn_renderstatistics.h>
#include <vsn_graphicssceneengine.h>
#include <vsn_graphicsscene.h>
#include <vsn_texture2d.h>
#include <tool_enabler.h>

#include <qt_aboutscenewidget.h>
#include <qt_openglcontext.h>
#include <qt_licensewnd.h>

#include <last.h>

VSN_BEGIN_NAMESPACE

namespace QtVision {

/* QtConverterEventListener */
QtConverterEventListener::QtConverterEventListener(QObject* pParent, GraphicsScene* pListenerEvent)
    : QObject(pParent)
    , m_pListenerEvent(pListenerEvent)
    , m_idleTimerId(-1)
{
    Q_ASSERT(m_pListenerEvent != nullptr);
    parent()->installEventFilter(this);
#ifndef VSN_OS_WIN
    m_idleTimerId = startTimer(QGuiApplication::primaryScreen()->refreshRate());
#endif
}

//-----------------------------------------------------------------------------
//
// ---
QtConverterEventListener::~QtConverterEventListener()
{
    parent()->removeEventFilter(this);
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::keyPressEvent(QKeyEvent* event)
{
    KeyEvent processEvent(ProcessEvent::KeyDown, event->key(), event->nativeScanCode(), event->nativeVirtualKey(), event->nativeModifiers());
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::keyReleaseEvent(QKeyEvent* event)
{
    KeyEvent processEvent(ProcessEvent::KeyUp, event->key(), event->nativeScanCode(), event->nativeVirtualKey(), event->nativeModifiers());
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseHoverEvent(QHoverEvent* event)
{
    Q_ASSERT(m_pListenerEvent != nullptr);
    int flags = 0;
    HoverEvent processEvent(ProcessEvent::HoverMove, PointI(event->posF().x(), event->posF().y()), PointI());
    processEvent.SetFlags(flags);
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
static int mapModifiers(Qt::KeyboardModifiers state)
{
    int flag = 0;
    if (state & Qt::ShiftModifier)
        flag |= mb_Shift;
    if (state & Qt::ControlModifier)
        flag |= mb_Control;
    if (state & Qt::AltModifier)
        flag |= mb_Alt;
    return flag;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mousePressEvent(QMouseEvent* event)
{
    Q_ASSERT(m_pListenerEvent != nullptr);
    ProcessEvent::EventType type = ProcessEvent::None;
    int flags = mapModifiers(event->modifiers());
    if (event->button() == Qt::LeftButton)
    {
        flags |= mb_LButton;
        type = ProcessEvent::LButtonDown;
    }
    else if (event->button() == Qt::MiddleButton)
    {
        flags |= mb_MButton;
        type = ProcessEvent::MButtonDown;
    }
    else if (event->button() == Qt::RightButton)
    {
        flags |= mb_RButton;
        type = ProcessEvent::RButtonDown;
    }

    ViewportPtr ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(type);
    processEvent.SetFlags(flags);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    processEvent.SetScreenPos(PointI(event->screenPos().x(), event->screenPos().y()));
    processEvent.SetWindowPos(PointI(event->windowPos().x(), event->windowPos().y()));
    processEvent.SetClientPos(PointI(event->localPos().x(), event->localPos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseMoveEvent(QMouseEvent* event)
{
    int flags = mapModifiers(event->modifiers());
    if (event->buttons() == Qt::LeftButton)
        flags |= mb_LButton;
    if (event->buttons() == Qt::MiddleButton)
        flags |= mb_MButton;
    if (event->buttons() == Qt::RightButton)
        flags |= mb_RButton;

    ViewportPtr ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(ProcessEvent::MouseMove);
    processEvent.SetFlags(flags);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    processEvent.SetScreenPos(PointI(event->screenPos().x(), event->screenPos().y()));
    processEvent.SetWindowPos(PointI(event->windowPos().x(), event->windowPos().y()));
    processEvent.SetClientPos(PointI(event->localPos().x(), event->localPos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseReleaseEvent(QMouseEvent* event)
{
    ProcessEvent::EventType type = ProcessEvent::None;
    int flags = mapModifiers(event->modifiers());
    if (event->button() == Qt::LeftButton)
    {
        flags |= mb_LButton;
        type = ProcessEvent::LButtonUp;
    }
    else if (event->button() == Qt::MiddleButton)
    {
        flags |= mb_MButton;
        type = ProcessEvent::MButtonUp;
    }
    else if (event->button() == Qt::RightButton)
    {
        flags |= mb_RButton;
        type = ProcessEvent::RButtonUp;
    }

    ViewportPtr ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(type);
    processEvent.SetFlags(flags);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    processEvent.SetScreenPos(PointI(event->screenPos().x(), event->screenPos().y()));
    processEvent.SetWindowPos(PointI(event->windowPos().x(), event->windowPos().y()));
    processEvent.SetClientPos(PointI(event->localPos().x(), event->localPos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseDoubleClickEvent(QMouseEvent* event)
{
    ProcessEvent::EventType type = ProcessEvent::None;
    int flags = mapModifiers(event->modifiers());
    if (event->button() == Qt::LeftButton)
    {
        flags |= mb_LButton;
        type = ProcessEvent::LButtonDblClk;
    }
    else if (event->button() == Qt::MiddleButton)
    {
        flags |= mb_MButton;
        type = ProcessEvent::MButtonDblClk;
    }
    else if (event->button() == Qt::RightButton)
    {
        flags |= mb_RButton;
        type = ProcessEvent::RButtonDblClk;
    }

    ViewportPtr ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(type);
    processEvent.SetFlags(flags);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    processEvent.SetScreenPos(PointI(event->screenPos().x(), event->screenPos().y()));
    processEvent.SetWindowPos(PointI(event->windowPos().x(), event->windowPos().y()));
    processEvent.SetClientPos(PointI(event->localPos().x(), event->localPos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseWheelEvent(QWheelEvent* event)
{
    WheelEvent processEvent;
    int flags = mapModifiers(event->modifiers());
    processEvent.SetFlags(flags);
    processEvent.SetWheelDelta(event->delta());
    processEvent.SetWindowPos(PointI(event->pos().x(), event->pos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseEnterEvent(QEnterEvent* event)
{
    ViewportPtr ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(ProcessEvent::MouseEnter);
    processEvent.SetFlags(0);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    processEvent.SetScreenPos(PointI(event->screenPos().x(), event->screenPos().y()));
    processEvent.SetWindowPos(PointI(event->windowPos().x(), event->windowPos().y()));
    processEvent.SetClientPos(PointI(event->localPos().x(), event->localPos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseLeaveEvent(QEvent* event)
{
    ViewportPtr ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(ProcessEvent::MouseLeave);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
// Перегрузка для внутренних работ.
// ---
bool QtConverterEventListener::eventFilter(QObject* watched, QEvent* event)
{
    if (watched != parent())
        return QObject::eventFilter(watched, event);

    switch (event->type())
    {
        case QEvent::KeyPress:
            return keyPressEvent(static_cast<QKeyEvent*>(event));
        case QEvent::KeyRelease:
            return keyReleaseEvent(static_cast<QKeyEvent*>(event));
        case QEvent::HoverMove:
            return mouseHoverEvent(static_cast<QHoverEvent*>(event));
        case QEvent::MouseMove:
            return mouseMoveEvent(static_cast<QMouseEvent*>(event));
        case QEvent::MouseButtonDblClick:
            return mouseDoubleClickEvent(static_cast<QMouseEvent*>(event));
        case QEvent::MouseButtonPress:
            return mousePressEvent(static_cast<QMouseEvent*>(event));
        case QEvent::MouseButtonRelease:
            return mouseReleaseEvent(static_cast<QMouseEvent*>(event));
        case QEvent::Wheel:
            return mouseWheelEvent(static_cast<QWheelEvent*>(event));
        case QEvent::Enter:
            return mouseEnterEvent(static_cast<QEnterEvent*>(event));
        case QEvent::Leave:
            return mouseLeaveEvent(event);
        default:
            break;
    }
    return QObject::eventFilter(watched, event);
}

//-----------------------------------------------------------------------------
//
// ---
void QtConverterEventListener::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_idleTimerId)
        BaseApplication::OnProcessSendPostedEvents();
    else
        QObject::timerEvent(event);
}

inline QtOpenGLWidgetPrivate::~QtOpenGLWidgetPrivate()
{
    if (m_pQContext != nullptr)
        m_pQContext->MakeCurrent();
    reset();
}

//-----------------------------------------------------------------------------
// 
// ---
void QtOpenGLWidgetPrivate::initialize()
{
    VSN_P(QtOpenGLWidget);
    if (m_bInitialized)
        return;
    qApp->setWindowIcon(QIcon(":/res/crystal.ico"));
    p.makeCurrent();
    m_pQContext = new QtOpenGLContextShell(&p, m_pSharedContainer);
    m_pQContext->MakeCurrent();
    m_bInitialized = true;
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidgetPrivate::reset()
{
    VSN_P(QtOpenGLWidget);
    VSN_DELETE_AND_NULL(m_pQContext);
    m_bInitialized = false;
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidgetPrivate::renderNow()
{
    VSN_P(QtOpenGLWidget);
    QOpenGLContext* openGLContext = p.context();
    if (!openGLContext)
        return;
    p.makeCurrent();
    p.paintGL();
    openGLContext->swapBuffers(openGLContext->surface());
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidgetPrivate::renderLater()
{
    VSN_P(QtOpenGLWidget);
    if (!m_bUpdatePending)
    {
        m_bUpdatePending = true;
        QCoreApplication::postEvent(&p, new QEvent(QEvent::UpdateRequest));
    }
}

//-----------------------------------------------------------------------------
// Конструктор по умолчанию.
// ---
QtOpenGLWidget::QtOpenGLWidget(QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f)
{
    VSN_INIT_PRIVATE(QtOpenGLWidget);
}

//-----------------------------------------------------------------------------
// Конструктор по умолчанию.
// ---
QtOpenGLWidget::QtOpenGLWidget(QtOpenGLWidgetPrivate& dd, QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f)
{
    vsn_d_ptr = &dd;
    vsn_d_ptr->setPublic(this);
}

//-----------------------------------------------------------------------------
// Деструктор OpenGLWidget освобождает созданные ресурсы
// ---
QtOpenGLWidget::~QtOpenGLWidget()
{
    // Вылет при закрытии вкладки документа
    makeCurrent();
    VSN_FINI_PRIVATE();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidget::makeCurrent()
{
    VSN_D(QtOpenGLWidget);
    if (d.m_pQContext)
        d.m_pQContext->MakeCurrent();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidget::doneCurrent()
{
    VSN_D(QtOpenGLWidget);
    if (d.m_pQContext)
        d.m_pQContext->DoneCurrent();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на OpenGLContextInterface.
// ---
OpenGLContextInterface* QtOpenGLWidget::contextInterface()
{
    VSN_D(QtOpenGLWidget);
    return d.m_pQContext;
}

//-----------------------------------------------------------------------------
// 
// ---
void QtOpenGLWidget::showCompanyWebSite()
{
    QDesktopServices::openUrl(QUrl(QStringLiteral("https://c3dlabs.com")));
}

//-----------------------------------------------------------------------------
// Просто вызывает функцию QWidget::update(). Служит для совместимости слотов.
// ---
void QtOpenGLWidget::updateWidget()
{
    update();
}

//-----------------------------------------------------------------------------
// Просто вызывает функцию QWidget::rapaint(). Служит для совместимости слотов.
// ---
void QtOpenGLWidget::repaintWidget()
{
    repaint();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidget::initializeGL()
{
    VSN_D(QtOpenGLWidget);
    if (!d.m_bInitialized && window() && window()->windowHandle())
        d.initialize();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidget::paintEvent(QPaintEvent* event)
{
    QOpenGLWidget::paintEvent(event);
}

//-----------------------------------------------------------------------------
//
// ---
bool QtOpenGLWidget::event(QEvent* event)
{
    VSN_D(QtOpenGLWidget);
    if (event->type() == QEvent::WindowChangeInternal && d.m_bInitialized && !qGuiApp->testAttribute(Qt::AA_ShareOpenGLContexts))
        d.reset();

    bool ok = QOpenGLWidget::event(event);

    switch (event->type())
    {
        case QEvent::UpdateRequest:
            if (d.m_bUpdatePending)
            {
                update();
                d.m_bUpdatePending = false;
                return true;
            }
            break;
        case QEvent::WindowChangeInternal:
            if (qGuiApp->testAttribute(Qt::AA_ShareOpenGLContexts))
                break;
            if (d.m_bInitialized)
                d.reset();
            // FALLTHROUGH
        case QEvent::Show: // reparenting may not lead to a resize so reinitalize on Show too
            if (!d.m_bInitialized && !size().isEmpty() && window() && window()->windowHandle())
                d.initialize();
            break;
        default:
            break;
    }
    return ok;
}

//-----------------------------------------------------------------------------
//
// ---
QImage QtOpenGLWidget::grabFrameImage(const GraphicsView* pGraphicsView, const SnapshotSettings& settings)
{
    makeCurrent();
    Image image = pGraphicsView->ToImage(width(), height(), settings);
    size_t size = (uint64)image.GetWidth() * (uint64)image.GetHeight();
    QImage::Format format = QImage::Format::Format_Invalid;
    switch (image.GetFormat())
    {
    case Image::Format::UINT8:
        format = QImage::Format_Indexed8;
        break;
    case Image::Format::UINT24:
        size *=3;
        format = QImage::Format_RGB888;
        break;
    case Image::Format::UINT32:
        size *= 4;
        format = QImage::Format_ARGB32;
        break;
    default:
        size = 0;
        break;
    };
    QImage qImage = QImage(image.GetWidth(), image.GetHeight(), format);
    memcpy(qImage.bits(), image.GetBits(), size);
    return qImage.mirrored();
}

/* OpenGLWidgetPrivate */
class QtOpenGLSceneWidgetPrivate : public QtOpenGLWidgetPrivate
{
    VSN_DECLARE_EX_PUBLIC(QtOpenGLSceneWidget)
public:
    explicit QtOpenGLSceneWidgetPrivate(GraphicsSceneEnginePtr ptrEngine, OpenGLContextContainer* pSharedContainer)
        : QtOpenGLWidgetPrivate(pSharedContainer)
        , m_pGraphicsView(std::make_shared<GraphicsView>(ptrEngine))
        , m_pEventFilter(nullptr)
        , m_pAboutScene(nullptr)
        , m_pAutoHideWindow(nullptr)
        , m_bVsnInitialized(false)
    {
    }
    virtual ~QtOpenGLSceneWidgetPrivate()
    {
        m_pGraphicsView->GetSceneEngine()->GetObjectPickSelection()->ResetOpenGLContext(m_pQContext);
        VSN_DELETE_AND_NULL(m_pEventFilter);
    }
    virtual void initialize() override;
public:
    void createAutoHideWindow();
    void processActivation();
    void processDeactivation();
public:
    GraphicsViewPtr m_pGraphicsView;
    QtConverterEventListener* m_pEventFilter;
    QtAboutSceneWidget* m_pAboutScene;
    QtAutoHideWindow* m_pAutoHideWindow;
    bool m_bVsnInitialized;
};

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidgetPrivate::initialize()
{
    VSN_P(QtOpenGLSceneWidget);
    if (m_bInitialized)
        return;
    QtOpenGLWidgetPrivate::initialize();
    if (m_bVsnInitialized)
        return;
    p.setFocusPolicy(Qt::StrongFocus);
    p.setMouseTracking(true);
    p.setAttribute(Qt::WA_Hover, true);
    m_pGraphicsView->Initialize();
    m_pEventFilter = new QtConverterEventListener(&p, m_pGraphicsView->GetGraphicsScene());
    Object::Connect(m_pGraphicsView.get(), &GraphicsView::OnViewModified, &p, &QtOpenGLWidget::repaintWidget);
    Object::Connect(m_pGraphicsView->GetViewport().get(), &Viewport::RefreshScreen, &p, &QtOpenGLWidget::repaintWidget);
    m_bVsnInitialized = true;
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidgetPrivate::createAutoHideWindow()
{
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidgetPrivate::processActivation()
{
    VSN_P(QtOpenGLSceneWidget);
    if (Essence* pEssence = m_pGraphicsView->GetSceneEngine()->GetTopEssence())
    {
        auto list = pEssence->FindChildren<Process*>();
        for (const auto& it : list)
        {
            it->SetViewport(m_pGraphicsView->GetViewport().get());
            bool bConnect = Object::Connect(it, &Process::OnModified, &p, &QtOpenGLWidget::repaintWidget);
            Q_ASSERT(bConnect != false);
            if (PrAbstractCamera* pAbsProcess = vobject_cast<PrAbstractCamera*>(it))
            {
                bool bConnect = Object::Connect(pAbsProcess, &PrAbstractCamera::OnCameraModified, &p, &QtOpenGLWidget::repaintWidget);
                Q_ASSERT(bConnect != false);
            }

        }
    }
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidgetPrivate::processDeactivation()
{
    VSN_P(QtOpenGLSceneWidget);
    if (Essence* pEssence = m_pGraphicsView->GetSceneEngine()->GetTopEssence())
    {
        auto list = pEssence->FindChildren<Process*>();
        for (const auto& it : list)
        {
            it->SetViewport(m_pGraphicsView->GetViewport().get());
            Object::Disconnect(it, &Process::OnModified, &p, &QtOpenGLWidget::repaintWidget);

            if (PrAbstractCamera* pAbsProcess = vobject_cast<PrAbstractCamera*>(it))
                Object::Disconnect(pAbsProcess, &PrAbstractCamera::OnCameraModified, &p, &QtOpenGLWidget::repaintWidget);
        }
    }
}


/* QtOpenGLSceneWidget */
QtOpenGLSceneWidget::QtOpenGLSceneWidget(QWidget* parent, Qt::WindowFlags f)
    : QtOpenGLWidget(*new QtOpenGLSceneWidgetPrivate(std::make_shared<GraphicsSceneEngine>(), nullptr), parent, f)
{
}

//-----------------------------------------------------------------------------
// Special constructor.
// ---
QtOpenGLSceneWidget::QtOpenGLSceneWidget(GraphicsSceneEnginePtr engine, OpenGLContextContainer* sharedContainer, QWidget* parent, Qt::WindowFlags f)
    : QtOpenGLWidget(*new QtOpenGLSceneWidgetPrivate(engine, sharedContainer), parent, f)
{
}

//-----------------------------------------------------------------------------
//
// ---
QtOpenGLSceneWidget::~QtOpenGLSceneWidget()
{
}

//-----------------------------------------------------------------------------
//
// ---
GraphicsScene* QtOpenGLSceneWidget::graphicsScene() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_pGraphicsView->GetGraphicsScene();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на компонент выбора объектов.
// ---
ObjectPickSelection* QtOpenGLSceneWidget::objectPickSelection() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_pGraphicsView->GetSceneEngine()->GetObjectPickSelection();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на GraphicsSceneEngine.
// ---
GraphicsSceneEnginePtr QtOpenGLSceneWidget::graphicsEngine() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_pGraphicsView->GetSceneEngine();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на GraphicsView.
// ---
GraphicsViewPtr QtOpenGLSceneWidget::ptrGraphicsView() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_pGraphicsView;
}

// \ru Вернуть указатель на GraphicsView
GraphicsView* QtOpenGLSceneWidget::graphicsView() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_pGraphicsView.get();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::setAboutSceneWidget(QtAboutSceneWidget* pAboutScene)
{
    VSN_D(QtOpenGLSceneWidget);
    d.m_pAboutScene = pAboutScene;
}

//-----------------------------------------------------------------------------
// Вернуть указатель на основной источник света
// ---
QtAutoHideWindow* QtOpenGLSceneWidget::createHeadToolbar()
{
    VSN_D(QtOpenGLSceneWidget);
    d.m_pAutoHideWindow = new QtAutoHideWindow(this);
    QToolBar* pHeadToolbar = new QToolBar(this);
    d.m_pAutoHideWindow->setToolBar(pHeadToolbar);

    QAction* actionViewFront = new QAction(QIcon(":/res/view_front_32.png"), QObject::tr("Front"), this);
    actionViewFront->setProperty("Orientation", QVariant((int)Orientation::Front));
    pHeadToolbar->addAction(actionViewFront);
    QObject::connect(actionViewFront, SIGNAL(triggered()), this, SLOT(viewPortCommands()));

    QAction* actionViewBack = new QAction(QIcon(":/res/view_back_32.png"), QObject::tr("Back"), this);
    actionViewBack->setProperty("Orientation", QVariant((int)Orientation::Rear));
    pHeadToolbar->addAction(actionViewBack);
    QObject::connect(actionViewBack, SIGNAL(triggered()), this, SLOT(viewPortCommands()));

    QAction* actionViewTop = new QAction(QIcon(":/res/view_top_32.png"), QObject::tr("Top"), this);
    actionViewTop->setProperty("Orientation", QVariant((int)Orientation::Up));
    pHeadToolbar->addAction(actionViewTop);
    QObject::connect(actionViewTop, SIGNAL(triggered()), this, SLOT(viewPortCommands()));

    QAction* actionViewBottom = new QAction(QIcon(":/res/view_bottom_32.png"), QObject::tr("Bottom"), this);
    actionViewBottom->setProperty("Orientation", QVariant((int)Orientation::Down));
    pHeadToolbar->addAction(actionViewBottom);
    QObject::connect(actionViewBottom, SIGNAL(triggered()), this, SLOT(viewPortCommands()));

    QAction* actionViewLeft = new QAction(QIcon(":/res/view_left_32.png"), QObject::tr("Left"), this);
    actionViewLeft->setProperty("Orientation", QVariant((int)Orientation::Left));
    pHeadToolbar->addAction(actionViewLeft);
    QObject::connect(actionViewLeft, SIGNAL(triggered()), this, SLOT(viewPortCommands()));

    QAction* actionViewRight = new QAction(QIcon(":/res/view_right_32.png"), QObject::tr("Right"), this);
    actionViewRight->setProperty("Orientation", QVariant((int)Orientation::Right));
    pHeadToolbar->addAction(actionViewRight);
    QObject::connect(actionViewRight, SIGNAL(triggered()), this, SLOT(viewPortCommands()));

    QAction* actionViewIsometry_x = new QAction(QIcon(":/res/isometry_x.png"), QObject::tr("Isometry"), this);
    actionViewIsometry_x->setProperty("Orientation", QVariant((int)Orientation::IsoXYZ));
    pHeadToolbar->addAction(actionViewIsometry_x);
    QObject::connect(actionViewIsometry_x, SIGNAL(triggered()), this, SLOT(viewPortCommands()));

    pHeadToolbar->addSeparator();

    QAction* actionZoomFit = new QAction(QIcon(":/res/view_all_32.png"), QObject::tr("Zoom Fit"), this);
    actionZoomFit->setProperty("Orientation", QVariant((int)Orientation::CustomBase + 1));
    pHeadToolbar->addAction(actionZoomFit);
    QObject::connect(actionZoomFit, SIGNAL(triggered()), this, SLOT(viewPortCommands()));

    QAction* actionPerspective = new QAction(QIcon(":/res/perspctv_32.png"), QObject::tr("Perspective"), this);
    actionPerspective->setProperty("Orientation", QVariant((int)Orientation::CustomBase + 2));
    actionPerspective->setCheckable(true);
    actionPerspective->setChecked(!viewport()->IsOrthographicProjection());
    pHeadToolbar->addAction(actionPerspective);
    QObject::connect(actionPerspective, SIGNAL(triggered()), this, SLOT(viewPortCommands()));

    d.m_pAutoHideWindow->setToolBar(pHeadToolbar);
    return d.m_pAutoHideWindow;
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::viewPortCommands()
{
    QAction* pAction = qobject_cast<QAction*>(sender());
    Orientation value = static_cast<Orientation>(pAction->property("Orientation").toInt());
    int val = int(Orientation::CustomBase);
    if ((int)value == (val + 1))
        graphicsView()->SetZoomToFit(sceneContent()->GetBoundingBox());
    else if ((int)value == (val + 2))
        viewport()->SetOrthographicProjection(!pAction->isChecked());
    else
        graphicsView()->SetOrientationCamera(value, true);
    update();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на основной источник света
// ---
Light* QtOpenGLSceneWidget::mainLight() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_pGraphicsView->GetGraphicsScene()->GetMainLight();
}

//-----------------------------------------------------------------------------
//
// ---
Camera* QtOpenGLSceneWidget::camera() const
{
    return viewport()->GetCamera();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на Viewport для отображения сцены.
// ---
Viewport* QtOpenGLSceneWidget::viewport() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_pGraphicsView->GetViewport().get();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на содержимое сцены.
// ---
SceneContentPtr QtOpenGLSceneWidget::sceneContent()
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_pGraphicsView->GetGraphicsScene()->GetSceneContent();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::resizeGL(int w, int h)
{
    VSN_D(QtOpenGLSceneWidget);
    QtOpenGLWidget::resizeGL(w, h);
    if (d.m_pQContext == nullptr)
        return;
    d.m_pQContext->MakeCurrent();
    if (OpenGLContextInterface::GetCurrentContext() && d.m_pQContext == OpenGLContextInterface::GetCurrentContext())
        d.m_pGraphicsView->DoResize(w, h);

    if (d.m_pAboutScene != nullptr)
    {
        const int margin = 10;
        QRect rc = rect();
        QSize sz = d.m_pAboutScene->sizeHint();
        rc.setTopLeft(QPoint(rc.right() - sz.width() - margin, margin));
        d.m_pAboutScene->setPosition(rc.topLeft());
    }
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::paintGL()
{
    VSN_D(QtOpenGLSceneWidget);
    if (d.m_pQContext == nullptr)
        return;

    if (RenderStatistics::IsActivated())
        RenderStatistics::ResetRenderCounters();

    d.m_pQContext->MakeCurrent();
    d.m_pGraphicsView->DoRender();
    d.m_pQContext->DoneCurrent();

    if (d.m_pAboutScene != Q_NULLPTR)
    {
        QPainter painter(this);
        d.m_pAboutScene->updateStatistics();
        d.m_pAboutScene->paintStatistics(&painter);
    }
}

//-----------------------------------------------------------------------------
//
// ---
bool QtOpenGLSceneWidget::event(QEvent* event)
{
    return QtOpenGLWidget::event(event);
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::enterEvent(QEvent* event)
{
    VSN_D(QtOpenGLSceneWidget);
    QtOpenGLWidget::enterEvent(event);
    d.m_pGraphicsView->GetSceneEngine()->GetObjectPickSelection()->SetOpenGLContext(d.m_pQContext);
    d.m_pGraphicsView->GetSceneEngine()->GetObjectPickSelection()->SetViewport(d.m_pGraphicsView->GetViewport());
    d.processActivation();
}

//-----------------------------------------------------------------------------
//
// ---
 void QtOpenGLSceneWidget::leaveEvent(QEvent* event)
{
    VSN_D(QtOpenGLSceneWidget);
    QtOpenGLWidget::leaveEvent(event);
    d.processDeactivation();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::focusInEvent(QFocusEvent* event)
{
    VSN_D(QtOpenGLSceneWidget);
    QtOpenGLWidget::focusInEvent(event);
    d.processActivation();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::focusOutEvent(QFocusEvent* event)
{
    VSN_D(QtOpenGLSceneWidget);
    QtOpenGLWidget::focusOutEvent(event);
    d.processDeactivation();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::ZoomToFit()
{
    VSN_D(QtOpenGLSceneWidget);
    if ( viewport() && sceneContent() )
        viewport()->ZoomToFit(sceneContent()->GetBoundingBox());
}

//-----------------------------------------------------------------------------
//
// ---
bool QtOpenGLSceneWidget::OnEvent(ProcessEvent* event)
{
    if (event->GetType() == ProcessEvent::Draw)
        repaint();
    return Object::OnEvent(event);
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::showEvent(QShowEvent* event)
{
    VSN_D(QtOpenGLSceneWidget);
    if (!d.m_bVsnInitialized)
        return;
    d.m_pGraphicsView->PreparingToDisplay();
    d.renderLater();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::closeEvent(QCloseEvent* event)
{
    QtOpenGLWidget::closeEvent(event);
}

//-----------------------------------------------------------------------------
// Creating processes to control the camera.
// ---
QT_FUNC(void) createProcessesCameraControls(QtOpenGLSceneWidget* pGLWidget, ProcessTypes prType, SceneSegment* pParent)
{
    Essence* pEssence = pParent == nullptr ? pGLWidget->graphicsEngine()->GetTopEssence() : pParent;
    Q_ASSERT(pEssence != nullptr);

    // Create and add process Orbit
    if (prType.CheckFlag(pt_Orbit))
    {
        PrCameraOrbit* pProcess = new PrCameraOrbit(pGLWidget->graphicsScene(), pEssence);
        pProcess->SetMouseButtonsActivate({ VSN::mb_RButton });
    }
    // Create and add process Rotate
    if (prType.CheckFlag(pt_Rotate))
    {
        PrCameraRotate* pProcess = new PrCameraRotate(pGLWidget->graphicsScene(), pEssence);
        pProcess->SetMouseButtonsActivate({ VSN::mb_RButton });
    }
    // Create and add process Rotate
    if (prType.CheckFlag(pt_ComplexRotation))
    {
        PrCameraComplexRotation* pProcess = new PrCameraComplexRotation(pGLWidget->graphicsScene(), pEssence);
        pProcess->SetMouseButtonsActivate({ VSN::mb_RButton });
    }
    // Create and add process Pan
    if (prType.CheckFlag(pt_Pan))
    {
        PrCameraPan* pProcess = new PrCameraPan(pGLWidget->graphicsScene(), pEssence);
        pProcess->SetMouseButtonsActivate({ VSN::mb_MButton });
    }
    // Create and add process Zoom
    if (prType.CheckFlag(pt_Zoom))
    {
        PrCameraZoomWheel* pProcess = new PrCameraZoomWheel(pGLWidget->graphicsScene(), pEssence);
        VSN_UNUSED(pProcess);
    }
}

//-----------------------------------------------------------------------------
// Setting the format for QSurface.
// ---
QT_FUNC(void) setSurfaceFormat()
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setSamples(4);
    format.setStencilBufferSize(8);
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);
}
/* SurfaceFormat */
struct SurfaceFormat {SurfaceFormat() { setSurfaceFormat(); }};
static SurfaceFormat format;

//-----------------------------------------------------------------------------
// License check
// ---
QT_FUNC(bool) activateLicense()
{
    VSN_ASSERT_X(QApplication::instance() != nullptr, "QtVision::activateLicense()", "QtVision::activateLicense() should be called after QApplication created!");

    QProcessEnvironment procEnv = QProcessEnvironment::systemEnvironment();
    QString envKey = procEnv.value("C3DKey");
    QString envSignature = procEnv.value("C3Dsignature");
    if (!envKey.isEmpty() && !envSignature.isEmpty())
    {
        const std::string key = envKey.toStdString();
        const std::string signature = envSignature.toStdString();

        ::EnableMathModules(key.c_str(), static_cast<int>(key.length()), signature.c_str(), static_cast<int>(signature.length()));
    }

    if (!IsMathVisionEnable())
    {
        QtVision::LicenseWnd licenseWnd;
        return licenseWnd.Activate();
    }

    return true;
}

/// \ru Позиционирование главного окна. \en Positioning the main window.
QT_FUNC(void) setWindowPosition(QWidget& widget)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    const QRect availableGeometry = widget.screen()->availableGeometry();
#else
    const QRect availableGeometry = QApplication::desktop()->availableGeometry();
#endif
    widget.resize(QSize(2 * availableGeometry.width() / 3, 2 * availableGeometry.height() / 3));
    widget.move(QPoint((availableGeometry.width() - widget.width()) / 2, (availableGeometry.height() - widget.height()) / 2));
}

const QString strFilters   = QObject::tr("Files *.c3d *.sat *.obj *.step *.stp *.x_b *.jt *.JT");
const QString strFileC3D   = QObject::tr("File C3D (*.c3d)");
const QString strFileOBJ   = QObject::tr("File OBJ (*.obj)");
const QString strFileACISC = QObject::tr("File ACISC (*.sat)");
const QString strFileSTEP  = QObject::tr("File STEP (*.step)");
const QString strFileStp   = QObject::tr("File STEP (*.stp)");
const QString strFilePARASOLID = QObject::tr("File PARASOLID (*.x_b)");
const QString strFileJT    = QObject::tr("File JT (*.jt *.JT)");
const QString strFileStl   = QObject::tr("File stl (*.stl *.STL)");

QT_FUNC(QStringList) openSaveFilters()
{
    QStringList filters;
    filters.append(strFilters);
    filters.append(strFileC3D);
    filters.append(strFileOBJ);
    filters.append(strFileACISC);
    filters.append(strFileSTEP);
    filters.append(strFileStp);
    filters.append(strFilePARASOLID);
    filters.append(strFileJT);
    filters.append(strFileStl);
    return filters;
}

} // namespace QtVision
VSN_END_NAMESPACE
