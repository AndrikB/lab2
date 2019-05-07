#include "paintwidget.h"
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include "mainwindow.h"

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    QPalette palette;
    palette.setColor(QPalette::Background,defaultBgColor);
    setPalette(palette);
    setAutoFillBackground(true);

    pen=QPen(defaultLineColor);
}

PaintWidget::~PaintWidget()
{

}

void PaintWidget::visualize(int size, int *array, int red, int yelow)//todo
{
    this->size=size;
    this->array=array;
    this->red=red;
    this->yelow=yelow;
    //QSize display=QApplication::desktop()->screenGeometry().size();

    this->setMinimumHeight(qMax(size, 50));
    this->setMinimumWidth(size*2+1);



    this->update();

}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    double penWidth=1.0*this->width()/size-1;
    pen.setWidth(1);

    for (int i=0;i<size;i++)
    {
        pen.setColor(defaultLineColor);


        if (i==red)
            pen.setColor(Qt::red);

        else if (i==yelow)
            pen.setColor(Qt::yellow);

        painter.setPen(pen);
        for (int j=1;j<penWidth;j++)
        {
            painter.drawLine(static_cast<int>((i)*(penWidth+1)+j), this->height(),
                              static_cast<int>((i)*(penWidth+1)+j), static_cast<int>((1.0- 1.0*array[i]/size)*this->height()));
        }

    }

    static_cast<MainWindow*>(parent()->parent())->change_display_location();
}
