#include "line.h"
#include <QPainter>
#include <QLineF>
#include <cmath> // 需要包含数学库

Line::Line() {}

void Line::paint(QPainter &painter)
{
    painter.setPen(strokeColor);
    painter.drawLine(start, end);
}

// 手动实现点到线段的距离计算
static qreal distanceToLine(const QPointF &point, const QLineF &line)
{
    qreal length = line.length();
    if (length == 0.0) {
        // 如果线段长度为0，直接计算点到端点的距离
        QLineF tmp(line.p1(), point);
        return tmp.length();
    }

    // 计算点到线段起点的向量在直线方向上的投影长度
    qreal t = ((point.x() - line.x1()) * (line.x2() - line.x1()) +
               (point.y() - line.y1()) * (line.y2() - line.y1())) /
              (length * length);

    // 约束投影在 [0, 1] 范围内，确保点在直线上
    t = qBound(0.0, t, 1.0);

    // 找到直线上最近的点
    QPointF closest(line.x1() + t * (line.x2() - line.x1()),
                    line.y1() + t * (line.y2() - line.y1()));

    // 计算两点之间的距离
    QLineF tmp(point, closest);
    return tmp.length();
}

bool Line::hitTest(const QPoint &point) const
{
    QLineF line(start, end);
    return distanceToLine(QPointF(point), line) < 5.0; // 使用手动实现的函数
}
