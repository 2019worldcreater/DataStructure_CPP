//
// Created by HP on 2020-12-2.
//

#ifndef CLIONCPP_COUNTING_SORT_H
#define CLIONCPP_COUNTING_SORT_H

//优化版计数排序，升序，稳定
void counting_sort(int *arr, int len);

//获得arr数组中最大值，最小值
void getMaxAndMinValueInArray(const int *arr, int len, int *max, int *min);

#endif //CLIONCPP_COUNTING_SORT_H
