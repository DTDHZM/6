#include "point.h"
#include <QPoint>
#include <QPainter>
#include <QColor>

Point::Point() {}

void Point::paint(QPainter &painter)
{
    painter.setPen(strokeColor);
    painter.drawPoint(start);
}

bool Point::hitTest(const QPoint &point) const
{
    return QLineF(start, point).length() < 5.0;
}
