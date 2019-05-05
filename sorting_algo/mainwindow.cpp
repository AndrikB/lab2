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

    f1.setFileName("filename.txt");
    ui->Algos_combobox->addItems(sorting.get_algorithms());
    ui->Algos_combobox->setCurrentIndex(0);
    connect(ui->Algos_combobox, SIGNAL(currentIndexChanged(QString)), this, SLOT(create_algo_steps(QString)));

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
    array=Sorting::shuffle(array, size);

    qDebug()<<"end shuffle";

    paintwidget.visualize(size, array, 2,50);


    create_algo_steps(ui->Algos_combobox->currentText());

}

void MainWindow::change_display_location()
{
    QSize display=QApplication::desktop()->screenGeometry().size();
    if ((this->pos().x()+this->width()>=0.9*display.width())||
        (this->pos().y()+this->height()>=0.9*display.height())){
        if (!this->isMaximized())this->move(0,0);
    }

    if ((0.8*display.height())<=this->height())
    {
        if (!this->isMaximized()) this->move(0,0);//todo set in center of screen(for all)
        this->showMaximized();
    }
}

void MainWindow::create_algo_steps(QString algoname)
{
    if (algoname=="Bogosort"){
        f1.open(QIODevice::Append,QIODevice::WriteOnly);
        connect(&sorting, SIGNAL(nextIteration(int*)), this, SLOT(nextIteration(int*)));
    }
    else
        connect(&sorting, SIGNAL(nextIteration(int, int, bool)), this, SLOT(nextIteration(int, int, bool)));

    sorting.start_sort(algoname, array, size);
    f1.close();

}

void MainWindow::nextIteration(int *array)
{

}

void MainWindow::nextIteration(int i, int j, bool swap)
{

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
    if (ui->Algos_combobox->currentText()=="Bogosort")
        if (next_or_prev==next)
        {

        }
        else
        {

        }

}


