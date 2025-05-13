#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建工具栏
    QToolBar *bar = new QToolBar;
    addToolBar(bar);

    // 创建动作组
    QActionGroup *group = new QActionGroup(bar);

    // 直线动作
    QAction *drawLineAction = new QAction("Line", bar);
    drawLineAction->setIcon(QIcon(":/picture/line.png"));
    drawLineAction->setCheckable(true);
    drawLineAction->setChecked(true);

    // 矩形动作
    QAction *drawRectAction = new QAction("Rectangle", bar);
    drawRectAction->setIcon(QIcon(":/picture/rect.png"));
    drawRectAction->setCheckable(true);

    // 点动作
    QAction *drawPointAction = new QAction("Point", bar);
    drawPointAction->setIcon(QIcon(":/picture/point.png"));
    drawPointAction->setCheckable(true);

    // 选择动作
    QAction *selectAction = new QAction("Select", bar);
    selectAction->setIcon(QIcon(":/picture/select.png"));
    selectAction->setCheckable(true);

    // 添加文件菜单
    QMenu *fileMenu = menuBar()->addMenu("文件(&F)");
    QAction *openAction = new QAction("打开(&O)", this);
    QAction *saveAction = new QAction("保存(&S)", this);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);

    // 添加动作到组和工具栏
    group->addAction(drawLineAction);
    group->addAction(drawRectAction);
    group->addAction(drawPointAction);
    group->addAction(selectAction);
    bar->addAction(drawLineAction);
    bar->addAction(drawRectAction);
    bar->addAction(drawPointAction);
    bar->addAction(selectAction);

    // 初始化画板
    paintWidget = new PaintWidget(this);
    setCentralWidget(paintWidget);

    // 连接信号槽
    connect(drawLineAction, &QAction::triggered, this, &MainWindow::drawLineActionTriggered);
    connect(drawRectAction, &QAction::triggered, this, &MainWindow::drawRectActionTriggered);
    connect(drawPointAction, &QAction::triggered, this, &MainWindow::drawPointActionTriggered);
    connect(selectAction, &QAction::triggered, this, &MainWindow::selectActionTriggered);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenAction);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveAction);
    connect(this, &MainWindow::changeCurrentShape, paintWidget, &PaintWidget::setCurrentShape);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawLineActionTriggered()
{
    emit changeCurrentShape(Shape::Line);
}

void MainWindow::drawRectActionTriggered()
{
    emit changeCurrentShape(Shape::Rect);
}

void MainWindow::drawPointActionTriggered()
{
    emit changeCurrentShape(Shape::Point);
}

void MainWindow::selectActionTriggered()
{
    emit changeCurrentShape(Shape::None);
}

void MainWindow::onSaveAction()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存绘图", "", "文本文件 (*.txt)");
    if (!fileName.isEmpty()) {
        paintWidget->saveShapes(fileName);
    }
}

void MainWindow::onOpenAction()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开绘图", "", "文本文件 (*.txt)");
    if (!fileName.isEmpty()) {
        paintWidget->loadShapes(fileName);
    }
}
