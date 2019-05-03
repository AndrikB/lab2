#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QApplication>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lyPainter->addWidget(&paintwidget);
    QSize display=QApplication::desktop()->screenGeometry().size();

    ui->count_items_spin->setMaximum(qMin(display.width()/2, display.height()-200));
    ui->count_items_slider->setMaximum(qMin(display.width()/2, display.height()-200));

    on_shuffle_btn_clicked();

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_shuffle_btn_clicked()
{
    qDebug()<<"start shuffle";
    if (array)
    {
        delete array;
    }
    size=ui->count_items_spin->value();
    array=new int [static_cast<unsigned long long>(size)];

    for (int i=0;i<size;i++)
    {
        array[i]=i+1;
    }

    for (int i=0; i <size; ++i)
    {
        int rand = QRandomGenerator::global()->bounded(0,size);
        swap(i, rand);
    }
    qDebug()<<"end shuffle";

    paintwidget.visualize(size, array, 2,50);

    //todo sort
}

void MainWindow::swap(int i, int j)
{
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

void MainWindow::on_next_btn_clicked()
{
    read_iteration(next);
    paintwidget.visualize(size, array);
}

void MainWindow::on_back_btn_clicked()
{
    read_iteration(prev);

    paintwidget.visualize(size, array);
}

void MainWindow::read_iteration(MainWindow::iteration next_or_prev)//todo
{
    if (next_or_prev==next)
    {

    }

}


