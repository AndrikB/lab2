#include "sorting.h"
#include<iostream>
#include<QList>
#include<QPair>
#include<QVector>
#include<QRandomGenerator>
#include<utility>
#include<vector>

using std::swap;
using std::pair;
using std::make_pair;
using std::vector;

Sorting::Sorting()
{

}

void Sorting::start_sort(QString name_algorithms, const int *arr, int size)
{
    int *array=new int[static_cast<unsigned long long>(size)];
    for (int i=0;i<size;i++)
        array[i]=arr[i];
    if (name_algorithms=="Bubble sort")
    {
        BubbleSort(array,size);
    }
    else if (name_algorithms=="Bogosort")
    {
        BogoSort(array,size);
    }
    else if (name_algorithms=="Selection sort")
    {
        SelectionSort(array,size);
    }
    else if (name_algorithms=="Insertion sort")
    {
        InsertionSort(array,size);
    }
    else if (name_algorithms=="Quick sort")
    {
        QuickSort(array,0,size-1);
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
    emit nextIteration(-1,-1,0);
}

void Sorting::BogoSort(int *arr, int size)
{
    while (!isSorted(arr,size))
    {
        arr=shuffle(arr,size);
        emit nextIteration(arr);
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
        {
            emit nextIteration(j,min_pos,0);
            if (arr[j]<arr[min_pos])
            {
                min_pos=j;
            }
        }
        if (min_pos>i)
        {
            emit nextIteration(i,min_pos,1);
            swap(arr[i],arr[min_pos]);
        }
    }
    emit nextIteration(-1,-1,0);
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
    emit nextIteration(-1,-1,0);
}

void Sorting::QuickSort(int *arr, int l, int r)
{
    if (l<r)
    {
        pair<int,int> q=partition(arr,l,r);
        QuickSort(arr,l,q.first);
        QuickSort(arr,q.second,r);
    }
}

pair<int,int> Sorting::partition(int *arr, int l, int r)
{
    vector<int> a1,a2,a3;//a1 - less than pivot, a2 - equal pivot, a3 - greater than pivot
    int d=(r-l+1),random_position=rand()%d+l;
    int m=arr[random_position];
    for (int i=l;i<=r;i++)
        if (arr[i]<m) a1.push_back(arr[i]);
        else if (arr[i]==m) a2.push_back(arr[i]);
        else a3.push_back(arr[i]);
    int s1=a1.size(),s2=a2.size(),s3=a3.size();
    int i=l,j=0;
    while (j<s1)
    {
        arr[i]=a1[j];
        i++; j++;
    }
    j=0;
    while (j<s2)
    {
        arr[i]=a2[j];
        i++; j++;
    }
    j=0;
    while (j<s3)
    {
        arr[i]=a3[j];
        i++; j++;
    }
    return make_pair(l+s1-1,l+s1+s2);
}
