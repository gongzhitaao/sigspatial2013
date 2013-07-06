#include "widget.h"
#include "ui_widget.h"

#include <QMouseEvent>
#include <QStatusBar>
#include <QGridLayout>

#include "drawingarea.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    drawingArea = new DrawingArea;

    statusBar = new QStatusBar;
    statusBar->resize(800, 20);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setRowStretch(0, 1);
    mainLayout->setRowStretch(1, 0);
    mainLayout->addWidget(drawingArea);
    mainLayout->addWidget(statusBar);

    setLayout(mainLayout);
    setMouseTracking(true);
}

Widget::~Widget()
{
    delete ui;
    delete drawingArea;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    statusBar->showMessage(QString("%1, %2")
                           .arg(event->pos().x()).arg(event->pos().y()));
}

void Widget::mousePressEvent(QMouseEvent *)
{

}
