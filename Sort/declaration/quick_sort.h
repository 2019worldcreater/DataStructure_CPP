
#ifndef CLIONCPP_QUICK_SORT_H
#define CLIONCPP_QUICK_SORT_H

#include "utils/utils.h"

//对arr{low~high}进行基准排序，返回最后基准值所在下标
int pivot_sort(int *arr, int low, int high);

//快速排序主逻辑函数,对arr{low~high}进行基准排序,然后对基准的两边分区再分别进行基准排序，直到长度为1
void quick_sort_main(int *arr, int low, int high);

//快速排序,升序
void quick_sort(int *arr, int len);

//将low,mid,high三者中不大不小的数放在[low]中（当然要交换）,即取得基准值
void getPivot(int *arr, int low, int high);

#endif //CLIONCPP_QUICK_SORT_H
