//
// Created by HP on 2020-12-3.
//

#ifndef CLIONCPP_RADIX_SORT_H
#define CLIONCPP_RADIX_SORT_H

#include "counting_sort.h"

//基数排序，升序
void radix_sort(int *arr, int len);

//获得数组中的最大位数，假如最大值为878,那么返回3,最大值的位数不就是最大位数吗
int getMaxDigit(int);

#endif //CLIONCPP_RADIX_SORT_H
