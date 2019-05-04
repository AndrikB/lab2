#include "sorting.h"
#include<iostream>
#include<QList>
#include<QPair>
#include<QVector>
#include<QRandomGenerator>

using std::swap;

Sorting::Sorting()
{

}

void Sorting::start_sort(QString name_algorithms, int *arr, int size)
{
    if (name_algorithms=="Bubble sort")
    {
        BubbleSort(arr,size);
    }
    else if (name_algorithms=="Bogosort")
    {
        BogoSort(arr,size);
    }
    else if (name_algorithms=="Selection sort")
    {
        SelectionSort(arr,size);
    }
    else if (name_algorithms=="Insertion sort")
    {
        InsertionSort(arr,size);
    }
}

void Sorting::BubbleSort(int *numbers,int size)
{
    for (int i=0;i<size-1;i++)
    {
        bool was_swap=0;
        for (int j=0;j<size-i-1;j++)
        {
            if (numbers[j]>numbers[j+1])
            {
                emit nextIteration(j,j+1,1);
                swap(numbers[j],numbers[j+1]);
                was_swap=1;
            }
            else
            {
                emit nextIteration(j,j+1,0);
            }
        }
        if (!was_swap)
        {
            break;
        }
    }

}

void Sorting::BogoSort(int *arr, int size)
{
    while (!isSorted(arr,size))
    {
        arr=shuffle(arr,size);
        emit nextIteration(arr,size);
    }
}

bool Sorting::isSorted(int *arr, int size)
{
    for (int i=1;i<size;i++)
        if (arr[i]<arr[i-1]) return false;
    return true;
}

int *Sorting::shuffle(int *arr, int size)
{
    for (int i=0; i <size; ++i)
    {
        int rand = QRandomGenerator::global()->bounded(0,size);
        swap(arr[i], arr[rand]);
    }
    return arr;
}

void Sorting::SelectionSort(int *arr, int size)
{
    for (int i=0;i<size-1;i++)
    {
        int min_pos=i;
        for (int j=i+1;j<size;j++)
            if (arr[j]<arr[min_pos])
            {
                min_pos=j;
            }
        if (min_pos>i)
        {
            emit nextIteration(i,min_pos,1);
            swap(arr[i],arr[min_pos]);
        }
    }
}

void Sorting::InsertionSort(int *arr, int size)
{
    for (int i=1;i<size;i++)
    {
        int cur=arr[i];
        int j=i-1;
        while (j>=0)
        {
            if (arr[j]<=cur) break;
            arr[j+1]=arr[j];
            emit nextIteration(j,j+1,1);
            j--;
        }
        arr[j+1]=cur;
    }
}
