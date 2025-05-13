#include "paintwidget.h"
#include "shape.h"
#include "point.h"
#include "line.h"
#include "rect.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    currShapeCode = Shape::None;
    selectedShape = nullptr;
    currentShape = nullptr;
    isDrawing = false;
    selectedControlPoint = -1;
}

void PaintWidget::setCurrentShape(Shape::Code s)
{
    currShapeCode = s;
    selectedShape = nullptr; // 切换模式时取消选择
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    foreach (Shape *shape, shapes) {
        shape->paint(painter);
    }

    if (currentShape && isDrawing) {
        currentShape->paint(painter);
    }

    // 绘制选中的图形的控制点
    if (selectedShape) {
        painter.setPen(Qt::blue);
        if (Line* line = dynamic_cast<Line*>(selectedShape)) {
            painter.drawEllipse(line->startPoint(), 3, 3);
            painter.drawEllipse(line->endPoint(), 3, 3);
        } else if (Rect* rect = dynamic_cast<Rect*>(selectedShape)) {
            QPoint p1 = rect->startPoint();
            QPoint p2(rect->endPoint().x(), rect->startPoint().y());
            QPoint p3 = rect->endPoint();
            QPoint p4(rect->startPoint().x(), rect->endPoint().y());
            painter.drawEllipse(p1, 3, 3);
            painter.drawEllipse(p2, 3, 3);
            painter.drawEllipse(p3, 3, 3);
            painter.drawEllipse(p4, 3, 3);
        } else if (Point* point = dynamic_cast<Point*>(selectedShape)) {
            painter.drawEllipse(point->startPoint(), 3, 3);
        }
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (currShapeCode == Shape::None) {
            // 选择模式
            selectedShape = findShapeAtPosition(event->pos());
            if (selectedShape) {
                // 确定选中的控制点
                if (Line* line = dynamic_cast<Line*>(selectedShape)) {
                    QPoint start = line->startPoint();
                    QPoint end = line->endPoint();
                    if (QLineF(event->pos(), start).length() < 5) {
                        selectedControlPoint = 1;
                    } else if (QLineF(event->pos(), end).length() < 5) {
                        selectedControlPoint = 2;
                    } else {
                        selectedControlPoint = 0;
                    }
                } else if (Rect* rect = dynamic_cast<Rect*>(selectedShape)) {
                    QPoint p1 = rect->startPoint();
                    QPoint p2(rect->endPoint().x(), rect->startPoint().y());
                    QPoint p3 = rect->endPoint();
                    QPoint p4(rect->startPoint().x(), rect->endPoint().y());
                    if (QLineF(event->pos(), p1).length() < 5) {
                        selectedControlPoint = 1;
                    } else if (QLineF(event->pos(), p2).length() < 5) {
                        selectedControlPoint = 2;
                    } else if (QLineF(event->pos(), p3).length() < 5) {
                        selectedControlPoint = 3;
                    } else if (QLineF(event->pos(), p4).length() < 5) {
                        selectedControlPoint = 4;
                    } else {
                        selectedControlPoint = 0;
                    }
                } else if (Point* point = dynamic_cast<Point*>(selectedShape)) {
                    selectedControlPoint = 1; // 只有一点可拖动
                }
                lastPos = event->pos();
                isDrawing = true;
                update();
            }
        } else {
            // 绘图模式
            switch (currShapeCode) {
            case Shape::Line:
                currentShape = new Line;
                break;
            case Shape::Rect:
                currentShape = new Rect;
                break;
            case Shape::Point:
                currentShape = new Point;
                break;
            default:
                break;
            }
            if (currentShape) {
                isDrawing = true;
                currentShape->setStart(event->pos());
                currentShape->setEnd(event->pos());
                update();
            }
        }
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (isDrawing) {
        if (currShapeCode == Shape::None && selectedShape) {
            // 处理拖动或缩放
            QPoint delta = event->pos() - lastPos;
            if (Line* line = dynamic_cast<Line*>(selectedShape)) {
                switch (selectedControlPoint) {
                case 1:
                    line->setStart(line->startPoint() + delta);
                    break;
                case 2:
                    line->setEnd(line->endPoint() + delta);
                    break;
                case 0:
                    line->setStart(line->startPoint() + delta);
                    line->setEnd(line->endPoint() + delta);
                    break;
                }
            } else if (Rect* rect = dynamic_cast<Rect*>(selectedShape)) {
                QPoint start = rect->startPoint();
                QPoint end = rect->endPoint();
                switch (selectedControlPoint) {
                case 1: // 拖动左上角
                    start += delta;
                    break;
                case 2: // 拖动右上角
                    end.setX(end.x() + delta.x());
                    start.setY(start.y() + delta.y());
                    break;
                case 3: // 拖动右下角
                    end += delta;
                    break;
                case 4: // 拖动左下角
                    start.setX(start.x() + delta.x());
                    end.setY(end.y() + delta.y());
                    break;
                case 0: // 移动整个矩形
                    start += delta;
                    end += delta;
                    break;
                }
                rect->setStart(start);
                rect->setEnd(end);
            } else if (Point* point = dynamic_cast<Point*>(selectedShape)) {
                point->setStart(point->startPoint() + delta);
                point->setEnd(point->endPoint() + delta);
            }
            lastPos = event->pos();
            update();
        } else if (currentShape) {
            // 绘图模式下的拖动
            currentShape->setEnd(event->pos());
            update();
        }
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (currShapeCode == Shape::None) {
            // 结束拖动/缩放
            isDrawing = false;
            selectedControlPoint = -1;
        } else if (currentShape) {
            // 结束绘图
            isDrawing = false;
            if (dynamic_cast<Line*>(currentShape)) {
                lines.append(currentShape);
            } else if (dynamic_cast<Rect*>(currentShape)) {
                rects.append(currentShape);
            } else if (dynamic_cast<Point*>(currentShape)) {
                points.append(currentShape);
            }
            shapes.append(currentShape);
            currentShape = nullptr;
        }
        update();
    }
}

Shape* PaintWidget::findShapeAtPosition(const QPoint &pos)
{
    // 逆序检查，后绘制的图形在上层
    for (int i = shapes.size() - 1; i >= 0; --i) {
        if (shapes[i]->hitTest(pos)) {
            return shapes[i];
        }
    }
    return nullptr;
}

void PaintWidget::saveShapes(const QString &filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        // 保存点
        for (Shape* shape : points) {
            QPoint pos = shape->startPoint();
            QColor color = shape->getStrokeColor();
            out << "POINT " << pos.x() << "," << pos.y() << ","
                << color.red() << "," << color.green() << "," << color.blue() << "\n";
        }

        // 保存线段
        for (Shape* shape : lines) {
            QPoint start = shape->startPoint();
            QPoint end = shape->endPoint();
            QColor color = shape->getStrokeColor();
            out << "LINE " << start.x() << "," << start.y() << ","
                << end.x() << "," << end.y() << ","
                << color.red() << "," << color.green() << "," << color.blue() << "\n";
        }

        // 保存矩形
        for (Shape* shape : rects) {
            Rect* rect = dynamic_cast<Rect*>(shape);
            if (rect) {
                QPoint start = rect->startPoint();
                QPoint end = rect->endPoint();
                QPoint p1 = start;
                QPoint p2(end.x(), start.y());
                QPoint p3 = end;
                QPoint p4(start.x(), end.y());
                QColor strokeColor = rect->getStrokeColor();
                QColor fillColor = rect->getFillColor();
                out << "RECT "
                    << p1.x() << "," << p1.y() << ","
                    << p2.x() << "," << p2.y() << ","
                    << p3.x() << "," << p3.y() << ","
                    << p4.x() << "," << p4.y() << ","
                    << strokeColor.red() << "," << strokeColor.green() << "," << strokeColor.blue() << ","
                    << fillColor.red() << "," << fillColor.green() << "," << fillColor.blue() << "\n";
            }
        }

        file.close();
    }
}

void PaintWidget::loadShapes(const QString &filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 清空原有数据
        qDeleteAll(shapes);
        shapes.clear();
        lines.clear();
        rects.clear();
        points.clear();

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" ");
            if (parts.isEmpty()) continue;

            QString type = parts[0];
            QStringList data = parts[1].split(",");

            if (type == "POINT" && data.size() == 5) {
                int x = data[0].toInt();
                int y = data[1].toInt();
                int r = data[2].toInt();
                int g = data[3].toInt();
                int b = data[4].toInt();
                Point* point = new Point();
                point->setStart(QPoint(x, y));
                point->setEnd(QPoint(x, y));
                point->setStrokeColor(QColor(r, g, b));
                points.append(point);
                shapes.append(point);
            } else if (type == "LINE" && data.size() == 7) {
                int x1 = data[0].toInt();
                int y1 = data[1].toInt();
                int x2 = data[2].toInt();
                int y2 = data[3].toInt();
                int r = data[4].toInt();
                int g = data[5].toInt();
                int b = data[6].toInt();
                Line* line = new Line();
                line->setStart(QPoint(x1, y1));
                line->setEnd(QPoint(x2, y2));
                line->setStrokeColor(QColor(r, g, b));
                lines.append(line);
                shapes.append(line);
            } else if (type == "RECT" && data.size() == 14) {  // 修正数据长度
                int x1 = data[0].toInt();
                int y1 = data[1].toInt();
                int x2 = data[2].toInt();
                int y2 = data[3].toInt();
                int x3 = data[4].toInt();
                int y3 = data[5].toInt();
                int x4 = data[6].toInt();
                int y4 = data[7].toInt();
                int sr = data[8].toInt();
                int sg = data[9].toInt();
                int sb = data[10].toInt();
                int fr = data[11].toInt();
                int fg = data[12].toInt();
                int fb = data[13].toInt();
                Rect* rect = new Rect();
                rect->setStart(QPoint(x1, y1));
                rect->setEnd(QPoint(x3, y3));
                rect->setStrokeColor(QColor(sr, sg, sb));
                rect->setFillColor(QColor(fr, fg, fb));
                rects.append(rect);
                shapes.append(rect);
            }
        }
        file.close();
        update();
    }
}
