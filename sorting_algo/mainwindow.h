#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

#include "paintwidget.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_next_btn_clicked();

    void on_shuffle_btn_clicked();

private:
    Ui::MainWindow *ui;
    PaintWidget paintwidget;
    int *array;
    QTime execTimer;
    bool isRunning = false;
    enum iteration{next, prev};

    void swap (int i, int j);
    void read_iteration(iteration next_or_prev);
};

#endif // MAINWINDOW_H
