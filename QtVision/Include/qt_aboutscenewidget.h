////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Класс QtAboutSceneWidget является виджетом, который отображается в рабочей области экрана в левом верхнем углу. 
             QtAboutSceneWidget отображает отрисовочную статистику о сцене, например, Fps, количество рисуемых тел и
             количество треугольников.
         \en QtAboutSceneWidget class is a widget that is displayed in the working area of the screen at the top left corner.
             The class displayes such scene rendering statistic elements as FPS, the number of bodies rendered and of triangles. \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __ABOUTSCENEWIDGET_H
#define __ABOUTSCENEWIDGET_H

#include <QObject>
#include <QPainter>
#include <QTime>
#include <QEvent>
#include <vsn_renderstatistics.h>
#include <qt_visiondef.h>

VSN_BEGIN_NAMESPACE

/** \brief \ru ВНИМАНИЕ! Этот файл не является частью API Vision. Он необходим для иллюстрации использования 
               ядра Vision с библиотекой Qt и ее классами. Этот заголовочный файл может изменяться от версии 
               к версии без предупреждения или полностью удаляться.
           \en WARNING! The file is not a part of API Vision. It is needed to illustrate how to use the Vision kernel
               with Qt library and its classes. This header file can be changed from a version to a version with
               no warning or completely deleted.\~
*/

namespace QtVision {


//------------------------------------------------------------------------------
/** \brief \ru Класс QtAboutSceneWidgetItem является пунктом для вывода строки информации в QtAboutSceneWidget.
           \en QtAboutSceneWidgetItem class is an item to hold a singe information row as a string to output in QtAboutSceneWidget class. \~
    \ingroup Vision_OpenGL
*/
// ---
class QtAboutSceneWidgetItem
{
public:
    /// \ru Конструктор по умолчанию. \en Default constructor. \~
    explicit QtAboutSceneWidgetItem();
    /// \ru Деструктор. \en Destructor. \~
    ~QtAboutSceneWidgetItem();
public:
    /// \ru Вернуть текст элемента. \en Returns the item text. \~
    inline QString text() const;
    /// \ru Установить текст элемента для последующей отрисовки. \en Sets the item text with the following rendering of the text. \~
    inline void setText(const QString& text);

    /// \ru Вернуть установленный шрифт элемента. \en Returns the assigned item font. \~
    inline QFont font() const;
    /// \ru Установить шрифт элемента для отрисовки. \en Sets the item font for rendering. \~
    inline void setFont(const QFont& font);

    /// \ru Вернуть размер всего виджета с учетом назначенных строк. \en Returns the size of the entire widget including all assigned strings. \~
    inline QSize sizeHint() const;
protected:
    QString m_text; ///< \ru Текст для отрисовки статистики. \en The property holds the text for rendering the statistics. \~
    QFont m_font;
};

/// \ru Вернуть текст элемента. \en Returns the item text. \~
inline QString QtAboutSceneWidgetItem::text() const
{ return m_text; }

/// \ru Установить текст элемента для последующей отрисовки. \en Sets the item text with the following rendering of the text. \~
inline void QtAboutSceneWidgetItem::setText(const QString& text)
{ m_text = text; }

/// \ru Вернуть установленный шрифт элемента. \en Returns the assigned item font. \~
inline QFont QtAboutSceneWidgetItem::font() const
{ return m_font; }

/// \ru Установить шрифт элемента для отрисовки. \en Sets the item font for rendering. \~
inline void QtAboutSceneWidgetItem::setFont(const QFont& font)
{ m_font = font; }

/// \ru Вернуть размер всего виджета с учетом назначенных строк. \en Returns the size of the entire widget including all assigned strings. \~
inline QSize QtAboutSceneWidgetItem::sizeHint() const
{
    if (m_text.isEmpty())
        return QSize();
    QFontMetrics fm(m_font);
    return fm.boundingRect(m_text).size();
}


class QtOpenGLSceneWidget;
//------------------------------------------------------------------------------
/** \brief \ru Класс QtAboutSceneWidget является виджетом, который отображается в рабочей области экрана в левом верхнем углу.
               QtAboutSceneWidget отображает отрисовочную статистику о сцене, например, Fps, количество рисуемых тел и 
               количество треугольников.
           \en QtAboutSceneWidget class is a widget that is displayed in the working area of the screen at the top left corner.
               The class displayes such scene rendering statistic elements as FPS, the number of bodies rendered and of triangles. \~
    \ingroup Vision_OpenGL
*/
// ---
class QT_CLASS QtAboutSceneWidget : public QObject
{
    Q_OBJECT
public:
    /// \ru Конструктор по умолчанию. \en Default constructor. \~
    explicit QtAboutSceneWidget(QWidget* pParent);
    /// \ru Деструктор. \en Destructor. \~
    virtual ~QtAboutSceneWidget();
public:
    enum InfoItem
    {
        iRenderFps       = 0,  ///< \ru Строка количества кадров в секунду при отрисовке всей сцены. \en The string contains the information about frames per second when rendering the entire scene. \~
        iRenderBodies    = 1,  ///< \ru Строка количества тел при отрисовке всей сцены. \en The string contains the information about the number of all bodies in the scene. \~
        iRenderTriangles = 2,  ///< \ru Строка количества треугольников при отрисовке всей сцены. \en The string contains the information about the number of all triangles in the scene. \~
        iLoadedBodies    = 3,  ///< \ru Строка количества тел при отрисовке всей сцены. \en The string contains the information about the number of all bodies in the scene. \~
        iCount,                ///< \ru Количество строк всего для вывода таблицы. \en The member holds the total number of strings for outputting the table. \~
    };
public:
    /// \ru Установить количество строк. \en Sets the string number. \~
    void setRowCount(int rows);
    /// \ru Вернуть указатель на Item. \en Returns a pointer to the item. \~
    QtAboutSceneWidgetItem* item(int row, int col) const;
public:
    /// \ru Вернуть размер всего информационного виджета. \en Returns the size of the informative widget. \~
    QSize sizeHint() const;
    /// \ru Установить позицию виджета на рабочем экране. \en Sets the position of the widget in the working screen. \~
    void setPosition(const QPoint& pnt);
    /// \ru Отрисовать запрошенную информацию на рабочем экране. \en Paints the requested statistics in the working screen. \~
    void paintStatistics(QPainter* p);
    /// \ru Обновить информацию для отрисовки на рабочем экране. \en Updates the statistics to repaint in the working screen. \~
    void updateStatistics();
protected:
    void createRowItems();
    void clearRowItems();
    void initTableContents();
    void calcMaxHeightWidth();
protected:
    virtual bool event(QEvent* event) override;
private:
    bool m_dirty;
    int m_maxHeightItem;
    int m_maxFirstColumnWidth;
    int m_maxSecondColumnWidth;

    int m_frames;
    int m_countTest;
    qreal m_fAngle;

    QSize m_sizeHint;
    QRect m_rect;
    QFont m_font;
    QTime m_time;
    QList<QtAboutSceneWidgetItem*> m_firstColumnChildren;
    QList<QtAboutSceneWidgetItem*> m_secondColumnChildren;
private:
    Q_DISABLE_COPY(QtAboutSceneWidget);
};


} // namespace QtVision

VSN_END_NAMESPACE

#endif // __ABOUTSCENEWIDGET_H
