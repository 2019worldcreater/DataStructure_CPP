
#ifndef CLIONCPP_HEAP_SORT_H
#define CLIONCPP_HEAP_SORT_H

#include "utils/utils.h"

//对以parent为根的子树进行大顶堆调整,给出大顶堆序列arr,和长度len
//请确保parent的两个子树都是大顶堆
void heapAdjust(int *arr, int parent, int len);

//降序排序只需改一下heap_adjust的大小判断即可
//堆排序,升序排序，初始序列arr，及长度len
void heap_sort(int *arr, int len);

#endif //CLIONCPP_HEAP_SORT_H
