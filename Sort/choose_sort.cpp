
#include "declaration/choose_sort.h"

//简单选择排序,主要是交换数据操作少很多，略优于冒泡， 不稳定：交换 i,min时，i-min之间可能存在 k,[k]=[i]
void simple_choose_sort(int *a, int n) {
    for (int i = 0; i < n - 1; ++i) { //每次循环从 i ~ n -1 中选出一个最小值所在下标 min, 然后交换 i 和 min
        int min = i; //最小值下标
        for (int j = i + 1; j < n; ++j) {
            if (a[min] > a[j])
                min = j;
        }
        if (min != i)  //如果不是本身
            swap(a, i, min);
    }
}