#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void changeCurrentShape(Shape::Code newShape);

private slots:
    void drawLineActionTriggered();
    void drawRectActionTriggered();
    void drawPointActionTriggered();
    void selectActionTriggered();
    void onSaveAction();
    void onOpenAction();

private:
    Ui::MainWindow *ui;
    PaintWidget *paintWidget;
};

#endif // MAINWINDOW_H
