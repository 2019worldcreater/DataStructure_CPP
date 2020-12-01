#include "declaration/merge_sort.h"

//合并操作,将分别排好序的left{low~mid}部分和right{mid+1,high}部分合并到arr{low~high}中
//我只需告诉数组的界限即可合并,合并的操作范围可能只是arr的一部分
void merge(int *arr, int low, int mid, int high) {
    if (low >= high) //小于2个元素，直接返回
        return;
    int temp[high - low + 1]; //left,right合并结果先存在temp中，否则arr会被覆盖，因为合并前的left,right就保存在arr中
    int leftIndex = low, rightIndex = mid + 1, minValue, index = 0;
    while (leftIndex <= mid && rightIndex <= high) { //两方都不为空时
        if (arr[rightIndex] < arr[leftIndex]) //只有右边比左边小时才选右边，保证稳定性
            temp[index++] = arr[rightIndex++];
        else
            temp[index++] = arr[leftIndex++];
    }
    //将剩下的元素依次进去，此时必然有一方为空
    while (leftIndex <= mid)
        temp[index++] = arr[leftIndex++];
    while (rightIndex <= high)
        temp[index++] = arr[rightIndex++];
    //将结果再复制进arr{low~high}
    for (int i = 0; i < index; ++i) {
        arr[low + i] = temp[i];
    }
}

void merge_sort_main(int *arr, int len) { //只是提供调用而已
    merge_sort(arr, 0, len - 1);
}

void merge_sort(int *arr, int low, int high) {
    /*
     * 我之前的想法是：每次分割，new两个数组分别保存arr的两部分，然后对两个再分别进行mergeSort
     * 后来我发现，只需要逻辑上产生分割线就行，不需要真正分割
     * 因为分割后，对left,right进行归并排序过程中，他们都不会越界，我们只需要声明一个mid分割线，即可将arr分为逻辑上的两部分
     * 而在合并时，也是对arr{low~high}这个数组的两部分进行合并，各部分不会干扰
     * 当然合并的时候要先将结果保存到一个temp数组中，然后再转到arr中
     */
    if (low < high) { //只有一个元素时，直接结束
        int mid = (low + high) / 2; //分割线
        merge_sort(arr, low, mid); //对left{low~mid}归并排序
        merge_sort(arr, mid + 1, high); //对right{mid+1~high}排序
        merge(arr, low, mid, high); //将left,right合并为{low~high}
    }
}

/*
 * 递归的思想是将一个整体逐个一分为二，然后逐个两两合并为一个整体
 * 但其实分割的过程中，并没有做任何事，只是按照递归返回顺序告诉合并函数哪两个部分需要合并
 * 所以我们可以使用迭代结构，人为操控合并次序，即合并哪两个部分或者说合并后的大部分的low，high以及分割线mid
 */
void merge_sort_dieDai(int *arr, int len) {
    //一开始我们让每两个相邻的元素构成的逻辑数组进行合并(gap=2), merge(0,0,1),merge(2,2,3)....merge(n,n,n-1)
    //然后再让每四个连续元素构成的数组进行合并(gap=4), merge(0,1,3), merge(4,5,7),...
    //gap = gap * 2.....
    //以此类推，最后当gap >= len时，就表示对整个数组的两部分进行合并，merge(0,len-1/2,len-1),此时排序完成

    //如果数组长度是2的幂，那么根本不需要考虑分配的时候，最后几个元素凑不齐gap个
    //如果长度不是的话，就会发生一些错误:
    // 当然我们仍然需要对剩下的k个元素进行merge,但mid界限不能简单设为该部分的中间位
    /*
     * 假如 len = 13
     * 我们设想一下，假如gap=2时，剩下一个元素没配对，此时merge它毛病，也可以不merge
     * 然后 gap = 4时，最后又是那个元素没配对，一样处理
     * 然后 gap = 8,剩下5个元素没配对, 即 [9]~[12], 此时如果将 mid = 10的话，会产生问题
     * mid的意义在于分隔开已经排好序的left,right部分, 之前[12]一直都是单飞,如果mid=10,那么表示[11][12]排好序了
     * 但实际上这两货之前根本没有交集，根本证明不了[11][12]是有序状态，所以存在毛病
     * mid应当设为11,这时才能保证左右部分各自排过序了
     * 最后 gap = 16,这时 mid 应设为 8, 因为上一步中[9]~[12]排过序了
     *
     * 后来我总结发现，当第一次配对不够时，mid就直接设为那部分的中位数，然后将mid设为low-1(这部分第一位数的下标 -1)
     * 然后第二次配对不够时，这时 mid 沿用之前的 low-1,然后 mid = newLow - 1
     * 以此类推，这样可以保证每次归并时使用的mid都具有意义
     * 我发现，每次配对不够的部分的长度必然是大于等于上一次不够时部分的长度
     * 至于为什么每次数量不够时 low~mid一定排序了，自己用数学证明下，我是偶然发现的
     */
    int gap = 2, low, mid, high; //low，mid，high保持过程中的逻辑数组界限
    bool notOver = true;
    int trueMid = -1;
    while (notOver) {
        low = 0; //从0开始
        while (low + gap - 1 < len) { //最后几个不够gap个，或者正好分配完了，才结束
            high = low + gap - 1;
            mid = (low + high) / 2;
            merge(arr, low, mid, high);//调用归并子函数
            low = high + 1;//下一个数组的下界
        }
        if (low < len) { //没有分配完，即存在不满gap个元素的部分
            if (trueMid < 0) { //第一次配对不够时
                trueMid = (len - 1 + low) / 2;
            }
            if (trueMid >= low) { //<low时只可能是发生在第n次配对不成功时 n>1,这时配对不成功的部分很上次一样，不需要配对
                merge(arr, low, trueMid, len - 1);
                trueMid = low - 1;
            }
        }
        if (gap >= len) //此时gap>=len,说明整个数组发生了归并，已结束
            notOver = false;
        gap *= 2;//范围扩大一倍
    }
}
