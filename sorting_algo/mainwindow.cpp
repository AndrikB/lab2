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

    ui->Algos_combobox->addItems(sorting.get_algorithms());
    ui->Algos_combobox->setCurrentIndex(0);
    connect(ui->Algos_combobox, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_shuffle_btn_clicked()));

    connect(&sorting, SIGNAL(nextIteration(int*)), this, SLOT(nextIteration(int*)));
    connect(&sorting, SIGNAL(nextIteration(int, int, bool)), this, SLOT(nextIteration(int, int, bool)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(on_next_btn_clicked()));

    f1.setFileName("file.txt");

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
        delete[] array;
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
    f1.close();
    f1.open(QFile::WriteOnly|QFile::Truncate);

    if (algoname=="Bogosort"&&ui->count_items_spin->maximum()>10){//becouse bogosort it is joke:))))

        if (ui->count_items_spin->value()>10){
            ui->count_items_spin->setValue(3);
            ui->count_items_spin->setMaximum(10);
            ui->count_items_slider->setMaximum(10);
            on_shuffle_btn_clicked();
            on_shuffle_btn_clicked();
            return;
        }
        ui->count_items_spin->setMaximum(10);
        ui->count_items_slider->setMaximum(10);

    }
    else{//correct val
        QSize display=QApplication::desktop()->screenGeometry().size();
        ui->count_items_spin->setMaximum(qMin(display.width()/2, display.height()-200));
        ui->count_items_slider->setMaximum(qMin(display.width()/2, display.height()-200));
    }

    sorting.start_sort(algoname, array, size);



}

void MainWindow::nextIteration(int *array)
{
    QTextStream st(&f1);
    for (int i=0;i<size;i++)
    {
        st<<(array[i])<<' ';
    }
    st<<endl;

}

void MainWindow::nextIteration(int i, int j, bool swap)
{
    QTextStream st(&f1);
    st<<i<<' '<<j<<' '<<false<<endl;
    st<<i<<' '<<j<<' '<<swap<<endl;
}

void MainWindow::stop_writing()
{
    is_reading=true;
    f1.close();
    f1.open(QFile::ReadOnly);
    str=new QTextStream(&f1);
}


void MainWindow::on_Start_btn_clicked()
{
    if (timer.isActive()){
        timer.stop();
    }
    else {
        timer.start(150/ui->AnimSpeed_spin->value());
    }

}



void MainWindow::on_next_btn_clicked()
{
    if (!is_reading)stop_writing();
    read_iteration(next);

}

void MainWindow::on_back_btn_clicked()
{
    if (!is_reading)stop_writing();//todo this for start(mb)
    read_iteration(prev);

    paintwidget.visualize(size, array);
}

void MainWindow::read_iteration(MainWindow::iteration next_or_prev)//todo
{
    if (ui->Algos_combobox->currentText()=="Bogosort"){
        if (next_or_prev==next)
        {

            for (int i=0;i<size;i++){
                *str>>array[i];

            }


        }
        else
        {

        }
        paintwidget.visualize(size, array);
    }
    else
    {
        if (next_or_prev==next)
        {
            int i,j, swap;
            *str>>i>>j>>swap;
            if (swap){
                std::swap(array[i], array[j]);
                //std::swap(i,j);
            }
            paintwidget.visualize(size, array,i,j);
        }
        else
        {

        }
    }


    str->readLine();
    if (str->atEnd())
        ui->next_btn->setEnabled(false);
}

