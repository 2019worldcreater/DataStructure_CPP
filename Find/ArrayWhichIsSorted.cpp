#include <iostream>

using namespace std;

int staticArrayFindBySentry(int *a, int n, int key) { //这里的n是a的长度+1,并且a[0]不使用
    //这是一段及其简单的代码，但其中的智慧也不一般
    a[0] = key; //哨兵
    //通常的查找我们会使用for循环来遍历，但这样做会在每一次检查是否越界 i<n
    int index = n; //只需从后往前查找，直到我们放置的哨兵
    while (a[index != key]) {
        index--;
    }
    return index;
}

//折半查找，数组（从小到大排序）、长度、关键字，返回下标,
int binary_Search(int *data, int n, int key) {
    //当线性表排好序后，只需看中间元素与key的大小关系，如果大于key，则代表在左半分区，小于则是右半分区
    //而low，high则是分区的界限
    int low = 0;
    int high = n - 1;
    while (low <= high) { //可能会相等，当上一次 low-mid-high, 如果low=high还查不到，low就会>high,说明失败
        //int mid = (low + high) / 2;//中间元素，low+hig=h奇数还是偶数都无所谓
        int mid = low + (high - low) * (key - data[low]) / (data[high] - data[low]); // low + value/perValue
        //这个公式综合考虑了key可能的分布位置, 前提是数组分布均匀，这种就像当我从1-1000中找5，我会从中间开始找吗，当然是得出一个大概的分布位置
        //如果分布不均匀，那还不如最初的公式
        int value = data[mid];
        if (value == key)
            return mid;
        else if (value > key)
            high = mid - 1; //key可能在 low~mid-1区域内
        else
            low = mid + 1;//key可能在 mid+1~high区域内
    }
    return -1;
    /*
     * 整个过程如果使用二叉树来描绘
     * 最坏的情况时，可以得知比较次数K = log2(n), n即节点数，k即深度，整个树则是完全二叉树
     */
}

//创造n个长度的斐波那契数列,{0,1,1,...} len = n
void createFibonacci(int *fibonacci, int n) {
    fibonacci[0] = 0;
    fibonacci[1] = 1;
    for (int i = 2; i < n; ++i) {
        fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2];
    }
}

//斐波那契黄金比例查找 {数组、长度、key}
int Fibonacci_Search(int *data, int len, int key) {
    /*
     * 其实就是只用加减法运算的二分查找，二分查找由于使用了除法，效率没有加减法高
     * 此时的mid不再是中间元素，而是位于黄金比例的位置,即 len{low~mid}/len{mid+1~low} 接近黄金比例
     * 要想只用加减法得到黄金比例位置，就需要斐波那契数列，在数列中，k越大，那么fibo[k-1]/fibo[k]就越接近黄金比例0.618
     * 而由于fibo[k]=fibo[k-1]+fibo[k-2], 那么令len{low~mid} = fibo[k-1],len{mid+1~high}=fibo[k-2], 整体长度 high-low + 1 = fibo[k]
     * 当mid<key时,右分区长度 k-=2;  mid > key,左分区 k-=1
     */
    int fibonacci[10]; //0,1,1...34, 数组长度0~34
    createFibonacci(fibonacci, 10);
    int k = 0; //搜索区域长度在斐波那契数列中的下标
    while (len > fibonacci[k]) { //找到数组长度最接近的斐波那契数, 要想分割就需要知道k值
        k++; //如果数组长度超过了某个值，那么就取下一个值
    }
    //如果数组长度小于那个数，就往后补全，否则不好分,全取[len-1]的值，以后如果返回len-1之后的下标，就表示是len-1
    for (int i = len; i < fibonacci[k]; ++i) {
        data[i] = data[len - 1]; //这样或许不太好，因为data数组的区域可能越界，但现在不考虑这些问题
    }

    int mid, low = 0, high = fibonacci[k] - 1;
    while (low <= high) {
        mid = low + fibonacci[k - 1] - 1; //黄金位置 即len{low~mid}=fibo[k-1]，但有个错误，如果high=mid-1
        // 那么low~high不再有fibo[k-1]长度，那么实际上真正的黄金位置早已失效，不过也能忍受这些错误，不影响运行
        //只有high/low=mid时才能保持fibo[k-1/k-2]长度，但这样做的后果就是，low=high时，程序无法结束，且出错
        if (data[mid] == key) {
            if (mid > len - 1)
                mid = len - 1;
            return mid;
        } else if (data[mid] > key) {
            k -= 1; //新的分区长度即 len{low~mid}即 fibo[k-1]
            high = mid - 1;
        } else {
            k -= 2;
            low = mid + 1;
        }
    }
    return -1;
}

int main() {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    cout << Fibonacci_Search(a, sizeof(a) / sizeof(int), 1);
    return 0;
}