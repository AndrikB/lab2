#include "paintwidget.h"
#include <QDebug>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    QPalette palette;
    palette.setColor(QPalette::Background,defaultBgColor);
    setPalette(palette);
    setAutoFillBackground(true);

    pen=QPen(defaultLineColor, 2);
}

void PaintWidget::visualize(int size, int *array, int red, int yelow)//todo
{
    this->size=size;
    this->array=array;
    this->red=red;
    this->yelow=yelow;

    this->update();

}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int penWidth=this->width()/size;
    //pen.setWidth()
    for (int i=0;i<size;i++)
    {
        painter.setPen(pen);

        painter.drawLine((i-1)*penWidth, this->height(),
                         (i-1)*penWidth, this->height() - array[i]);
    }

}
