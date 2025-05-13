#ifndef SHAPE_H
#define SHAPE_H

#include <QPoint>
#include <QPainter>
#include <QColor>

class Shape
{
public:
    enum Code { Line, Rect, Point, None }; // 添加Point和None

    Shape();
    virtual ~Shape() {}

    void setStart(QPoint s) { start = s; }
    void setEnd(QPoint e) { end = e; }
    QPoint startPoint() const { return start; }
    QPoint endPoint() const { return end; }

    void setStrokeColor(const QColor &color) { strokeColor = color; }
    QColor getStrokeColor() const { return strokeColor; }
    void setFillColor(const QColor &color) { fillColor = color; }
    QColor getFillColor() const { return fillColor; }

    virtual void paint(QPainter &painter) = 0;
    virtual bool hitTest(const QPoint &point) const = 0;
protected:
    QPoint start;
    QPoint end;
    QColor strokeColor = Qt::black;
    QColor fillColor = Qt::transparent;
};

#endif // SHAPE_H
