///////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru .
         \en .\~
*/
///////////////////////////////////////////////////////////////////////////////
#ifndef __QT_HEADSUPTOOLBAR_H
#define __QT_HEADSUPTOOLBAR_H

#include <QWidget>
#include <QToolBar>
#include <qt_visiondef.h>

/** \brief \ru ВНИМАНИЕ! Этот файл не является частью API Vision. Он необходим для иллюстрации использования
               ядра Vision с библиотекой Qt и ее классами. Этот заголовочный файл может изменяться от версии
               к версии без предупреждения или полностью удаляться.
           \en WARNING! The file is not a part of API Vision. It is needed to illustrate how to use the Vision kernel
               with Qt library and its classes. This header file can be changed from a version to a version with
               no warning or completely deleted.\~
*/

VSN_BEGIN_NAMESPACE

namespace QtVision {

/* QtAutoHideWindow */
class QT_CLASS QtAutoHideWindow : public QWidget
{
    Q_OBJECT
public:
    explicit QtAutoHideWindow(QWidget* pParent = nullptr);
    virtual ~QtAutoHideWindow();
public:
    QToolBar* toolBar() const { return m_pToolBar; }
    void setToolBar(QToolBar* pToolBar);
public:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
protected:
    void reposition();
protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
protected Q_SLOTS:
    void allowedAreasChanged(Qt::ToolBarAreas allowedAreas);
private:
    QToolBar* m_pToolBar;
private:
    Q_DISABLE_COPY(QtAutoHideWindow);
};

} // namespace QtVision

VSN_END_NAMESPACE

#endif /* __QT_HEADSUPTOOLBAR_H */
