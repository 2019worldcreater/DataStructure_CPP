//
// Created by HP on 2020-11-16.
//

#ifndef CLIONCPP_LINKCHILDTREE_H
#define CLIONCPP_LINKCHILDTREE_H

/*
 * 树就是有且仅有一个根节点，每个节点有0或多个孩子节点，除了根节点有且仅有一个双亲节点，兄弟：双亲相同的节点
 * 堂兄弟：双亲不同，但各自的双亲互为兄弟，即 深度一样，双亲不一样；  子孙：某节点的所有孩子、孩子的孩子、孩子的孩子的孩子.....
 * 子树： 每个节点可以看成是一个子树的根节点， 某节点A有3个孩子节点，那么这三个孩子节点又是3个子树的根，而A节点又可以看成一个大子树
 * 而根节点就是最大子树的根， 兄弟节点为根的子树互不相交， 即两子树从根遍历到的节点不可能相同
 * 只要保证节点之间的连线只存在双亲（只有一个）、孩子两种关系即可满足树的定义
 *
 * 节点的度：就是其孩子节点的数量， 树的度即所有节点中的最大度
 * 节点的深度：定义根节点为第1层，孩子节点 + 1， 孩子的孩子 + 2...，   树的深度就是树中节点的最大深度
 * 叶子节点：度为0的节点 ， 内部节点：度不为0的节点（除了根节点）
 */

/*
 * 使用顺序结构、双亲表示法存储树中的节点
 * 如果我们使用数组来存储每个节点，必须在每个存储元素中附加表示节点关系的信息，否则孩子找不妈，妈找不到孩子，兄弟也无法相见
 * 如果存储其孩子节点的下标，那么不同节点有不同个数的孩子，有点浪费，通过分析：每个节点孩子个数可能不同，但双亲节点绝对一样，有且仅有一个（除了根节点）
 * 于是我们可以存储每个节点的双亲节点在数组中的下标parent，而根节点的parent = -1
 * 如果我们需要遍历树的功能呢，那就再附加一个下标firstChild，用以指向其第一个孩子节点（最左边的孩子）， rightBrother 指向节点的右兄弟节点, 没有则为-1
 * 即 Node = {data, parent,firstChild,rightBrother}
 *
 * 总之这样的结构设计并不是一定的，完全取决于需要和耗费的平衡考虑
 *
 */

/*
 * 还有第二种方法: 孩子兄弟表示法，纯链表实现
 * 即 Node{data, Node *firstChild, Node *rightBrother}（有点像二叉树的结构）, 如有需要可加上 Node *parent
 * 但其实这三种 方法，都离不开 firstChild、rightBrother、Parent, 无非用不同结构实现其这种概念而已
 * f\r\p的提出只是为了满足不同的功能并尽可能耗费更小
 */


/*
 * 顺序结构搭配单链表结构的孩子表示法
 *
 * 方案一：
 *      单纯的链表，每个节点有0或多个指针，指针其孩子节点， 耗费空间大
 * 方案二:
 *      同上，不过节点附加一个信息表示孩子个数即度
 * 方案三：孩子表示法
 *      为每个节点分配一个链表，该节点的指针域指向第一个孩子，然后该孩子指向右兄弟，依次类推
 *      即数组保存所有节点Node{data,*firstChild}，每个节点有个指针域firstChild，即单链表的头结点
 *      链表中的节点ChildNode{childIndex,*rightBrother}, 即孩子节点所在数组下标，指向下一个右兄弟节点
 * 方案三：双亲孩子表示法
 *       结构和方案三一样，不过数组中的Node{data,parentIndex,*firstChild}
 *       其实这个就相当于双亲表示法 Node{data,parent,firstChild,rightBrother}的以另一种结构的呈现，非常巧妙
 *  A{ B{C}, D}:
 *       0 {A,-1,Link->1,3}
 *       1 {B,0, Link->2}
 *       2 {C,1,nullptr}
 *       3 {D,0,nullptr}
 */

/*
 * 太难写了，没写完，主要是之前写的 ArrayList和LinkList太垃圾了，可用性太差
 */
#include "../List/LinkList.h"
#include "../List/ArrayList.h"
#include "../StackAndQueue/Stack/LinkStack.h"
#include "../StackAndQueue/Queue/SequentialQueue.h"
#include "../Tool/MathTool.h"

/*struct childNode { //相当于之前我写的 Node<int>,正是LinkList<int>中保存的节点
    int childIndex; //该孩子节点咋数组中的下标
    childNode *rightBrother;
};*/
template<typename T>
struct dataNode { //封装树中的节点
    T data; //树中的数据节点
    int parent; //双亲节点在数组中的下标
    //childNode *firstChild; //LinkList<int> *firstChild 完全能替代，我已经实现了单链表，用就完了, 只要明白其中的概念即可
    LinkList<int> *firstChild; //链表中保存该dataNode节点的孩子节点的下标
};

template<typename T>
class LinkChildTree {
private:
    typedef LinkList<int> *childList;
    typedef dataNode<T> *nodePtr; //数组中如果直接保存dataNode,一开始就分配空间，太浪费了，保存指向dataNode的指针不就行了吗
    //逻辑上数组可以看成保存dataNode，使用时注意一下就行
    ArrayList<nodePtr> *treeList; //maxSize = 100, 链表和数组都可以复用以前的结构了,初始时里面的指针都是Null，我不想一开始就分配空间
    //树中的节点其实被包装了起来，dataNode->data才是树的节点数据,
    int root; //根节点下标
    //存储数组历史中被删掉元素的下标并且该位置未分配给新元素
    LinkStack<int> *indexStack;
    /*
    * 如果从treeList中直接删去这个元素，那么其他元素的下标就会统统靠前一位
    * 那么几乎所有节点的孩子节点的下标都要修改，这是极其不明智的，我们可以保留该元素位置
    * 于是我们可以把删除的节点的下标存储在一个 栈中， 删除就push，如果要addChild
    * 就优先从栈中取位置，使用treeList.change, 将那个nodeptr改为新节点的指针即可，否则正常addItem
    */

    //私有函数的存在是为了公有函数尽可能少的了解实现的底层结构，比如数组和链表的增删改、栈的push/pop

    //创建dataNode节点,赋值data,和 parent下标，并初始化孩子链表，注意nullptr的可能
    nodePtr createDataNode(T data, int parent);

    //返回数组中下标为index的元素，即指向dataNode的指针,可能null
    nodePtr getDataNodeByIndex(int index);

    //返回dataNode的孩子链表的指针
    childList getChildListByDataNodeIndex(int index);

    //往数组treeList中添加新节点的指针，返回其下标,-1失败
    int addDataNodeToArray(nodePtr node);

    //改变数组中dataNode的指针指向
    bool modDataNode(int index, nodePtr node);

    //如果栈中有未被分配的闲置下标，就返回，否则-1
    int getAvailableIndex();

    //返回child节点下标在其父节点parent的孩子链表中的下标, 孩子链表中的值肯定是各不相同的且可对比的
    int getChildNodeIndexInChildListOfDataNode(int parent, int child);

    //返回在parent孩子链表中index下标的孩子坐标节点的信息，即parent 第 index + 1个孩子的下标
    int getChildIndexByChildListIndex(int parent, int index);

    //考虑到可能是删除某个子树时的一个节点删除步骤，所以双亲也要删除时，没必要还告诉它们
    //删除数组中下标index的dataNode, isToldParent表示是否在父节点的孩子链表中删除
    //删除的只能是叶子节点，因为一旦一个内部节点删除时，它的双亲难道就要继承责任吗，当然不可能，也不符合逻辑
    bool deleteDataNode(int index, bool isToldParent);

    //parent节点的孩子链表新增一个节点，孩子下标为childIndex,插入链表的位置 childRelativeIndex
    //考虑到有加右兄弟、左兄弟的情况，封装的函数，因为插入下标不尽相同
    bool addChildIndexToParentChildListInWhere(int parent, int childIndex, int childRelativeIndex);

    //给deleteSubTree调用的递归函数,只有第一次递归时layer = 0的index对应的dataNode才需向双亲节点报告
    //想删除一个根为index的子树，(index,0)即可
    bool deleteSubTree(int index, int layer);

    //返回该child节点的兄弟节点的下标，true代表是右节点，否则为做节点
    int getBrotherIndex(int child, bool isRightBrother);

    //给child节点添加兄弟节点、isRightBrother true->右兄弟,false->左兄弟
    bool addBrotherNode(int child, T data, bool isRightBrother);

    //给parent添加孩子节点，添加的是第index + 1个节点
    bool addChildNodeInWhere(int parent, int index, T data);

    //释放dataNode节点，仅此而已
    void freeDataNode(nodePtr node);


public:
    //树的初始化，无任何节点
    LinkChildTree();

    ~LinkChildTree();

    //返回index为根的子树的深度,-1/0都错误
    int getSubTreeDepth(int index);

    //树初始化是否成功,否则一切操作都不能进行
    bool isTreeInitDone();

    //摧毁树,以后不能再用了
    void destroy();

    //清空树
    void clearTree();

    //是否为空树，初始化失败也一样为true
    bool isEmpty();

    //根节点下标,-1代表没有，即树空
    int getRootIndex();

    //返回parent下标的dataNode的孩子节点的个数,返回-1代表不存在dataNode,0代表没孩子
    int getChildNumOfDataNode(int parent);

    //返回index的dataNode的所以孩子节点的下标给child数组，建议int r[getChildNumOfDataNode(parent)],返回总个数
    int getAllChildIndexByDataNodeIndex(int parent, int *child);

    //树中节点总数, -1代表树初始化失败
    int getTreeNodeNum();

    //child节点是否是parent的子孙后代
    bool isGenerationsOf(int child, int parent);

    //添加根节点
    bool addTreeRootNode(T data);

    //其他的方法都只是取得某节点的孩子、兄弟、双亲在数组中的下标
    //该方法用于通过下标获取真正的数据
    bool getTreeNodeByIndex(int index, T *data);

    //改变 tree[index]的值
    bool changeTreeNodeValue(int index, T data);

    //返回parent所在树节点的第一个孩子节点（最左孩子节点）的在数组中的下标
    // 如果parent不存在节点，或该节点没有孩子，返回-1
    int getFirstChildIndex(int parent);

    //返回parent节点的最右孩子节点下标
    int getLastChildIndex(int parent);

    //获得tree[index]节点的双亲节点下标,错误则返回-1
    int getParentIndex(int index);

    //获得tree[index]节点的右兄弟节点下标, -1代表右兄弟不存在或该节点不存在
    int getRightBrotherIndex(int index);

    int getLeftBrotherIndex(int index);

    //双亲节点下标、数据, 添加的节点为最右子节点，成功->true
    bool addChildNode(int parent, T data);

    //给index节点添加右兄弟节点，如果之前有右兄弟，那么就变成现任右兄弟的右兄弟
    bool addRightBrotherNode(int index, T data);

    //给index节点添加左兄弟节点，如果之前有左兄弟，那么就变成现任左兄弟的左兄弟
    bool addLeftBrotherNode(int index, T data);

    //删除以tree[index]为根节点的整个子树(包括根)， 不能简单的删掉一个节点，就像树一样，你裁掉某个树枝，难道这棵子树还能活下来吗
    bool deleteSubTree(int index);

    //删除叶子节点
    bool deleteLeafNode(int index);

    //tree[index]是叶子节点吗,如果甚至不是节点为false
    bool isLeafNode(int index);

    //虽然不能简单的insertNode到任意位置
    //但还有一个更骚的操作：移花接木
    //将以tree[index1]为根的子树，从原位置删除，并将其移动到tree[index2]叶子节点上去, 子树的根作为该叶子结点的孩子
    //节点child换一个新双亲
    bool CutFormAndMoveTo(int child, int parent);

    //index下标在数组中是否合法，可以认为是此时可不可以从数组中获取index下标的树节点，未初始化时同样false
    //即index处是否有节点
    bool isValid(int index);

    //树的深度
    int getTreeDepth();

    //获得子树SubTree的节点的数量
    int getSubTreeNodeNum(int subtreeRoot);

    //层序遍历子树subTree,将下标保存在nodeIndex中,返回总个数
    //建议int index[getSubTreeNodeNum(subtreeRoot)]
    int floorPrintSubTree(int subtreeRoot, int *nodeIndex);
};

template<typename T>
int LinkChildTree<T>::floorPrintSubTree(int subtreeRoot, int *nodeIndex) {
    //要试现层序遍历，递归不能用了，要讲究先来后到的话， 队列是个不错的选择
    //能保证下一层的节点不会先于上一层的节点
    //将队头的子节点依次入队，结束时将队头出队(赋值)，直到队列为空，初始时队列中只有根节点
    if (isValid(subtreeRoot)) {
        int index = 0, head, childNum;
        SequentialQueue<int> *queue = new SequentialQueue<int>();
        queue->queueIn(subtreeRoot);
        while (!queue->isEmpty()) {
            queue->getHead(&head);
            childNum = getChildNumOfDataNode(head);
            if (childNum > 0) {
                int childIndex[childNum];
                childNum = getAllChildIndexByDataNodeIndex(head, childIndex);
                for (int i = 0; i < childNum; ++i) {
                    queue->queueIn(childIndex[i]);
                }
            }
            queue->queueOut(&head);
            nodeIndex[index++] = head;
        }
        queue->clearQueue();
        delete queue;
        return index;
    }
    return 0;
}

template<typename T>
int LinkChildTree<T>::getSubTreeNodeNum(int subtreeRoot) {
    nodePtr node = getDataNodeByIndex(subtreeRoot);
    if (node != nullptr) {
        if (isLeafNode(subtreeRoot)) { //递归结束条件
            return 1;
        } else {
            int sum = 1; //1很关键，代表节点自身
            childList list = node->firstChild;
            if (list != nullptr && !list->isEmpty()) {
                int index;
                for (int i = 0; i < list->getLength(); ++i) {
                    list->getDataByIndex(i, &index);
                    sum += getSubTreeNodeNum(index); //递归，子树的子树的节点数
                }
            }
            return sum;
        }
    }
    return 0;
}

template<typename T>
bool LinkChildTree<T>::isGenerationsOf(int child, int parent) {
    if (isValid(child) && isValid(parent)) {
        nodePtr node = getDataNodeByIndex(child);
        for (int i = 0; i < getSubTreeDepth(parent) - 1; ++i) { //循化次数不超过parent子树的深度
            if (node == nullptr)
                return false;
            else {
                if (node->parent == parent)
                    return true;
                else
                    node = getDataNodeByIndex(node->parent);
            }
        }
    }
    return false;
}

template<typename T>
int LinkChildTree<T>::getSubTreeDepth(int index) {
    if (isValid(index)) {
        if (isLeafNode(index)) {
            return 1;
        } else {
            int childNum = getChildNumOfDataNode(index);
            int childIndex[childNum];
            int realNum = getAllChildIndexByDataNodeIndex(index, childIndex);
            for (int i = 0; i < realNum; ++i) {
                int child = childIndex[i];
                childIndex[i] = getSubTreeDepth(child); //递归求解子树深度
            }
            return 1 + getMax(childIndex, realNum); //返回所有子树最大深度 + 1(自身)
        }
    }
    return -1;
}

template<typename T>
bool LinkChildTree<T>::deleteLeafNode(int index) {
    return deleteDataNode(index, true);
}

template<typename T>
int LinkChildTree<T>::getTreeDepth() {
    return getSubTreeDepth(root);
}

template<typename T>
bool LinkChildTree<T>::CutFormAndMoveTo(int child, int parent) {
    int index = getChildNodeIndexInChildListOfDataNode(getParentIndex(child), child);
    childList list = getChildListByDataNodeIndex(getParentIndex(child));
    childList parentList = getChildListByDataNodeIndex(parent);
    if (list != nullptr && parentList != nullptr && list->isValid(index)) {
        int i;
        if (list->deleteNodeByIndex(index, &i)) {
            if (parentList->addNode(child)) {
                nodePtr node = getDataNodeByIndex(child); //记得修改双亲下标
                node->parent = parent;
                return true;
            }
        }
    }
    return false;
}

template<typename T>
bool LinkChildTree<T>::addRightBrotherNode(int index, T data) {
    return addBrotherNode(index, data, true);
}

template<typename T>
bool LinkChildTree<T>::addLeftBrotherNode(int index, T data) {
    return addBrotherNode(index, data, false);
}

template<typename T>
void LinkChildTree<T>::freeDataNode(nodePtr node) {
    if (node != nullptr) {
        childList list = node->firstChild;
        if (list != nullptr) {
            list->releaseLinkList();
            delete list;
        }
        free(node);
    }
}

template<typename T>
bool LinkChildTree<T>::addChildNodeInWhere(int parent, int index, T data) {
    if (isValid(parent)) {
        nodePtr node = createDataNode(data, parent);
        if (node != nullptr) {
            int childIndex; //该子节点添加后在数组中的下标
            bool addFine;
            if ((childIndex = getAvailableIndex()) != -1) {
                addFine = modDataNode(childIndex, node); //只需将新节点的地址覆盖原元素即可
            } else {
                childIndex = addDataNodeToArray(node);
                addFine = childIndex > -1;
            }
            if (addFine) { //添加成功，就要更新双亲节点的孩子链表
                //下标节点插入到孩子链表第index个
                return addChildIndexToParentChildListInWhere(parent, childIndex, index);
            } else {
                freeDataNode(node);
                return false;
            }
        }
    }
    return false;
}

template<typename T>
bool LinkChildTree<T>::addBrotherNode(int child, T data, bool isRightBrother) {
    if (child == getRootIndex())
        return false;
    if (isValid(child)) {
        //该节点在双亲节点孩子链表中的下标
        int index = getChildNodeIndexInChildListOfDataNode(getParentIndex(child), child);
        if (isRightBrother) { //插入孩子链表原位置相当于添加左兄弟
            index = index + 1; //插入到链表中该child后面
        }
        return addChildNodeInWhere(getParentIndex(child), index, data);
    }
    return false;
}

template<typename T>
int LinkChildTree<T>::getChildIndexByChildListIndex(int parent, int index) {
    childList list = getChildListByDataNodeIndex(parent);
    if (list != nullptr && !list->isEmpty()) {
        int childIndex;
        list->getDataByIndex(index, &childIndex);
        return childIndex;
    }
    return -1;
}

template<typename T>
int LinkChildTree<T>::getLastChildIndex(int parent) {
    return getChildIndexByChildListIndex(parent, getChildNumOfDataNode(parent) - 1);
}

template<typename T>
int LinkChildTree<T>::getBrotherIndex(int child, bool isRightBrother) {
    nodePtr node = getDataNodeByIndex(child);
    if (node != nullptr) {
        //child在双亲的孩子链表中的下标
        int childIndex = getChildNodeIndexInChildListOfDataNode(node->parent, child);
        childList list = getChildListByDataNodeIndex(node->parent);
        if (list != nullptr && childIndex != -1) {
            int bro;
            if (isRightBrother) {
                //是否合法
                if (list->getDataByIndex(childIndex + 1, &bro))
                    return bro;
            } else {
                if (list->getDataByIndex(childIndex - 1, &bro))
                    return bro;
            }
            return -1;
        }
    }
    return -1;
}

template<typename T>
int LinkChildTree<T>::getLeftBrotherIndex(int index) {
    return getBrotherIndex(index, false);
}

template<typename T>
bool LinkChildTree<T>::modDataNode(int index, nodePtr node) {
    return treeList->changeDataByIndex(index, node);
}

template<typename T>
int LinkChildTree<T>::getAvailableIndex() {
    if (!indexStack->isEmpty()) {
        int index;
        indexStack->pop(&index);
        return index;
    }
    return -1;
}

template<typename T>
int LinkChildTree<T>::addDataNodeToArray(nodePtr node) {
    if (treeList->addItem(node)) {
        return treeList->getIndexOfData(node);
    }
    return -1;
}

template<typename T>
bool LinkChildTree<T>::deleteSubTree(int index, int layer) {
    nodePtr node = getDataNodeByIndex(index);
    if (node != nullptr) {
        int childNum = getChildNumOfDataNode(index);
        if (childNum > 0) { //递归，有孩子接着deleteSubTree
            int childIndex[childNum]; //接收所有的孩子节点的下标
            childNum = getAllChildIndexByDataNodeIndex(index, childIndex);
            for (int i = 0; i < childNum; ++i) {
                deleteSubTree(childIndex[i], layer + 1); //递归，以孩子节点为新子树的根节点删除子树，直到找到叶子结点
            }
        }
        //这些删除都是没有更新双亲节点的孩子链表，所以删除完后，双亲节点也不会识别为叶子节点
        //只能手动清空链表，才能deletDataNode
        childList list = node->firstChild;
        if (list != nullptr)
            list->cleanLinkList();
        if (isLeafNode(index)) { //孩子链表为空即为叶子
            bool isTold = (layer == 0);
            return deleteDataNode(index, isTold);
        }
    }
    return false;
}

template<typename T>
bool LinkChildTree<T>::deleteDataNode(int index, bool isToldParent) {
    if (isLeafNode(index)) {
        nodePtr node = getDataNodeByIndex(index);
        if (node != nullptr) {
            int parent = node->parent; //双亲节点下标
            freeDataNode(node);
            modDataNode(index, nullptr); //将数组中对饮元素的指向为null
            int childIndex; //该index在双亲节点孩子链表中的下标
            if (isToldParent) { //需要更新双亲节点孩子链表
                int linkIndex = getChildNodeIndexInChildListOfDataNode(parent, index);
                childList parentList = getChildListByDataNodeIndex(parent);
                if (parentList != nullptr) {
                    parentList->deleteNodeByIndex(linkIndex, &childIndex);
                }
            }
            indexStack->push(index); //新增空闲下标
            return isToldParent ? childIndex == index : true; //需验证是否删除正确节点
        }
    }
    return false;
}

template<typename T>
bool LinkChildTree<T>::deleteSubTree(int index) {
    return deleteSubTree(index, 0); //只有layer = 0时的index下标的dataNode的parent需要删除其孩子信息
}

template<typename T>
bool LinkChildTree<T>::getTreeNodeByIndex(int index, T *data) {
    nodePtr node = getDataNodeByIndex(index);
    if (node != nullptr) {
        *data = node->data;
        return true;
    }
    return false;
}

template<typename T>
LinkList<int> *LinkChildTree<T>::getChildListByDataNodeIndex(int index) {
    nodePtr node = getDataNodeByIndex(index);
    if (node != nullptr)
        return node->firstChild;
    return nullptr;
}

template<typename T>
int LinkChildTree<T>::getChildNodeIndexInChildListOfDataNode(int parent, int child) {
    childList list = getChildListByDataNodeIndex(parent);
    if (list != nullptr) {
        return list->getIndexOfData(child);
    }
    return -1;
}

template<typename T>
bool LinkChildTree<T>::isTreeInitDone() {
    return treeList != nullptr && indexStack != nullptr; //new成功
}

template<typename T>
bool LinkChildTree<T>::isEmpty() {
    return !isTreeInitDone() || treeList->isEmpty() || getTreeNodeNum() <= 0 || root == -1;
}

template<typename T>
bool LinkChildTree<T>::addTreeRootNode(T data) {
    if (isTreeInitDone() && isEmpty() && root == -1) { //树空且初始化成功时才能加
        nodePtr node = createDataNode(data, -1);
        if (node != nullptr) {
            root = addDataNodeToArray(node);
            return root > -1;
        }
    }
    return false;
}

template<typename T>
int LinkChildTree<T>::getChildNumOfDataNode(int parent) {
    childList list = getChildListByDataNodeIndex(parent);
    return (list != nullptr && list->isLinkListInitDone()) ? list->getLength() : -1;
}

template<typename T>
bool LinkChildTree<T>::addChildIndexToParentChildListInWhere(int parent, int childIndex, int childRelativeIndex) {
    nodePtr node = getDataNodeByIndex(parent);
    if (node != nullptr) {
        childList list = node->firstChild;
        if (list != nullptr)
            return list->insertInto(childRelativeIndex, childIndex);
    }
    return false;
}


template<typename T>
dataNode<T> *LinkChildTree<T>::createDataNode(T data, int parent) {
    auto *node = (dataNode<T> *) malloc(sizeof(dataNode<T>));
    if (node != nullptr) {
        node->data = data;
        node->parent = parent;
        node->firstChild = new LinkList<int>(); //初始化链表，里面都是保存孩子节点下标的节点
    }
    return node;
}

template<typename T>
int LinkChildTree<T>::getTreeNodeNum() {
    //需减去stack中的无用下标数量
    return isTreeInitDone() ? treeList->getLength() - indexStack->getLength() : -1;
}

template<typename T>
int LinkChildTree<T>::getRootIndex() {
    return root;
}

template<typename T>
bool LinkChildTree<T>::changeTreeNodeValue(int index, T data) {
    nodePtr node = getDataNodeByIndex(index);
    if (node != nullptr) {
        node->data = data;
        return true;
    }
    return false;
}

template<typename T>
bool LinkChildTree<T>::isValid(int index) {
    //考虑到删除的下标，treeList中并未删除元素，treeList中合法的下标可能并不合法
    return isTreeInitDone() && treeList->isValid(index) && !indexStack->isHave(index);
}

template<typename T>
int LinkChildTree<T>::getFirstChildIndex(int parent) {
    return getChildIndexByChildListIndex(parent, 0);
}

template<typename T>
int LinkChildTree<T>::getAllChildIndexByDataNodeIndex(int parent, int *child) {
    childList list = getChildListByDataNodeIndex(parent);
    if (list != nullptr && !list->isEmpty()) {
        return list->getAllData(child);
    }
    return 0;
}

template<typename T>
dataNode<T> *LinkChildTree<T>::getDataNodeByIndex(int index) {
    if (isValid(index)) {
        nodePtr node;  //只需声明指针，不需申请空间，因为数组中的nodeptr已经指向了一段空间，我们只需要其地址
        if (treeList->getDataByIndex(index, &node)) { //不论是指针还是数值，想改变值，就要传入其自身的指针
            return node;
        }
    }
    return nullptr;
}

template<typename T>
int LinkChildTree<T>::getParentIndex(int index) {
    nodePtr node = getDataNodeByIndex(index);
    if (node != nullptr) {
        return node->parent;
    }
    return -1;
}

template<typename T>
bool LinkChildTree<T>::isLeafNode(int index) {
    return getChildNumOfDataNode(index) == 0;
}

template<typename T>
bool LinkChildTree<T>::addChildNode(int parent, T data) {
    return addChildNodeInWhere(parent, getChildNumOfDataNode(parent), data);
}

template<typename T>
int LinkChildTree<T>::getRightBrotherIndex(int index) {
    return getBrotherIndex(index, true);
}

template<typename T>
void LinkChildTree<T>::destroy() {
    //有问题哦，indexStack别算进去
    if (isTreeInitDone()) {
        clearTree();
        delete treeList; //此后就不能再用了
        delete indexStack;
        indexStack = nullptr;
        treeList = nullptr;
    }
}

template<typename T>
void LinkChildTree<T>::clearTree() {
    if (!isEmpty()) { //树初始化成功并且长度 > 0
        /*
        * 1、为数组中每个dataNode释放空间
        * 2、摧毁每个dataNode里面的链表，dataNode->data和parent不需手动释放，除非data需要，但我应该只会把数值或结构体赋值给data
        * 除非我又令data指向真正的data，但没必要，dataNode节点的存在就是承载data，只有不确定是否需要初始化空间时，才用指针动态分配
        * dataNode的存在就意味着数据是确定下来的，不想array中的nodeptr,一开始并不会有那么多节点
        */
        nodePtr node;
        for (int i = 0; i < getTreeNodeNum() + indexStack->getLength(); ++i) {
            if (indexStack->isHave(i)) //stack里面的下标是已经delete的了，再delete会出错
                continue;
            treeList->getDataByIndex(i, &node);
            treeList->changeDataByIndex(i, nullptr); //先把数组中原位置改为null
            if (node != nullptr) {
                childList list = node->firstChild;
                list->releaseLinkList();
                delete list; // dataNode初始化时childList是new出来来， 详情见createDataNode()
                free(node); //dataNode是malloc的
                //node = nullptr; 又犯了这种错误，不会影响数组中原值
            }
        }
        treeList->clean(); //刚刚只是
        indexStack->clearStack();
        root = -1;
    }
}


template<typename T>
LinkChildTree<T>::~LinkChildTree() {
    destroy();
}

template<typename T>
LinkChildTree<T>::LinkChildTree() {
    treeList = new ArrayList<nodePtr>(); //后面的步骤不会考虑, isTreeInitDone的关键
    indexStack = new LinkStack<int>();
    root = -1; //一个节点都没有
}

#endif //CLIONCPP_LINKCHILDTREE_H
