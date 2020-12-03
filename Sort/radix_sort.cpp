#include "declaration/radix_sort.h"

/*
 * 基数排序思想和计数、桶排序差不多，不过却很有趣
 * 之前我们是按照数的值来确定桶的位置，但在基数排序值是用数某一位的值来确定

    桶中保存的是特定某一位上，位值相同的数,比如 834,954, 在比较个位时，同属一个桶

 * 假如我们有 99,3,44,51,67
 * 第一轮比较倒数第一位数即个位 digitValue of arr[i]， 那么将arr[i]，放在buckets[digitValue](这个桶也是数组)中（直接放在后面即可）
 * 这时每个桶为, {51},{3},{44},{67},{99},(每位有10中可能，所以共用10个桶，空桶我没写出来) 按顺序输出到原序列 51,3,44...,清空所有桶
 * 第二轮，再次遍历arr，比较每个数的十位(没有十位，默认0), 同样道理，按照十位值放在对应桶中--> {3},{44},{51},{67},{99}, 由于没有数有第三位，排序完成
 *
 * 这个道理很简单，第一轮我们保证个位是升序的，这时只有一位的数，一定按照桶的先后顺序分配好了，即输出序列中只有个位数大的一定在个位数小的之前
 * 第二轮按照输出序列再次遍历，这时，如果十位不同，那没什么好说的，如果十位相同，个位不同，那么经过上一轮的比较，十位相同的数，个位大的排后面
 *      而没有十位的数，在最前面的桶中，同样按照个位大的排后面的原则
 * 第三轮再次遍历，只有三位的数，当百位相同，十位/个位不同时,由于第一轮和第二轮的成果，使得后面两位数排好序了的，这时放在桶中没问题
 *      而没有百位的数，之前的二位数一定在个位数之后，然后二位数也已经排好序，所以没问题
 * .....
 * 即第 i 轮结束后，都能确保在倒数第i位到个位的范围内是已经排好序了的，只看后i位，是有序的
 * 同样第 i + 1 轮也会使用到 i 轮的成果
 * 只有当 i = 最大位数时，才能结束，否则像 1000000,就会一直排在前面
 *
 * 由于需要对负数进行处理，再加上诸如 99999,99991等高位数密集序列的考虑
 * 我们可以对初始序列做一个转化，即arr[i] = arr[i]-min,然后再进行正常的基数排序
 * 最后得到的最终序列只需 arr[i] = arr[i] + min, 即可原样返回
 * 对于复杂结构node{key,data}来说也可以进行
 *
 * 这里我换个桶的实现思路，不用链表结构
 * 我们让n个桶共用一个一维数组结构，为此每个桶的界限要分清楚，此时我想起了计数排序时实现的 bucket[i] += bucket[i-1]
 * 外加两个数组 digitValues[len], bucket_num[10],共用的数组我们就用 outArray[len], bucket_num保存每个桶的元素个数
 * 这样的话，在一轮中遍历arr过程中，我们可以使用一个数组将arr中的digitValue先保存起来,并且 bucket_num[digitValue]++
 * 然后对 bucket_num[i] += bucket[i-1], 此时i桶的的最后一位数在outArray中的下标就是 bucket_num[i]-1
 * 然后我们反向遍历arr，找到arr中每个数arr[i]对应的digitValue,然后index = bucket_num[digitValue]-1, digitValue--;
 * outArray[index] = arr[i], 最后将outArray复制进arr, 这就是一轮的额外步骤，当然 bucket_num清空
 */

void radix_sort(int *arr, int len) {
    int minValue, maxValue;
    getMaxAndMinValueInArray(arr, len, &maxValue, &minValue);
    int maxDigit = getMaxDigit(maxValue); //最大数的位数即最大位数
    if (maxValue == 0 || (maxValue == minValue)) //全是0，或者全一样
        return;
    //每个桶是一个链表队列
    const int BUCKET_NUM = 10;
    //先进行部分处理，只保存与minValue的差值
    for (int k = 0; k < len; ++k) {
        arr[k] = arr[k] - minValue;
    }

    int *outArray = new int[len]; //暂存结果
    int *digitValues = new int[len]; //暂存arr每一个数的digitValue
    int *bucket_num = new int[BUCKET_NUM]; //保存每个桶的元素个数

    //开始真正的处理部分，从个位到最大位, % 10 / 1 可取得个位，% 100 / 10取得十位....
    for (int digit = 1, mod = 10, dev = 1, digitValue; digit <= maxDigit; ++digit, mod *= 10, dev *= 10) {
        for (int k = 0; k < BUCKET_NUM; ++k) {
            bucket_num[k] = 0; //每个桶初始时为空
        }

        for (int i = 0; i < len; ++i) { //遍历arr
            int arrValue = arr[i];
            digitValue = arrValue % mod / dev;
            digitValues[i] = digitValue; //暂存arr[i]的位值
            bucket_num[digitValue]++; //桶中元素加一
        }

        //累加前面桶的元素个数，可以得到该桶的最后一个元素所在位置
        for (int l = 1; l < BUCKET_NUM; ++l) {
            bucket_num[l] += bucket_num[l - 1];
        }

        int index = 0;
        //反向遍历arr
        for (int m = len - 1; m >= 0; --m) {
            digitValue = digitValues[m];
            index = bucket_num[digitValue] - 1; //该元素在最终输出序列中的下标
            outArray[index] = arr[m];
            bucket_num[digitValue]--;
        }

        //复制给arr
        for (int j = 0; j < len; ++j) {
            arr[j] = outArray[j];
        }
    }

    //所以元素加上min
    for (int l = 0; l < len; ++l) {
        arr[l] = arr[l] + minValue;
    }

    delete[] bucket_num;
    delete[] outArray;
    delete[] digitValues;
}

int getMaxDigit(int maxValue) {
    int digit = 0; //如果最大值为0，那么位数为0
    while (maxValue > 0) {
        maxValue = maxValue / 10;// 321->32->3->0
        digit++;
    }
    return digit;
}
