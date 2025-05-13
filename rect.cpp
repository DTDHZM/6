#include "rect.h"
#include <QPoint>
#include <QPainter>
#include <QColor>

Rect::Rect() {}

void Rect::paint(QPainter &painter)
{
    painter.setPen(strokeColor);
    painter.setBrush(fillColor);
    painter.drawRect(start.x(), start.y(),
                     end.x() - start.x(),
                     end.y() - start.y());
}

bool Rect::hitTest(const QPoint &point) const
{
    QRect rect(start, end);
    return rect.adjusted(-5, -5, 5, 5).contains(point);
}
