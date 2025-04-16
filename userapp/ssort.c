#include "drawpic.c"



int* createArray(int size)
{
    int* arr = (int*)malloc(size * sizeof(int));
    int i;
    for (i = 0; i < size; i++)
    {
        arr[i] = rand() % 1000;     //数组元素为0~999的随机整数
    }
    return arr;
}

inline void swap(int* a, int firstIndex, int secIndex)
{
    int temp;
    temp = a[firstIndex];
    a[firstIndex] = a[secIndex];
    a[secIndex] = temp;
}

void insertionSort(int arr[],int size,int lEdge)
{

    drawArr(arr,size,lEdge,RGB(255,0,0));    //画出初始数组,作为底板
    int i, j;
    for (i = 1; i < size; i++)
    {
        for (j = i;(j>0)&&(arr[j-1]>arr[j]);j--)//从后向前遍历
        {
            drawSwap(arr,j,j-1,lEdge,RGB(255,0,0));    //交换元素
            nanosleep_delay();
        }
    }
}