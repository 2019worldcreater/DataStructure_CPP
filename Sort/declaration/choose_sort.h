//
// Created by HP on 2020-12-1.
//

#ifndef CLIONCPP_CHOOSE_SORT_H
#define CLIONCPP_CHOOSE_SORT_H


#include "utils/utils.h"

//简单选择排序,主要是交换数据操作少很多，略优于冒泡， 不稳定：交换 i,min时，i-min之间可能存在 k,[k]=[i]
void simple_choose_sort(int *a, int n);

#endif //CLIONCPP_CHOOSE_SORT_H
