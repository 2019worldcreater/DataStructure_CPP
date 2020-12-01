#include "declaration/direct_insert_sort.h"

//直接插入排序,升序， 稳定
void direct_insert_sort(int *arr, int len) {
    //排序过程类似于我们生活中对手牌排序 : 从小到大
    //首先看第2个元素，与第1个元素比较，如果[1]<[0],那么 [1]插入到0前面，0顺序后移
    //然后看第3个元素，如果[2]小于[1],那么往前找到比[2]小的元素 i, 然后插入到 i后面，i+1顺序后移，直到 i = -1
    //对于第n个元素，也是一样，找到前面第一个小于[len]的元素 i,插入到 i后面

    for (int i = 1; i < len; ++i) { //从第二个元素开始，往前找
        int index = i - 1; //从上一个元素开始比较
        int value = arr[i]; //保存值，相当于将牌提出来
        while (index >= 0 && arr[index] > value) { //往前寻找第一个不大于 value的元素
            arr[index + 1] = arr[index]; //value每往前一步,前面的元素就可以顺序后移了
            index--;
        }
        arr[index + 1] = value; //此时[index] <= value,亦或是 index = -1,即value最小
    }
}
