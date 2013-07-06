#include "drawingarea.h"

DrawingArea::DrawingArea(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);
}

QSize DrawingArea::sizeHint() const
{
    return QSize(600, 400);
}

QSize DrawingArea::minimumSizeHint() const
{
    return QSize(400, 360);
}

QSizePolicy DrawingArea::sizePolicy() const
{
    return QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void DrawingArea::paintEvent(QPaintEvent * /* event */)
{

}
