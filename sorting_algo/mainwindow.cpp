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
    is_reading=false;
    previous_action=next;
    stage_iteration=false;
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
    f1.putChar('\n');

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
    //st<<i<<' '<<j<<' '<<false<<endl;
    //if (swap)
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
            set_line_before(str);
            set_line_before(str);
            for (int i=0;i<size;i++)
            {
                *str>>array[i];
            }

            set_line_before(str);
        }
        paintwidget.visualize(size, array);
    }
    else
    {
        int i,j, swap;
        if (next_or_prev==next)
        {
            if (previous_action==next)//++
            {
                if (!stage_iteration){//now is looking
                    *str>>i>>j>>swap;
                    if (swap) stage_iteration=true;
                }
                else {//now is swaping
                    set_line_before(str);
                    *str>>i>>j>>swap;
                    std::swap(array[i], array[j]);
                    stage_iteration=false;

                }
            }
            else {
                if (!stage_iteration){//now is looking
                    *str>>i>>j>>swap;
                    if (swap) std::swap(array[i], array[j]);
                    else {
                        *str>>i>>j>>swap;stage_iteration=swap;
                        stage_iteration=swap;
                    }
                }
                else {//now is swaping
                    *str>>i>>j>>swap;
                    stage_iteration=swap;
                }

            }


        }
        else//prev
        {
            if (previous_action==next){//++

                if (!stage_iteration){//now is looking

                    set_line_before(str);
                    *str>>i>>j>>swap;
                    qDebug()<<i<<j<<swap;
                    if (swap) {

                        std::swap(array[i], array[j]);
                        set_line_before(str);str->seek(str->pos()-1);
                    }
                    else {
                        set_line_before(str);
                        set_line_before(str);
                        *str>>i>>j>>swap;
                        qDebug()<<"new"<<i<<j<<swap;
                        stage_iteration=swap;
                        if (!swap) {set_line_before(str);str->seek(str->pos()-1);}

                    }
                }
                else {
                    set_line_before(str);
                    set_line_before(str);
                    *str>>i>>j>>swap;
                    stage_iteration=swap;
                    if (!swap) {set_line_before(str);str->seek(str->pos()-1);}

                }


            }
            else {
                if (!stage_iteration){//now is looking
                    set_line_before(str);
                    *str>>i>>j>>swap;
                    if (swap) stage_iteration=true;
                    else {set_line_before(str);str->seek(str->pos()-1);}
                }
                else {//now is swaping

                    set_line_before(str);
                    *str>>i>>j>>swap;
                    qDebug()<<"a"<<i<<j<<swap;
                    std::swap(array[i], array[j]);
                    stage_iteration=false;
                    set_line_before(str);str->seek(str->pos()-1);
                }

            }
        }
        paintwidget.visualize(size, array,i,j);
    }

    previous_action=next_or_prev;
    str->readLine();
    if (str->atEnd())
        ui->next_btn->setEnabled(false);
}

void MainWindow::set_line_before(QTextStream *str)
{
    str->seek(str->pos()-2);
    char c;
    *str>>c;
    while (c!='\n') {
        str->seek(str->pos()-2);
        *str>>c;
    }
}

