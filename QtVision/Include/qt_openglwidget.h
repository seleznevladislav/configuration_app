////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Класс QtOpenGLWidget реализует интеграцию C3D Vision c Qt. Представленные классы 
             необходимы для рендеринга графики Vision с помощью OpenGL.
         \en QtOpenGLWidget class implements integration of C3D Vision with Qt.
             The presented classes are necessary to render graphics with OpenGL. \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __OPENGLWIDGET_H
#define __OPENGLWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QGLWidget>
#include <QOpenGLWidget>
#include <QSurfaceFormat>

#include <vsn_vision.h>
#include <vsn_application.h>
#include <vsn_meshgeometry.h>
#include <vsn_graphicsview.h>
#include <vsn_graphicssceneengine.h>
#include <vsn_objectpickselection.h>
#include <vsn_openglcontextinterface.h>
#include <qt_cursors.h>
#include <qt_visiondef.h>
#include <qt_headsuptoolbar.h>

VSN_BEGIN_NAMESPACE

/** \brief \ru ВНИМАНИЕ! Этот файл не является частью API Vision. Он необходим для иллюстрации использования 
               ядра Vision с библиотекой Qt и ее классами. Этот заголовочный файл может изменяться от версии 
               к версии без предупреждения или полностью удаляться.
           \en WARNING! The file is not a part of API Vision. It is needed to illustrate how to use the Vision kernel
               with Qt library and its classes. This header file can be changed from a version to a version with
               no warning or completely deleted.\~
*/

namespace QtVision {

class QtAboutSceneWidget;
class QtOpenGLWidgetPrivate;
//------------------------------------------------------------------------------
/** \brief \ru Класс QtOpenGLWidget реализует интеграцию C3D Vision c Qt.
           \en QtOpenGLWidget class implements integration of C3D Vision with Qt. \~
    \details \ru QtOpenGLWidget необходим для работы C3D Vision с другими классами библиотеки Qt. Например,
                 как самостоятельный виджет или может быть применен в качестве viewport для QGraphicsView
                 и QGraphicsScene. Vision внутри работает с собственным представлением контекста OpenGl 
                 (OpenGLContextInterface) и класс QtOpenGLWidget содержит некоторые оболочки, обеспечивающие 
                 совместную работу QOpenGLContext и OpenGLContextInterface.
             \en QtOpenGLWidget is needed for C3D Vision to operate with other classes of the Qt library,
                 for example, as an independent widget or can be applied as the viewport for both QGraphicsView
                 and QGraphicsScene. C3D Vision inside operates with its own representation of OpenGL context
                 (OpenGLContextInterface), and QtOpenGLWidget class contains certain shells providing the ability
                 for QtOpenGLWidget and OpenGLContextInterface to operate jointly. \n \~
    \ingroup Vision_OpenGL
*/
// ---
class QT_CLASS QtOpenGLWidget : public QOpenGLWidget, public Object
{
    Q_OBJECT
    VSN_OBJECT(QtOpenGLWidget)
public:
    /// \ru Конструктор по умолчанию. \en Default constructor. \~
    explicit QtOpenGLWidget(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    /// \ru Деструктор OpenGLWidget освобождает созданные ресурсы. \en OpenGLWidget destructor releases created resources. \~
    virtual ~QtOpenGLWidget();
public:
    /**
    \brief \ru Подготовка OpenGL для рендеринга в этом виджете (установить текущий OpenGL контекст).
           \en Prepares OpenGL for rendering in the widget. Sets the current OpenGL context.
        \detaild \ru Нет необходимости вызывать эту функцию в большинстве случаев, потому что она 
                     вызывается автоматически перед вызовом paintGL().
                 \en There is no need to call this function in most cases, as it is called automatically
                     before calling paintGL().
    */
    using QOpenGLWidget::makeCurrent;
    void makeCurrent();
    /**
    \brief \ru Освободить OpenGL контекст.
           \en Releases OpenGL context.
        \detailed \ru Нет необходимости вызывать эту функцию в большинстве случаев, поскольку
                      в функции paintGL() контекст будет назначен и освобожден.
                  \en There is no need to call this function in most cases, as there will be the context set and
                      released in paintGL() function.
    */
    using QOpenGLWidget::doneCurrent;
    void doneCurrent();

    /// \ru Вернуть указатель на OpenGLContextInterface. \en Returns a pointer to OpenGLContextInterface. \~
    OpenGLContextInterface* contextInterface();

    /// \ru Нарисовать сцену и сохранить в картинку. \en NO TRANSLATION. \~
    QImage grabFrameImage(const GraphicsView* pGraphicsView, const SnapshotSettings& settings);
public Q_SLOTS:
    /// \ru Показать сайт компании. \en Show the company website. \~
    void showCompanyWebSite();
public:
    /// \ru Просто вызывает функцию QWidget::update(). Служит для совместимости слотов.
    /// \en Just calls QWidget::update() function. Serves for compatibility of slots. \~
    VSN_SLOT(Public, updateWidget, void updateWidget())
    /// \ru Просто вызывает функцию QWidget::repaint(). Служит для совместимости слотов.
    /// \en Just calls QWidget::repaint() function. Serves for compatibility of slots. \~
    VSN_SLOT(Public, repaintWidget, void repaintWidget())
protected:
    /// \ru Инициализация OpenGL ресурсов. \en Initialises OpenGL resources. \~
    virtual void initializeGL() override;
protected:
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
protected:
    QtOpenGLWidget(QtOpenGLWidgetPrivate& dd, QWidget* parent, Qt::WindowFlags f);
    VSN_DECLARE_PRIVATE(QtOpenGLWidget);
private:
    Q_DISABLE_COPY(QtOpenGLWidget);
};

class QtAutoHideWindow;
class QtOpenGLSceneWidgetPrivate;
//------------------------------------------------------------------------------
/** \brief \ru Класс QtOpenGLSceneWidget - это виджет для рендеринга графики Vision с помощью OpenGL.
           \en QtOpenGLSceneWidget is a widget for rendering C3D Vision graphics with OpenGL. \~
    \details \ru Класс QtOpenGLSceneWidget полностью самостоятельный, создает и инициализирует 
                 в полной мере компоненты Vision. Этот класс представляет необходимые функции для работы с Vision. 
                 Класс QtOpenGLSceneWidget самостоятельно устанавливает текущий контест OpenGL, когда это необходимо. 
             \en QtOpenGLSceneWidget class is fully autonomous, so it completely creates and initialises
                 C3D Vision components. This class provides required functions to work with C3D Vision.
                 QtOpenGLSceneWidget class independently sets the current OpenGL context when necessary. \n \~
    \ingroup Vision_OpenGL
*/
// ---
class QT_CLASS QtOpenGLSceneWidget : public QtOpenGLWidget
{
    Q_OBJECT
    VSN_OBJECT(QtOpenGLSceneWidget)
public:
    /// \ru Конструктор по умолчанию. \en Default constructor. \~
    explicit QtOpenGLSceneWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    /// \ru Специальный конструктор. \en Special constructor. \~
    explicit QtOpenGLSceneWidget(GraphicsSceneEnginePtr engine, OpenGLContextContainer* sharedContainer, 
                QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    /// \ru Деструктор OpenGLWidget освобождает созданные ресурсы. \en OpenGLWidget destructor releases created resources. \~
    virtual ~QtOpenGLSceneWidget();
public:
    /// \ru Вернуть указатель на источник освещения сцены. \en Returns a pointer to the lighting source in the scene. \~
    Light* mainLight() const;
    /// \ru Вернуть указатель на камеру. \en Returns a pointer to the camera. \~
    Camera* camera() const;
    /// \ru Вернуть указатель на Viewport для отображения сцены. \en Returns a pointer to Viewport for displaying the scene. \~
    Viewport* viewport() const;
    /// \ru Вернуть указатель на содержимое сцены. \en Returns a pointer to the scene content. \~
    SceneContentPtr sceneContent();
    /// \ru Вернуть указатель на графическую сцену. \en Returns a pointer to the graphic scene. \~
    GraphicsScene* graphicsScene() const;
    /// \ru Вернуть указатель на компонент выбора объектов. \en Returns a pointer to the item of the object selection. \~
    ObjectPickSelection* objectPickSelection() const;
    /// \ru Вернуть указатель на GraphicsSceneEngine. \en Returns a pointer to GraphicsSceneEngine. \~
    GraphicsSceneEnginePtr graphicsEngine() const;
    /// \ru Вернуть указатель на GraphicsView. \en Returns a pointer to GraphicsView. \~
    GraphicsViewPtr ptrGraphicsView() const;
    /// \ru Вернуть указатель на GraphicsView. \en Returns a pointer to GraphicsView. \~
    GraphicsView* graphicsView() const;
public:
    void setAboutSceneWidget(QtAboutSceneWidget* pAboutScene);
    QtAutoHideWindow* createHeadToolbar();
public Q_SLOTS:
    void viewPortCommands();
public:
    /// \ru Масштабировать модель до размера viewport по габариту модели. \en Zooms the model to ensure the bounding box of the model fits inside the viewport. \~
    VSN_SLOT(Public, ZoomToFit, void ZoomToFit())
protected:
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent* event) override;
    virtual void enterEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void focusInEvent(QFocusEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual bool OnEvent(ProcessEvent* event);
protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void closeEvent(QCloseEvent* event) override;
private:
    Q_DISABLE_COPY(QtOpenGLSceneWidget);
    VSN_DECLARE_EX_PRIVATE(QtOpenGLSceneWidget);
    friend class QtRenderSelection;
};

enum ProcessType
{
    pt_Pan             = 0x01, ///< \ru Инициализация процесса параномирования. \en Initializes the pan process. \~
    pt_Orbit           = 0x02, ///< \ru Инициализация процесса вращения вокруг орбиты. \en Initializes the process of rotation about the orbit. \~
    pt_Rotate          = 0x04, ///< \ru Инициализация процесса вращения вокруг центра габарита. \en Initialization the process of rotation about the center of the boundbox. \~
    pt_ComplexRotation = 0x08, ///< \ru Инициализация процесса вращения вокруг оси. \en Initialization the process of rotation about the axis. \~
    pt_Zoom            = 0x10, ///< \ru Инициализация процесса масштабирования. \en Initializes the zoom process. \~
    pt_AllProcess = pt_Orbit | pt_Zoom | pt_Pan,
};
VSN_DECLARE_FLAGS(ProcessTypes, ProcessType)

//static Color topColor    = Color(174, 174, 174);
//static Color bottomColor = Color(250, 250, 250);

static Color topColor = Color(250, 250, 250);
static Color bottomColor = Color(174, 174, 174);

/// \ru Создание процессов для управления камерой. \en Creates processes to control the camera. \~
QT_FUNC(void) createProcessesCameraControls(QtOpenGLSceneWidget* pGLWidget, 
    ProcessTypes prType = pt_AllProcess, SceneSegment* pParent = nullptr);

/// \ru Установка формата для QSurface. \en Sets the format for QSurface. \~
QT_FUNC(void) setSurfaceFormat();
/// \ru Проверка лицензии. \en License check.
QT_FUNC(bool) activateLicense();
/// \ru Позиционирование главного окна. \en Positioning the main window.
QT_FUNC(void) setWindowPosition(QWidget& widget);
//
QT_FUNC(QStringList) openSaveFilters();

} // namespace QtVision

VSN_END_NAMESPACE

#endif // __OPENGLWIDGET_H
