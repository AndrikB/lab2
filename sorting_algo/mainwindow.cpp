#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lyPainter->addWidget(&paintwidget);

    size=142;
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
    array=new int [static_cast<unsigned long long>(size)];

    for (int i=0;i<size;i++) //todo norm init
    {
        array[i]=i;
    }

    for (int i=0; i <size; ++i)
    {
        int rand = QRandomGenerator::global()->bounded(0,size);
        swap(i, rand);
    }
    qDebug()<<"end shuffle";

    paintwidget.visualize(size, array);

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

void MainWindow::read_iteration(MainWindow::iteration next_or_prev)//todo
{
    if (next_or_prev==next)
    {

    }

}

