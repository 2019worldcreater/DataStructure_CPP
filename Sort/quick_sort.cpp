#include "declaration/quick_sort.h"
#include "declaration/direct_insert_sort.h"

int rightIndex; // pivotSort()的返回值 ~ lastIndex是重复元素位置
/*
 * 额外优化 ： 1.使用high,mid,low三数中值作为pivot, 对于已经具备一定升序、降序的数组很有效
 *          2、数组大小不大时，使用直接插入排序
 *          3、使用重复元素堆积，聚集，大大提高了重复数组的处理性能，几百倍吧
 */
int pivot_sort(int *arr, int low, int high) {
    /*对 arr{low~high}区域进行分区基准排序
     * 过程：选中第一个元素 arr[low] 为基准值 pivot
     * 然后遍历pivot后面的值{low+1,high}, 如果发现[k] < pivot,那就将 [k]插入到[low]前面
     * 如果[k]>=pivot,则保持位置不变，即比pivot大的值放在pivot右边，比pivot小的放在它之前
     *
     * 经过这样的操作后，pivot一定与最终的排序结果是一样的位置，所以每次分区基准排序就是为了找到基准值最后的确切位置
     *
     * 如果插入到pivot之前还需要依次后移的话，效率有点低
     * 所以可以换个思路，这个算法本身只要求一点，大的放在pivot之后，小的放在pivot之前
     * 我们可以在把小的值拿到前面的同时，把大的值放在后面不就行了
     *
     * 模拟过程 :  3,4,2,1,5,6
     * pivot = 3, index->4
     * 遍历：发现2小于3,交换4,2 ---> 3,2,4,1,5,6, index->4
     *      发现1小于3,交换4,1--->3,2,1,4,5,6
     * 遍历结束，此时 index-1 --> 1, 交换 3,1 --> 1,2,3,4,5,6,  index-1-->3(pivot)
     *
     * 优化3，是为了解决重复元素多的数组运行慢的问题，但同时也影响了其他类型数组(不一定是正影响)
     *       for循环中的else if判断和后面的 rightIndex ,for这两个都是优化3的处理，不想要的删掉即可
     *       同时在主函数调用quick_sort，改一下 rightPartIndex-->pivotIndex
     */
    if (low >= high) //只有一个数，或者出错
        return low;

    //之前么有这个优化时，如果是一个升序、降序数组（有特定顺序了）,耗时很长，只有对随机数组处理还好
    //加了这个优化后，随机数组处理慢了点，但对其他两种数组处理简直嗨翻天，但对重复数组还是欠佳
    getPivot(arr, low, high); //优化1：取三点中值为基准，之前是之前取第一个数为基准的，这个函数将 high,mid,low中中间值放在了low

    int lastIndex = high + 1; //数组后面重复元素的起始下标
    int pivot = arr[low]; //基准值
    int index = low + 1; //如果存在比基准值小的数，就与该位置进行交换
    for (int i = low + 1; i <= high; ++i) {
        if (arr[i] < pivot) {
            swap(arr, i, index);//[index] > [i] 或 index == i
            index++; //low+1~index-1保存比pivot小的值
        } else if (arr[i] == pivot) {
            //优化3: 将重复元素先放在该数组部分后面
            //比如循环中: 3,4,5,1,3,5,6,首先 4,1交换, 然后 3(倒数第三个),6交换,lastIndex-->3, 即 3,1,5,4,6,5,3, index->1
            //循化最后一步, i = lastIndex --> 3, 这时比较3,3, 会导致循环一直卡死在这
            //所以lastIndex = i时表示到了重复元素区域了，直接结束
            if (lastIndex == i)
                break;
            lastIndex--; //如果有重复元素，就指针往前
            swap(arr, lastIndex, i);
            i--; //原先lastIndex的值还要比较一次
        }
    }

    rightIndex = index - 1;
    //优化3的后续步骤： 将堆积的重复元素放在pivot的周围，这样，再进行左右分区排序时，就不必考虑 pivot周围那一部分区域
    for (int j = lastIndex; j < high + 1; ++j) {
        rightIndex++;
        swap(arr, rightIndex, j);
    }
    //此时 [low+1]~[index-1]都是小于pivot的,我只需交换[low]和[index-1],将pivot放在这些值的后面就行了，又没规定一定要按顺序
    swap(arr, low, index - 1);
    return index - 1; //最后基准值所在下标
}


void quick_sort_main(int *arr, int low, int high) {
    /*
    * 一开始我们对 arr[0]~arr[len-1]进行基准排序，得到最后基准值所在下标index
     * 此时这个位置[index]与最中排序结果相一致，所以我们以index为界
     * 对左分区low~index-1,和右分区index+1~high分别进行基准排序，直到所有分区找到了基准值的最后位置
     */
    if (low < high) { //如果low=high,那么这个分区只有一个元素，low>high则这个分区没有元素
        if (high - low + 1 < 10) { //优化2,在数组大小不大时，可以使用直接插入排序，对随机数组有一定影响
            return direct_insert_sort(arr + low, high - low + 1); //arr+low是数组在那部分的起始地址
        }
        int pivotIndex = pivot_sort(arr, low, high);
        int rightPartIndex = rightIndex; //pivotIndex~rightPartIndex区域是重复元素区域，不需考虑
        //尾递归，其实就是将递归调用表达式写在return中，编译器优化的
        return quick_sort_main(arr, low, pivotIndex - 1), quick_sort_main(arr, rightPartIndex + 1, high);
    }
}

void quick_sort(int *arr, int len) {
    quick_sort_main(arr, 0, len - 1);
}

//此时arr数组中的值，大小顺序为 mid<=low<=high
void getPivot(int *arr, int low, int high) {
    int mid = low + ((high - low) >> 1);; //听说移位操作更好，都差不多
    if (arr[low] > arr[high]) //保证 low < high
        swap(arr, low, high);
    if (arr[mid] > arr[high]) //保证 mid < high
        swap(arr, low, high);
    if (arr[low] < arr[mid]) //保证 low > mid
        swap(arr, low, mid);
}
