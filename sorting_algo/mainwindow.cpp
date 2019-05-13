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
    f1.setFileName("file.txt");


    //if (!open_old())
        on_shuffle_btn_clicked();

    connect(ui->Algos_combobox, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_shuffle_btn_clicked()));




    connect(&sorting, SIGNAL(nextIteration(int*)), this, SLOT(nextIteration(int*)));
    connect(&sorting, SIGNAL(nextIteration(int, int, bool)), this, SLOT(nextIteration(int, int, bool)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(on_next_btn_clicked()));
    connect(ui->AnimSpeed_spin, SIGNAL(valueChanged(int)), this, SLOT(change_speed(int)));


}

bool MainWindow::open_old()
{
    QFile f2("oldFile.txt");
    if (!f2.open(QFile::ReadOnly)) return false;
    QTextStream str(&f2);
    algName=str.readLine();
    ui->Algos_combobox->setCurrentText(algName);
    str>>size;
    if (size<2) return false;
    array=new int[static_cast<unsigned long long>(size)];
    for (int i=0;i<size;i++)
        str>>array[i];
    int z;
    str>>z; stage_iteration=z;
    str>>z; if (z==0) previous_action=MainWindow::Iteration::next;
            else previous_action=MainWindow::Iteration::prev;

    str>>iteration;

    stop_writing();
    for (int i = -1; i < iteration; i++) {
        if (f1.size()-2>this->str->pos())return false;
        this->str->readLine();
    }

    on_back_btn_clicked();
    on_next_btn_clicked();
    ui->count_items_spin->setValue(size);
    return true;
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
    set_all_enabled(true);
    ui->back_btn->setEnabled(false);
    iteration=0;write_iteration();
    if (timer.isActive())on_Start_btn_clicked();

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

    paintwidget.visualize(size, array);
    algName=ui->Algos_combobox->currentText();
    create_algo_steps(algName);
}

void MainWindow::change_display_location()
{
    QSize display=QApplication::desktop()->screenGeometry().size();

    if ((0.8*display.height())<=this->height())
    {
        //if (!this->isMaximized()) this->move(0,0);//todo set in center of screen(for all)
        //this->showMaximized();
    }
}

void MainWindow::create_algo_steps(QString algoname)
{
    f1.close();
    f1.open(QFile::WriteOnly|QFile::Truncate);
    f1.putChar('\n');

    if (algoname=="Bogosort"&&ui->count_items_spin->maximum()>7){//becouse bogosort it is joke:))))

        if (ui->count_items_spin->value()>7){
            ui->count_items_spin->setValue(5);
            ui->count_items_spin->setMaximum(7);
            ui->count_items_slider->setMaximum(7);
            on_shuffle_btn_clicked();
            on_shuffle_btn_clicked();
            return;
        }
        ui->count_items_spin->setMaximum(7);
        ui->count_items_slider->setMaximum(7);

    }
    else if (algoname!="Bogosort"){//correct val
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
    st<<i<<' '<<j<<' '<<swap<<endl;
}

void MainWindow::stop_writing()
{
    is_reading=true;
    f1.close();
    f1.open(QFile::ReadOnly);
    str=new QTextStream(&f1);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QFile f2("oldFile.txt");
    f2.open(QFile::WriteOnly|QFile::Truncate);
    QTextStream str(&f2);
    str<<algName<<endl;
    str<<size<<endl;
    for (int i=0;i<size;i++)
        str<<array[i]<<' ';
    str<<endl;
    str<<stage_iteration<<' '<<previous_action<<endl;

    str<<iteration<<endl;

    f2.close();
}

void MainWindow::set_all_enabled(bool enable)
{
    ui->back_btn->setEnabled(enable);
    ui->next_btn->setEnabled(enable);
}


void MainWindow::on_Start_btn_clicked()
{
    if (timer.isActive()){
        timer.stop();
        ui->Start_btn->setText("start");
        set_all_enabled(true);
    }
    else {
        timer.start(static_cast<int>(1000.0/ui->AnimSpeed_spin->value()));
        ui->Start_btn->setText("pause");
        set_all_enabled(false);
    }

}

void MainWindow::change_speed(int speed)
{
    if (timer.isActive()){
        timer.stop();
        timer.start(static_cast<int>(1000.0/speed));
    }
}

void MainWindow::write_iteration()
{
    ui->iteration_lbl->setText("Iteration: "+QString::number(iteration));
}

void MainWindow::on_next_btn_clicked()
{
    if (!is_reading)stop_writing();


    read_iteration(next);
    if (!timer.isActive())
        ui->back_btn->setEnabled(true);
    write_iteration();
}

void MainWindow::on_back_btn_clicked()
{
    if (!is_reading)stop_writing();


    read_iteration(prev);
    if (!timer.isActive())
        ui->next_btn->setEnabled(true);
    write_iteration();
}

void MainWindow::read_iteration(MainWindow::Iteration next_or_prev)//todo
{

    if (algName=="Bogosort"){
        if (next_or_prev==next)
        {
            iteration++;
            for (int i=0;i<size;i++){
                *str>>array[i];
            }


        }
        else
        {
            iteration--;
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
                    *str>>i>>j>>swap;iteration++;
                    if (swap) stage_iteration=true;

                }
                else {//now is swaping
                    set_line_before(str);
                    *str>>i>>j>>swap;
                    std::swap(array[i], array[j]);
                    std::swap(i, j);
                    stage_iteration=false;

                }
            }
            else {
                if (!stage_iteration){//now is looking
                    *str>>i>>j>>swap;iteration++;
                    if (swap) {std::swap(array[i], array[j]);std::swap(i, j);}
                    else {
                        *str>>i>>j>>swap;iteration++;
                        stage_iteration=swap;
                    }
                }
                else {//now is swaping
                    *str>>i>>j>>swap;iteration++;
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
                    if (swap) {

                        std::swap(array[i], array[j]);
                        std::swap(i, j);
                        set_line_before(str);str->seek(str->pos()-1);iteration--;
                    }
                    else {
                        set_line_before(str);iteration--;
                        set_line_before(str);
                        *str>>i>>j>>swap;
                        stage_iteration=swap;
                        if (!swap) {set_line_before(str);str->seek(str->pos()-1);iteration--;}

                    }
                }
                else {
                    set_line_before(str);iteration--;
                    set_line_before(str);
                    *str>>i>>j>>swap;
                    stage_iteration=swap;
                    if (!swap) {set_line_before(str);str->seek(str->pos()-1);iteration--;}

                }


            }
            else {
                if (!stage_iteration){//now is looking
                    set_line_before(str);
                    *str>>i>>j>>swap;
                    if (swap) stage_iteration=true;
                    else {set_line_before(str);str->seek(str->pos()-1);iteration--;}
                }
                else {//now is swaping

                    set_line_before(str);
                    *str>>i>>j>>swap;
                    std::swap(array[i], array[j]);
                    std::swap(i, j);
                    stage_iteration=false;
                    set_line_before(str);str->seek(str->pos()-1);iteration--;
                }

            }
        }
        paintwidget.visualize(size, array,i,j);
    }

    previous_action=next_or_prev;
    str->readLine();
    if (str->pos()<4)
        ui->back_btn->setEnabled(false);
    if (str->atEnd()){
        timer.stop();
        ui->Start_btn->setText("start");
        ui->next_btn->setEnabled(false);
    }
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

