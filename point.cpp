#include "point.h"
#include <QPoint>
#include <QPainter>
#include <QColor>

Point::Point() {}

void Point::paint(QPainter &painter)
{
    // 设置画笔颜色，用于绘制辅助圆圈
    QColor highlightColor = strokeColor;
    highlightColor.setAlpha(100); // 设置透明度为50（0-255）

    // 绘制半透明的辅助圆圈，扩大显示区域
    painter.setBrush(highlightColor);    // 设置填充颜色
    painter.setPen(Qt::NoPen);           // 边框不绘制
    painter.drawEllipse(start, 10, 10);  // 画一个半径为10的辅助圆圈

    // 恢复画笔颜色，用于绘制实际点
    painter.setPen(strokeColor);
    painter.setBrush(Qt::NoBrush);       // 取消填充
    painter.drawPoint(start);            // 绘制实际的点
}

bool Point::hitTest(const QPoint &point) const
{
    return QLineF(start, point).length() < 5.0;
}
