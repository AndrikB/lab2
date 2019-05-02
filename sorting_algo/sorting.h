#ifndef SORTING_H
#define SORTING_H
#include<QVector>
#include<QPair>

class Sorting
{
public:
    Sorting();
    QVector<QPair<QPair<int,int>,int>> BubbleSort(QList<int> &numbers);
};

#endif // SORTING_H
