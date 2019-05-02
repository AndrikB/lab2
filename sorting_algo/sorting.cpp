#include "sorting.h"
#include<QList>
#include<QPair>
#include<QVector>

Sorting::Sorting()
{

}

QVector<QPair<QPair<int,int>,int>> Sorting::BubbleSort(QList<int> &numbers)
{
    QVector<QPair<QPair<int,int>,int>> ans;
    int n=numbers.size();
    for (int i=0;i<n-1;i++)
    {
        for (int j=0;j<n-i-1;j++)
        {
            if (numbers[j]>numbers[j+1])
            {
                ans.push_back({{j,j+1},1});
            }
            else
            {
                ans.push_back({{j,j+1},0});
            }
        }
    }
    return ans;
}
