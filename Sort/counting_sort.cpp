#include "declaration/counting_sort.h"

/*
 * 计数排序，其实是个挺简单的排序方法
 * 原理: 我们找到arr中最大值max，准备数组 bucket[max+1]={0,0..} 即为每个数准备的桶
 * 然后遍历数组 arr, 如果 arr[i] = k, 那么bucket[k]++
 * 最后遍历输出 bucket, 假如 bk[2]=3,那么就连续输出3个2(赋值在原数组arr中)
 *
 * 但上面的是简陋版的计数排序，首先对于负数无法比较，其次如果只有 1000~1100,难道生成一个bk[1101]吗，太浪费了
 * 并且bk中只保存了值的次数，如果是复杂结构node{key,data},这时根本没点卵用，鬼知道谁是谁
 *
 * 所以需要改进版计数排序： 这里实现升序，降序的话，只需改一处 index = max-arr[i]
 * 首先找出 arr中的最小值min,最大值max, 生成bucket[max-min+1]
 * 遍历arr,key = arr[i], 那么 index = key-min, bk[index]++; bk[2]=2,表示有2个min+2
 * 将得到的bucket进行以下处理:
 *      bucket[i] += bk[i-1] + bk[i-2]...+bk[0], 其实可以从bk[1]开始，bk[i]+=bk[i-1],因为bk[i-1]已经保存了之前的总数
 *      即bk[i]的值再把前面的所有值都加在一起
 *      假如 bk[0]=2,bk[1]=3,bk[2]=2 --> bk[0]=2,bk[1]=5,bk[2]=7
 *      转换后的bk数组具有这个含义：即arr[0]~arr[1]应该放min+2, arr{1,4}放3个min+1,arr{5,6}放2个min+2
 *      而在各个区域中，虽然是同样的值，但我们还有arr，这个保存了各个元素初始位置的东东, 如果我们倒序遍历arr
 *      将对应的arr[i]值放在那块区域的最后一个即bk[index]标识的位置，然后bk[index]减一，这时如果在遇到同值，会放在上一次遇到的同值之前
 *
 * 所以最后只需倒序遍历arr, index = arr[i] - min, 然后 finalIndex = bucket[index] - 1
 * finalIndex即arr[i]在最终排序结果中的位置，因为是倒序遍历arr的，那么肯定要先放在那块区域的最后一个（当然需要一个temp暂存结果）
 * 然后 bucket[index]--, 下一次遇到和arr[i]同值时，就会放在arr[i]之前的位置中，如此排序结束
 *
 * 于是即节省了空间，也保证了序列的稳定性，对于复杂结构也具有良好可用性(只需将key提取出来对比就行)
 *
 * 计数排序限制：只限于比较数值，序列的分布不能太极端
 * 但如果序列没有违反这些限制，那么计数排序的速度远超shell、heap....
 *
 */

void counting_sort(int *arr, int len) {
    int min, max;
    getMaxAndMinValueInArray(arr, len, &max, &min);
    if (min == max) //我感觉不需要弄了
        return;
    int *bucket = new int[max - min + 1]; //计数桶
    for (int m = 0; m < max - min + 1; ++m) {
        bucket[m] = 0;
    }
    int index;
    //计数
    for (int i = 0; i < len; ++i) {
        index = arr[i] - min;
        bucket[index]++;
    }
    //转换bucket
    for (int j = 1; j < max - min + 1; ++j) {
        bucket[j] += bucket[j - 1];
    }
    int finalIndex;
    int *finalResult = new int[len]; //暂存结果,如果直接写int finalR[len]会出错，因为new是在堆中，直接声明是在栈中，大小不同

    for (int k = len - 1; k >= 0; --k) { //被++k害惨了
        index = arr[k] - min; //找到对应的桶
        finalIndex = bucket[index] - 1; //排序数组中的位置
        finalResult[finalIndex] = arr[k];
        bucket[index]--; //下一个同值，放在前一个
    }
    //复制
    for (int l = 0; l < len; ++l) {
        arr[l] = finalResult[l];
    }
    delete[] finalResult;
    delete[] bucket;
}

//获得arr数组中最大值、最小值
void getMaxAndMinValueInArray(const int *arr, int len, int *max, int *min) {
    int maxValue = arr[0];
    int minValue = arr[0];
    for (int i = 1; i < len; ++i) {
        if (arr[i] > maxValue)
            maxValue = arr[i];
        if (arr[i] < minValue)
            minValue = arr[i];
    }
    *max = maxValue;
    *min = minValue;
}