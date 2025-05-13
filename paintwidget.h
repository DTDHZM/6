#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QList>
#include "shape.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);
    void saveShapes(const QString &filename);
    void loadShapes(const QString &filename);

public slots:
    void setCurrentShape(Shape::Code s);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Shape::Code currShapeCode = Shape::None;
    Shape* currentShape = nullptr;
    bool isDrawing = false;
    QList<Shape*> shapes;
    QList<Shape*> lines;
    QList<Shape*> rects;
    QList<Shape*> points;

    // 选择相关变量
    Shape* selectedShape = nullptr;
    int selectedControlPoint = -1; // -1表示未选中，0表示移动整个图形，其他值表示具体控制点
    QPoint lastPos;

    Shape* findShapeAtPosition(const QPoint &pos);
};

#endif // PAINTWIDGET_H
