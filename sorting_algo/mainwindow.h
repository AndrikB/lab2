#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
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

    void on_Start_btn_clicked();
    void change_speed(int);

private:
    Ui::MainWindow *ui;
    PaintWidget paintwidget ;
    Sorting sorting;

    int *array=Q_NULLPTR;
    int size;
    enum iteration{next, prev};


    QFile f1;
    QTextStream *str;
    bool is_reading=false;//true - it is vizualization

    bool stage_iteration=false;//false-look; true - swap
    iteration previous_action=next;
    QTimer timer;

    void read_iteration(iteration next_or_prev);
    void set_line_before(QTextStream* str);
    void stop_writing();

    void set_all_enabled(bool enable);
};

#endif // MAINWINDOW_H
