#ifndef SORTING_H
#define SORTING_H
#include <QObject>
#include<QVector>
#include<QPair>

class Sorting :public QObject
{
    Q_OBJECT
public:
    Sorting();
    static int* shuffle(int *arr,int size);
    QStringList get_algorithms(){return algorithms;}
    void start_sort(QString name_algorithms,const int *arr,int size);
    static bool isSorted(int *arr,int size);
signals:
    void nextIteration(int i,int j,bool swap);
    void nextIteration(int *arr);

private:
    QStringList algorithms={"Bubble sort","Bogosort","Selection sort","Insertion sort"};


    //----------sorting algorithms--------------//
    void BubbleSort(int *arr,int size);
    void BogoSort(int *arr,int size);
    void SelectionSort(int *arr,int size);
    void InsertionSort(int *arr,int size);

};

#endif // SORTING_H
