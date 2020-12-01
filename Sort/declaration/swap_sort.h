//
// Created by HP on 2020-12-1.
//

#ifndef CLIONCPP_SWAP_SORT_H
#define CLIONCPP_SWAP_SORT_H

#include "utils/utils.h"

//直接交换排序, 数组,长度
void swap_sort(int *a, int n) {
    //交换排序，最简单
    //从数组第一个元素开始，依次比对其后每个元素，如果不符合大小规则，则交换两个元素
    //每个元素都会比对其后所有元素
    for (int i = 0; i < n - 1; ++i) { //0~n-2元素就行，n-1后面没元素了
        for (int j = i + 1; j < n; ++j) { //i元素后面的元素
            if (a[i] > a[j])  //交换两个数， 每次 i 循环都会让 i ~ n-1 中的最小值转到 i
                swap(a, i, j);
        }
    }
}

#endif //CLIONCPP_SWAP_SORT_H
