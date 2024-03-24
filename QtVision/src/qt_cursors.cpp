#include <qt_cursors.h>

#include <QPainter>
#include <QBitmap>
#include <QColor>

#include <last.h>

VSN_BEGIN_NAMESPACE

namespace QtVision {

// ---
static QPixmap paintPixmap(int size, const QColor& c)
{
    QPixmap result(size, size);
    result.fill(c);
    QPainter p(&result);
    int factor = 4;

    QPen pen(QColor(255, 255, 255, 180), 3);
    p.setPen(pen);
    // cross
    p.drawLine(size / 2, size - size / factor, size / 2, size / factor);
    p.drawLine(size / factor, size / 2, size - size / factor, size / 2);
    // top-left
    p.drawLine(0, 0, size / factor, 0);
    p.drawLine(0, 0, 0, size / factor);
    // top-right
    p.drawLine(size, 0, size - size / factor, 0);
    p.drawLine(size - 1, 0, size - 1, size / factor);
    // botton-left
    p.drawLine(0, size - 1, size / factor, size - 1);
    p.drawLine(0, size, 0, size - size / factor);
    // botton-right
    p.drawLine(size, size - 1, size - size / factor, size - 1);
    p.drawLine(size, size - 1, size - 1, size - size / factor);


    QPen penBlack(Qt::black, 1);
    p.setPen(penBlack);
    // cross
    p.drawLine(size / 2, size - size / factor, size / 2, size / factor);
    p.drawLine(size / factor, size / 2, size - size / factor, size / 2);
    // top-left
    p.drawLine(0, 0, size / factor, 0);
    p.drawLine(0, 0, 0, size / factor);
    // top-right
    p.drawLine(size, 0, size - size / factor, 0);
    p.drawLine(size - 1, 0, size - 1, size / factor);
    // botton-left
    p.drawLine(0, size - 1, size / factor, size - 1);
    p.drawLine(0, size, 0, size - size / factor);
    // botton-right
    p.drawLine(size, size - 1, size - size / factor, size - 1);
    p.drawLine(size, size - 1, size - 1, size - size / factor);
    return result;
}

// ---
QT_FUNC(QCursor) findCursor(int size)
{
    QPixmap px = paintPixmap(size, Qt::transparent);
    QCursor result(px, size / 2, size / 2);
    return result;
}

} // namespace QtVision

VSN_END_NAMESPACE
