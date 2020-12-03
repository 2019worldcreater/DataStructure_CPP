#include "declaration/bucket_sort.h"
#include "declaration/quick_sort.h"

/*
 * 桶排序,其实是一种分而治之的思想，而非排序本身
 * 映射函数的设计，和原本数据都有很大影响
 *
 * 在计数排序中，bucket中放的是同值，arr[i]的所在桶index=arr[i]-min
 * 但桶排序中，每个桶存放的可能是不同的值(所以每个桶是个独立的数组，而不是计数排序中只有一个计数值)
 * 而arr[i]的桶号 index = f(arr[i]), 其中 f(x)是设计好的映射函数，很重要
 * 通过设计合适的映射函数，将arr[i]分配到不同的bucket中，arr[i]不同时，桶号可能相同，但通常相差不大
 * 在升序排序实现中
 * 如果有 bucket[i],bucket[j], i < j, 那么必须 j 桶中的元素都大于 i 桶中的元素(通过映射函数实现)
 * 这时由于每个桶中的元素相差不大，并且数量应该不多，我们可以使用任意的排序算法对每个桶内部排序，都排好序后 ,这时每个桶也已经是升序序列了
 * 并且后面的桶大于前一个桶，所以只要顺次将每个桶中的元素取出即可得到最终的整体升序序列
 *
 * 首先考虑需要多少个 bucket, 之前我想用定量的桶，就算有些桶用不到也无所谓，后来我想用 max-min/len作为桶数,考虑到{1,1000}这种,也可以设定num<=len/2
 *      --之后我读到一篇博客说到，使用元素个数个桶，即len个桶,即使有些桶用不上
 * 映射函数f的设计, 暂定为 int(double(arr[j] - min) * len / double(max-min + 1)), 如果大于maxIndex则放在最后一个桶
 * 单个桶的排序选择直接插入排序
 *
 * bucket如何实现，参考radix_sort.cpp的思路，用链表太浪费性能了，之前我用链表，感觉慢死了,用一个一维数组模拟桶高效百倍
 *
 * 其实这种根据映射函数确定下标，然后同一映射值的key放在一个桶中的概念，其实就是之前哈希表外加链表法解决冲突
 * 而其实这也是 java hashMap的存储原理
 */

void bucket_sort(int *arr, int len) {
    int min, max;
    getMaxAndMinValueInArray(arr, len, &max, &min);
    if (max == min)
        return;
    const int BUCKET_NUM = len; //默认len个桶
    int *buckets = new int[BUCKET_NUM]; //保存每个桶中的元素个数
    int *outArray = new int[len]; //桶共用数组
    int *bucket_index_all = new int[len]; //保存每个arr[i]的相应桶号
    //每个桶为空
    for (int i = 0; i < BUCKET_NUM; ++i) {
        buckets[i] = 0;
    }

    int max_min_minus = max - min + 1;
    int bucket_index; //桶号
    //对arr遍历，分配每个元素去相应的桶中
    for (int j = 0; j < len; ++j) {
        bucket_index = int(double(arr[j] - min) * len / double(max_min_minus)); //映射函数,我查了很多博客才发现的
        if (bucket_index > BUCKET_NUM - 1) //太大的直接放在最后一个桶
            bucket_index = BUCKET_NUM - 1;
        bucket_index_all[j] = bucket_index;
        buckets[bucket_index]++;
    }

    int *bucket_size = new int[BUCKET_NUM]; //暂存每个桶的大小，后面有用
    for (int n = 0; n < BUCKET_NUM; ++n) {
        bucket_size[n] = buckets[n];
    }

    //buckets[i] += bucket[i-1]
    for (int k = 1; k < BUCKET_NUM; ++k) {
        buckets[k] += buckets[k - 1];
    }

    int outIndex;
    //反向遍历arr，将元素放进对应的桶中，其实桶只是数组outArray中的某一个区域
    for (int l = len - 1; l >= 0; --l) {
        bucket_index = bucket_index_all[l]; //桶号
        outIndex = buckets[bucket_index] - 1; //该元素在outArray中的下标
        buckets[bucket_index]--;
        outArray[outIndex] = arr[l];
    }

    int *address = outArray; //每个桶的逻辑数组起始地址
    //对每个逻辑桶进行排序
    for (int m = 0; m < BUCKET_NUM; ++m) {
        direct_insert_sort(address, bucket_size[m]);
        address += bucket_size[m]; //下一个桶的起始地址
    }

    for (int i1 = 0; i1 < len; ++i1) {
        arr[i1] = outArray[i1];
    }

    delete[] bucket_index_all;
    delete[] outArray;
    delete[] buckets;
}
