#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>

class DrawingArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    QSizePolicy sizePolicy() const;
    
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
    
};

#endif // DRAWINGAREA_H
