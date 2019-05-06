#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <fstream>

#include <QMainWindow>
#include <QTime>
#include <QFile>
#include <QTextStream>

#include "paintwidget.h"
#include "sorting.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void change_display_location();
    ~MainWindow();

private slots:
    void on_next_btn_clicked();

    void on_shuffle_btn_clicked();

    void on_back_btn_clicked();

    void create_algo_steps(QString algoname);
    void nextIteration(int* array);
    void nextIteration(int i, int j, bool swap);

private:
    Ui::MainWindow *ui;
    PaintWidget paintwidget ;
    Sorting sorting;

    int *array=Q_NULLPTR;
    int size;
    enum iteration{next, prev};


    std::ofstream f1;
    bool is_reading=false;//true - it is vizualization
    void read_iteration(iteration next_or_prev);
    void stop_writing();
};

#endif // MAINWINDOW_H
