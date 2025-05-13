#ifndef LINE_H
#define LINE_H

#include "shape.h"

class Line : public Shape
{
public:
    Line();
    void paint(QPainter &painter) override;
    bool hitTest(const QPoint &point) const override;
};

#endif // LINE_H
