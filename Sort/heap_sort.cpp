#include "declaration/heap_sort.h"


/*
 * 堆排序：不稳定
 * 算法实现基于完全二叉树，准确来说是堆
 *
 * 堆：树中每个节点的值都大于(小于)或等于其叶子节点的完全二叉树
 * 其中 parent >= child 称为大顶堆， parent <= child，称为小顶堆
 * 很明显，大顶堆的根节点是最大值，小顶堆的根节点则是最小值
 *
 * 堆中我们只能知道，根节点最大/最小，但其他元素的大小顺序无法判断, 即数组中的元素只有[0]是确定的，其他都是不确定的
 *
 * 完全二叉树的性质： 完全二叉树中，从根节点开始按层序遍历顺序依次编号，root = 0, 某节点的编号为 i, 树的总结点数 = n
 *              (1) 如果2i + 1 > n-1，则结点i无左孩子（结点i为叶子结点）；否则其左孩子是结点2i+1。
                (2) 如果2i + 2 > n-1，则结点i无右孩子；否则其右孩子是结点2i+2
也就是可以将一个完全二叉树按照层序遍历顺序用一个数组来表示 node[n],其中 i 下标节点的左右孩子下下为 2i + 1， 2i+2(如果有的话)，根节点为[0]

    目标：升序排列，从小到大，(如果是降序排序，使用小顶堆即可)
    所以我们可以根据初始数组序列构造一个大顶堆，然后交换 [0],[n-1],这时数组中的最后一个值是最大的了，之前[n-1]不一定是最小
    然后再根据[0]~[n-2]这部分序列构造新的大顶堆，然后交换[0],[n-2]，依次类推
    直到数组全部排列完成

    调整一个大顶堆时，我们会依次对每个非叶子节点进行调整，即让每个非叶子节点为根的子树就是大顶堆，叶子节点本身可视为大顶堆
    当我们对node为根的子树调整时，如果某个节点 node的两个子树都是大顶堆，那么此时如果child1.data = max(node,child1,child2) 那么交换node和child1的值
    然后再对child1为根的子树进行调整，依次类推，如果没有发生交换，则可以结束

    如果按照层序遍历顺序，倒序从第一个非叶节点开始调整即 即在数组中下标为(arr.len/2-1),之前的包括它都是非叶子节点
    按照这个顺序调整的话，必然可以使得每次都能满足节点node的两个子树都已经调整完成，对node调整时如果发生child交换只需对一个child继续调整
    而不需要考虑另外一个未发生交换的child

    我们可以设计一个递归函数 heapAdjust(arr,parent,len); //对parent为根的子树调整大顶堆， 函数内部如果发生了交换，会递归调用 headAdjust(arr,child,len);
    主函数调用时 for(int i = arr.len/2 -1; i >= 0; --i)  heapAdjust(arr,i,len);   即为构造初始大顶堆，因为所有node的子树都可能不是大顶堆

    第一次取大值时，当我们交换root[0]和[n-1]后，这时可以简单的调用 heapAdjust(arr,0,len-1);
    因为我们交换的是 root 和 最后一个叶子节点，这时整棵树中只有一个子树是不符大顶堆的，即root，所以只需对root为起始点开始调整就行，root的两子树都是大顶堆，没变
    在heapAdjust中如果root与其中的一个child交换，那么继续递归调用headAdjust(,child,);

    以后每次取root，都可以这样快速构造出新大顶堆
 */

//对以parent为根的子树进行大顶堆调整,给出大顶堆序列arr,和长度len
//请确保parent的两个子树都是大顶堆
void heapAdjust(int *arr, int parent, int len) {
    int leftChild = 2 * parent + 1; //左孩子下标
    if (leftChild > len - 1) //不存在左孩子，即叶子节点，直接返回
        return;
    int rightChild = 2 * parent + 2; //右孩子下标
    int changeChild = leftChild; //与parent发生交换的child下标
    if (rightChild <= len - 1 && arr[leftChild] < arr[rightChild])
        changeChild = rightChild; //两个子树的最大值
    if (arr[changeChild] > arr[parent]) { //如果真的存在交换
        //交换parent 和 child
        swap(arr, changeChild, parent);
        heapAdjust(arr, changeChild, len); //当两个子树都是大顶堆时，交换parent,child,只需对交换的child子树进行调整
    }
}


//堆排序,升序排序，初始序列arr，及长度len
void heap_sort(int *arr, int len) { //降序排序只需改一下heap_adjust的大小判断即可
    //构造初始大顶堆,从第一个非叶子节点开始逆序调整，这样的顺序能保证i子树的两个子树必然是大顶堆
    for (int i = len / 2 - 1; i >= 0; --i) {
        heapAdjust(arr, i, len);
    }
    for (int j = 0; j < len - 1; ++j) { //共进行len-1次，每次取root放在数组后面，然后再次构造大顶堆
        swap(arr, 0, len - 1 - j); //取最大值，放在数组后面
        heapAdjust(arr, 0, len - 1 - j); //数组序列逐次减一,重新构造大顶堆
    }
}