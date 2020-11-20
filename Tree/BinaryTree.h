//
// Created by HP on 2020-11-19.
//

#ifndef CLIONCPP_BINARYTREE_H
#define CLIONCPP_BINARYTREE_H
/*
 * 二叉树即树中的每个节点只能够最多有2个子节点，且严格分为左孩子、右孩子
 * 斜树：树中的节点只有左/右孩子节点
 * 满二叉树：除了最后一层，每个节点都有2个孩子，最后一层都是叶子
 * 完全二叉树 ： 1、简单来说就是 倒数第二层的节点中，如果某节点有孩子（必须有左孩子），那么必然它同层左边的节点都有2个孩子
 *      除了最后两层，其他层的节点都有2个孩子
 *            2、编程实现来说，使用队列层序遍历时，如果遇到了没有右孩子的的节点，那么队列里面剩余的节点全是叶子节点
 *
 * 二叉树性质： 1、第 i 层最多有 2的i-1次方个节点； 2、i 层的树最多有 2的 i次方 -1个节点
 *          3、 ni(度即孩子个数为i的节点的数量),n0为叶子---->   n2 = n0 -1
 *              推导: 设想一下，树中每个节点都与它的每个孩子节点有一条连接线
 *              那么由下往上思考，每个节点除了根节点都与它的双亲节点有一根连接线
 *              连接线数量 = n0 + n1 + n1 - 1, 即所有节点数量 - 1
 *              如果由上向下思考，每个节点的度即它与孩子节点的连接线总数， 那么 连接线数 = n2*2 + n1
 *              那么 n0 + n1 + n2 - 1 = n2 * 2 + n1 ----> n2 = n0-1
 *          4、节点数为 n 的王全二叉树的深度为 log2(n) + 1（向下取整），推导：设深度为 k, 那么 2^(k-1) < n < 2^k, 两边取log2
 *          5、 完全二叉树中，从根节点开始按层序遍历顺序依次编号，root = 1, 某节点的编号为 i, 树的总结点数 = n
 *              (1) 如果2i>n，则结点i无左孩子（结点i为叶子结点）；否则其左孩子是结点2i。
                (2) 如果2i+1>n，则结点i无右孩子；否则其右孩子是结点2i+1
 */

/*
 * 顺序存储结构 ： 最简单的做法: 将树补全为完全二叉树，层序遍历该树，每个节点的序号即为在数组中的序号，你也可以设计结构体，保留左右孩子的下标，没什么好说的
 *
 */
#include "../StackAndQueue/Queue/SequentialQueue.h"

template<typename T>
struct binaryNide {
    T data;
    binaryNide<T> *leftChild, *rightChild;
};

//前序遍历根为root的子树，原理 ：先遍历根节点、遍历左子树（递归）、遍历右子树（递归）
template<typename T>
void PreOrderTraverse(binaryNide<T> *root, SequentialQueue<T> *queue) {//保存在队列中，方便拿出啦，否则数组还要传下标递归
    if (root != nullptr) {
        queue->queueIn(root->data);
        PreOrderTraverse(root->leftChild); //左子树又作为新的树遍历
        //root的左子树遍历完才会轮到右子树, 同样左子树的子树也是一样的遍历顺序
        PreOrderTraverse(root->rightChild); //不用担心child是null，开头有判断
    }
}

//中序遍历根为root的子树，原理 ：先遍历左子树（递归），然后根节点、遍历右子树（递归）
template<typename T>
void InOrderTraverse(binaryNide<T> *root, SequentialQueue<T> *queue) {
    if (root != nullptr) {
        //一个节点被遍历的前序条件就是他的左孩子被遍历到，并且左孩子遍历完的下一个必然是根节点，然后才是右孩子
        InOrderTraverse(root->leftChild); //左子树作为新的树继续遍历，同样根要在左子树的左子树之后
        queue->queueIn(root->data);
        InOrderTraverse(root->rightChild);
    }
}

//后序遍历，左子树、右子树、根
template<typename T>
void PostOrderTraverse(binaryNide<T> *root, SequentialQueue<T> *queue) {
    if (root != nullptr) {
        //一个节点被遍历的前序条件就是他的左、右孩子都被遍历到，并且左、右孩子遍历完的下一个必然是根节点,优先左孩子
        InOrderTraverse(root->leftChild);
        InOrderTraverse(root->rightChild);
        queue->queueIn(root->data);
    }
}

/*
 * 由遍历结果推导树的构造： 之前我是这么描述前序遍历：根、左子树、右子树，而不是左孩子、右孩子，说明存在某个界限区分左右子树
 *  前序遍历中， 根节点后面必然是左子树、右子树，如何划分左右子树：根据中序中根节点位置
 *  一般题目给出前序、中序，求后续；   前序第一个节点，是整个树的根，然后找到中序中根节点位置，即可区分前序中左右子树
 *  然后再次递归分析左子树、右子树，前序中左子树的部分已经划分出了，那么第一个节点依旧是左子树的根，然后再去中序中左子树部分找左子树根的位置，
 *  然后划分出左子树的左右子树，依次类推，右子树也是一样，树即可还原
 *
 *  前序得知根节点，中序得知左右子树界限
 */

/*
 * 还可以根据队列中的一连串值生成二叉树，比如前序生成的算法跟遍历一样， 先给root赋值，然后 createNode(root.leftChild)\create(root.rightChild)
 * 前提是这树是完全二叉树，否则拿遍历结果还原，不一定能还原成功，同样的遍历结果，有无数种可能
 */

/*
 * 线索二叉树：将二叉树节点中的空的left、child指针指向某种遍历方式下该节点的前驱和后继
 * 比如中序线索二叉树，某节点的前驱和后继就是其在中序遍历结果中前面的节点和后面的节点
 * 为了区分child指向的是孩子还是前驱或后继，需要一个flag标识
 */
template<typename T>
struct trackTree {
    T data;
    bool isLeftChild, isRightChild; //如果false就是前驱或后继
    trackTree<T> *leftChild, rightChild;

};

enum childFlag {
    isChild = 0, isPreOrNext = 1
};

//中序线索化，返回最后一个遍历的节点指针
template<typename T>
//root自然是根节点，而pre只需要在过程中指向最后一次的遍历到的节点，所以需要修改指针的值，二级指针
trackTree<T> *InOrderTraverse_Thr(trackTree<T> *root, trackTree<T> **pre) { //初始化，root=根节点，*pre = null
    if (root != nullptr) {
        InOrderTraverse_Thr(root->leftChild, pre);
        //此时pre指向左子树最后一个遍历的节点,即当前root的前驱节点
        if (root->leftChild == nullptr) { //空指针域才需要补充前驱、后继信息
            root->isLeftChild = isPreOrNext;
            root->leftChild = *pre;
        }
        //不管是啥顺序遍历，我们也不会知道下一个遍历的是谁，既然这样，为何前驱和后继要同时得知呢
        // 不如就利用已知信息，设置当前节点root的前驱pre，和前驱pre的后继root,到时自然下一个节点会遍历到，到时root的后继会揭晓
        trackTree<T> *preNode = *pre;
        if (preNode != nullptr && preNode->rightChild == nullptr) { //第一个遍历到的节点的前驱自然是null,所以防止运行出错
            preNode->isRightChild = isPreOrNext;
            preNode->rightChild = root;
        }
        //在这之前pre即是上一次遍历的节点
        *pre = root; //指向当前遍历的节点，与中序遍历的输出类似,不能写在遍历右子树的下面，否则右子树遍历的时候鬼知道前驱是谁
        InOrderTraverse_Thr(root->rightChild, pre);
        return *pre; //此时*pre指向最后一个遍历的节点
    }
}
//前序、后续线索化的原理大致一致,那两个if和pre写在遍历当前节点root的前面即可， 你把两个if遮住理解一下
//线索二叉树的遍历， 先给二叉树加个头结点，其leftChild指向树的根节点，rightChild指向最后一个遍历的节点，同样最后一个节点的后继为头结点
#endif //CLIONCPP_BINARYTREE_H
