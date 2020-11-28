/*
 * 在删除节点实现过程中，我曾想通过修改node的指针的值使双亲节点指向不同节点
 * 这样做就需要二级指针，即对某个指针取地址，但只有root是保存在任何情况下的
 * 如果你使用一个局部指针变量将树中的节点指针赋值给他们，那么这个局部指针的取地址是无效的，因为它实际保存在栈中，而非原本的树上
 * 所以需要直接对 root 或其子树取地址，像  &root,&(root->leftChild),...
 * 当然无限长的 root->l->l->...弄不出来， 但递归可以解决，参数中需要使用二级指针nodePtr *, 因为参数的传递是传值的,即创建一个局部变量
 * 如果使用二级指针，那么原本一级指针的值就会存放在原本的位置
 */

#ifndef CLIONCPP_BINARYSORTEDTREEANDBALANCETREE_H
#define CLIONCPP_BINARYSORTEDTREEANDBALANCETREE_H

#include "../Tree/BinaryTree.h"

/*
 * 二叉排序树，即左子树上所有节点都小于根节点，右子树所有节点大于根节点的具有某种排序特性的二叉树
 * 中序遍历的结果是从小到大有序的
 * 比起普通的顺序表和有序表，二叉排序树查找能力还行，并且插入、删除操作很不错
 * 查找时，只需先将key与root比较，如果大于root，比较其右孩子，反之比较左孩子，如果孩子也不等，执行重复上述步骤，util child==null
 * 插入key也是一样的，只需找到child==null时，key应该处于child位置
 *
 *
 *
 * 平衡二叉树(AVL)：前题是二叉排序树， 适合查找多，删除、插入少，因为旋转操作也会消耗性能
 * 我们令叶子节点的height为1，那么双亲节点parent.height=MAX(left.height,right.height)，即以node为根的子树的深度
 * 设定节点node的BF = left.height - right.height, 如果每个节点的 |BF| <= 1, 那么就称该树为平衡二叉树，即左子树与右子树的深度不能相差超过1
 *      为什么需要balance：在二叉排序树中，如果我们不断向树中插入更大（或更小）的key,即0,1,2,3,..,
 *      这时二叉树的形状就会呈现为斜树，也就是一边倒的情况，当我们尝试从这种树中查找时，会由于树的分布不均衡，导致查询效率大大降低
 *      如果分布均匀的话，我们查找的次数会减少很多，所以要让左、右子树的深度相差不能太大，这里我们希望差值不超过1
 *
 * 保证树的平衡，需要在插入key之后，进行判断，然后进行调整
 * 1、确定失衡节点，从key所在节点出发，一路向上，直到遇到 |BF|>1的节点，该节点就是失衡点，没有就算了
 *          失衡节点是离插入节点node最近的BF失衡的节点，如果有，该节点一定是node的双亲的双亲的....该节点为根的子树即最小不平衡子树
            之后我又观察到如果从root朝着node方向行进，如果过程中某个节点 K 是不平衡的，但他的孩子(node方向)是平衡的，那么K就是需要调整的失衡点
            我说错了，在root->node过程中，可能又从平衡->失衡->平衡，所以root不失衡，不代表不存在失衡点！！
 * 2、调整操作：
 *       注意node==root时的操作
 *      （1）BF<-1时，左旋（即给左子树补充）:，假设 node 是失衡点
 *              former=node, node = former.right, node.left = former, former.right = node.left
 *              //以下解释中，node一直都是node
 *              即node的右孩子取代了node所在位置，而node成为了其右孩子的左孩子，考虑到node的右孩子之前有左孩子，所以node的右孩子指向node.r.l
 *              因为node.r.l 是大于node.r小于node的存在,作为node.r很合理
 *       (2)BF>1时, 右旋：
 *              former=node,node=former.left, node.right=former, former.left=node.right
 *              //以下解释中，node一直都是node
 *              即node的左孩子取代了node所在位置，而node成为了其左孩子的右孩子，考虑到node的左孩子之前有右孩子，所以node的左孩子指向node.l.r
 *
 *        (3)上述两种操作存在特例，如果 BF<-1，即左旋时，如果node.right.BF > 0, 那么先将 node.right看成node进行右旋
 *        而 BF > 1,node.left.BF < 0时, 对 node.left进行左旋
 *        如果BF=0不需特殊处理， 处理完左/右孩子后，在重新对node左/右旋
 *        对孩子的左右旋即保证失衡点往右下/左下一定有三个连成一条线
 *
 * 3、删除节点后怎么调整，找到被删除节点K的parent，从parent向上查找直到root,第一个出现失衡的节点即为失衡点，对他进行跟上面一样的额操作即可
 * 如果被删除的是root只需看root是否失衡
 */
class sortedBinaryTree {
    //只需保存int数值
    typedef binaryNode<int> *nodePtr;

private:
    nodePtr root; //根节点

    //创建节点
    nodePtr createNode(int value) {
        nodePtr node = (nodePtr) malloc(sizeof(binaryNode<int>));
        if (node != nullptr) {
            node->height = 1;
            node->data = value;
            node->rightChild = nullptr;
            node->leftChild = nullptr;
        }
        return node;
    }

    bool isNull(nodePtr node) {
        return node == nullptr;
    }

    //返回value所在节点；如果value不存在，那么返回value插入后所在的父节点, side表示插入是作为左/右孩子
    // 将node节点的父节点指针赋值给parent
    //所以要对比一下 node.data,如果不是，说明要插入
    nodePtr findNode(int value, int *side = nullptr, nodePtr *parent = nullptr) { //递归也可以实现
        nodePtr node = root;
        nodePtr parentNode = nullptr; //node的父节点
        while (!isNull(node)) {
            if (value > node->data) { //右孩子
                if (node->rightChild != nullptr) {
                    parentNode = node;
                    node = node->rightChild;
                } else { //value应该插入为node的右孩子
                    if (side != nullptr) {
                        *side = 1;
                    }
                    break;
                }
            } else if (value < node->data) {
                if (node->leftChild != nullptr) {
                    parentNode = node;
                    node = node->leftChild;
                } else {//value应该插入为node的左孩子
                    if (side != nullptr) {
                        *side = 0;
                    }
                    break;
                }
            } else { //找到了value所在node
                if (parent) { //指向parent节点
                    *parent = parentNode;
                }
                return node;
            }
        }
        return node;
    }

    //递归可以返回指向指针的指针，可以以此修改树中指针的值
    //&root,node,key
    void nodePtrPtr(nodePtr *ptr, nodePtr *node, int key);

    //左旋操作，传入失衡点和其父节点
    void leftTurn(nodePtr node, nodePtr parent);

    //左旋操作，传入失衡点和其父节点
    void rightTurn(nodePtr node, nodePtr parent);

    //计算left.height-right.height
    int calBF(nodePtr node);

    //失衡节点是离插入节点node最近的BF失衡的节点，如果有，该节点一定是node的双亲的双亲的....即最小不平衡子树
    //之后我又观察到如果从root朝着node方向行进，如果过程中某个节点是不平衡的，但他的孩子(node方向)是平衡的，那么K就是需要调整的失衡点
    //过程中可能会有 平衡->失衡->平衡的状态，即便root平衡，不代表整棵树平衡
    //加入新节点node时，如果导致node.parent的height变大,才需要考虑是否平衡被破坏
    //那么传递插入的node到参数，从root开始朝node方向一路查看节点的BF值，如果BF值由失衡转为正常，那么将对应节点、双亲的地址赋值给后面两个参数
    //true代表存在失衡点
    bool findUnBalancedNode(nodePtr node, nodePtr *UnbalancedNode, nodePtr *UnbalancedNodeParent);

    //传入插入的节点node，亦或是删除节点的parent，检查树是否失衡，尝试平衡树
    void balanceTree(nodePtr node);

public:

    //重新计算树中每个节点的height
    int reCalculateHeight(nodePtr node);

    //先给出根节点的值
    sortedBinaryTree(int rootValue) {
        root = createNode(rootValue);
    }

    //false存在或错误，true成功
    bool addValue(int value) {
        int side = -1; //插入是作为那边孩子
        nodePtr node = findNode(value, &side); //如果value不存在，node返回的是应该插入节点所在的双亲结点
        if (!isNull(node) && node->data != value && side != -1) {
            nodePtr child = createNode(value);
            if (!isNull(child)) {
                if (side == 1) {
                    node->rightChild = child;
                } else if (side == 0) {
                    node->leftChild = child;
                }

                //以下代码是平衡二叉树的需要

                reCalculateHeight(root); //重新计算树上所有节点的深度,方便计算BF值
                //只有当child的插入使的树的深度变化时
                if (!node->leftChild || !node->rightChild) { //child没有兄弟,说明child的插入可能改变了树的深度
                    //现在开始检查平衡并调整
                    balanceTree(child);
                }
                reCalculateHeight(root);
                return true;
            }
        }
        return false;
    }

    //返回key节点指针,没有返回null
    nodePtr findData(int key) {
        nodePtr node = findNode(key, nullptr);
        if (node->data == key) {
            return node;
        }
        return nullptr;
    }

    ~sortedBinaryTree() {
        nodePtr node;
        SequentialQueue<nodePtr> queue;
        queue.queueIn(root);  //层序顺序删除
        while (!queue.isEmpty()) {
            queue.queueOut(&node);
            if (node->leftChild) {
                queue.queueIn(node->leftChild);
            }
            if (node->rightChild) {
                queue.queueIn(node->rightChild);
            }
            free(node);
        }
    }


    //删除key所在节点
    bool deleteByKey(int key) {
        /*
                * 要从排序二叉树中删除一个节点绝非易事，并且该节点可以是有左右子树的
                * 当一个节点s被删除后，需要一个节点顶替s的位置，而这个节点必然是s子树上的(因为只有s子树上的节点是全部小于或大于s.parent)
                * 还有一种麻烦的做法(不考虑)：把s子树上的节点重新插入....
                * 1\ s.left ==null时, 直接令 s.parent.side(right/left) = s.right, 同理s.right==null, s.parent.side = s.left
                * 2\ 两者都不为null, 那么找到中序遍历序列中 s的直接前驱或后继k,替换s位置即可：我这里说直接前驱的替代方案
                *    直接前驱 K 代表, K大于S左子树上所有数，同时K也小于S右子树上所有数，完美的替补， K一定是不大于S中最大的
                *    很明显直接前驱的位置就是从 s的左孩子一路往右走，最后一个节点即为直接前驱K
                *    此时K可能有左孩子，但绝对没有右孩子，我只需将 K的数据转移到 S 节点上，不需要真正的删除 S
                *    过程中可保持parent是K的双亲，这样，如果parent==s,说明s左子树只有K
                *    那么使parent.left = k.left,否则 k就是parent的右孩子，那么 parent.right=k.left
                *    最后K成为了无用,删除K节点即可
                */
        nodePtr parentOfDeletedNode = nullptr; //被删除节点的父节点
        nodePtr node = findNode(key, nullptr, &parentOfDeletedNode); //待删除节点
        nodePtr replace; //替换节点
        int side = -1; //如果被删除的是根节点，那么需要得知node的side，当node有一个孩子为null时
        if (node->data != key) //没找到
            return false;
        if (parentOfDeletedNode) { //node可能是root
            if (parentOfDeletedNode->leftChild == node)
                side = 0;
            if (parentOfDeletedNode->rightChild == node)
                side = 1;
        }

        if (node->leftChild == nullptr) { //当左孩子null时，node的父节点的原本指向node的相应指针重新指向node的另一个孩子
            replace = node->rightChild;
            free(node); //释放空间
            if (parentOfDeletedNode) { //改变父节点的指针域
                if (side == 1)
                    parentOfDeletedNode->rightChild = replace;
                else
                    parentOfDeletedNode->leftChild = replace;
            } else {
                //考虑根节点的可能，如果是根节点，那么直接子树根节点成为整棵树根节点即可
                root = replace;
            }
        } else if (node->rightChild == nullptr) { //和上面的一样
            replace = node->leftChild;
            free(node);
            if (parentOfDeletedNode) { //代表挺多重复的，唉没办法，算了
                if (side == 1)
                    parentOfDeletedNode->rightChild = replace;
                else
                    parentOfDeletedNode->leftChild = replace;
            } else {
                root = replace;
            }
        } else { //node左右子树都有节点的情况
            nodePtr parent = node; //直接前驱节点的双亲
            replace = node->leftChild; //先指向左孩子,此时不必当心没有孩子
            //有可能node左子树只有replace一个节点，所以parent要指向node
            while (replace->rightChild) { //按照中序遍历规则，此时一路向右下
                parent = replace;
                replace = replace->rightChild;
            }
            //将替代节点的数值传给node，否则删除node挺麻烦的
            node->data = replace->data; //数据转移
            if (parent == node) { //左子树只有replace与replace的左子树
                //replace的左子树取代其位置
                parent->leftChild = replace->leftChild;
            } else {
                //replace的左子树取代其位置
                parent->rightChild = replace->leftChild;
            }
            free(replace); //replace节点无用了
        }


        reCalculateHeight(root);
        //删除节点后，也需要平衡二叉树
        if (parentOfDeletedNode) //有父节点，就以父节点向上查找
            balanceTree(parentOfDeletedNode);
        else //说明删除的是根节点
            balanceTree(root);
        reCalculateHeight(root);
        return true;
    }
};

void sortedBinaryTree::rightTurn(nodePtr node, nodePtr parent) {
    nodePtr former = node;
    if (calBF(former->leftChild) < 0) { //former会右旋，是因为former.bf > 1, 如果former.l.bf < 0，就需要对l左旋
        leftTurn(former->leftChild, former);
    }
    node = node->leftChild;
    nodePtr right = node->rightChild;
    if (root != former) { //former.l替代原本位置
        if (parent->rightChild == former)
            parent->rightChild = node;
        else if (parent->leftChild == former)
            parent->leftChild = node;
    } else {
        root = node;
    }
    node->rightChild = former;
    former->leftChild = right;
}

void sortedBinaryTree::leftTurn(nodePtr node, nodePtr parent) {
    nodePtr former = node;
    if (calBF(former->rightChild) > 0) { //former会左旋，是因为former.bf < -1, 如果former.r.bf > 0，就需要对r右旋
        rightTurn(former->rightChild, former);
    }
    //此时former.r 可能已经换人了
    node = node->rightChild;
    nodePtr left = node->leftChild;
    if (root != former) { //node.right替代原本位置
        if (parent->rightChild == former)
            parent->rightChild = node;
        else if (parent->leftChild == former)
            parent->leftChild = node;
    } else {
        root = node;
    }
    node->leftChild = former;
    former->rightChild = left;
}

//递归计算整棵树上节点的高度,并在过程中改变, 调用 (root)即可计算整棵树
int sortedBinaryTree::reCalculateHeight(nodePtr node) {
    if (!node) //如果不存在改子树返回0
        return 0;
    if (!node->leftChild && !node->rightChild) { //如果是叶子，返回1
        node->height = 1; //左右旋后需要改变
        return 1;
    }
    //否则计算左、右子树的最大值并返回
    int left = reCalculateHeight(node->leftChild); //计算左子树
    int right = reCalculateHeight(node->rightChild);
    node->height = left > right ? left + 1 : right + 1; //最大深度 + 1(自身)
    return node->height; //返回该子树的高度
}

int sortedBinaryTree::calBF(nodePtr node) {
    if (node) { //考虑到节点是null
        int left = 0, right = 0;
        if (node->leftChild)
            left = node->leftChild->height;
        if (node->rightChild)
            right = node->rightChild->height;
        return left - right;
    }
    return 0;
}

bool sortedBinaryTree::findUnBalancedNode(nodePtr node, nodePtr *UnbalancedNode, nodePtr *UnbalancedNodeParent) {
    if (node) {
        //过程中从非平衡-平衡的分界节点即失衡点
        nodePtr parent = nullptr; //失衡点的双亲节点
        nodePtr bNode = root; //失衡点
        bool isBalance = true; //是否开始失衡
        int value = node->data;
        while (bNode) {
            if (isBalance) { //需要找到第一个非平衡点,才开始判断子节点是否平衡
                if (calBF(bNode) > 1 || calBF(bNode) < -1) {
                    isBalance = false;
                    if (bNode->data == value) { //刚找到第一个失衡点，却又正好是node,那么直接返回，因为接下来的判断会认为没找到
                        *UnbalancedNode = bNode;
                        *UnbalancedNodeParent = parent;
                        return true;
                    }
                }
            }

            if (bNode->data < value) {
                //如果开始平衡，那么此时的bNode就是失衡点
                if (!isBalance) { //找到过程中第一个不平衡点才需要判断子节点是否平衡
                    if (calBF(bNode->rightChild) >= -1 && calBF(bNode->rightChild) <= 1) { //|BF|<=1
                        *UnbalancedNode = bNode;
                        *UnbalancedNodeParent = parent;
                        return true;
                    }
                }
                parent = bNode;
                bNode = bNode->rightChild;
            } else if (bNode->data > value) {
                //如果开始平衡，那么此时的bNode就是失衡点
                if (!isBalance) {
                    if (calBF(bNode->leftChild) >= -1 && calBF(bNode->leftChild) <= 1) {
                        *UnbalancedNode = bNode;
                        *UnbalancedNodeParent = parent;
                        return true;
                    }
                }
                parent = bNode;
                bNode = bNode->leftChild;
            } else if (bNode->data == value)
                return false; //都找到了node,说明无非平衡点
        }
    }
    return false;
}


void sortedBinaryTree::nodePtrPtr(nodePtr *ptr, nodePtr *node, int key) {
    if (ptr) {
        nodePtr nodePtr1 = *ptr;
        if (nodePtr1->data == key)
            *node = nodePtr1; //如果
        else {
            if (nodePtr1->data > key)
                nodePtrPtr(&((*ptr)->rightChild), node, key);
            else
                nodePtrPtr(&((*ptr)->leftChild), node, key);
        }
    }
}

void sortedBinaryTree::balanceTree(sortedBinaryTree::nodePtr node) {
    nodePtr unBalancedNode = nullptr, parent = nullptr; //失衡点以及双亲
    if (findUnBalancedNode(node, &unBalancedNode, &parent)) { //从root->node过程中找到失衡点
        if (unBalancedNode) {
            int bf = calBF(unBalancedNode); //确定是左旋还是右旋
            if (bf > 1) //左子树更长
                rightTurn(unBalancedNode, parent);
            else if (bf < -1)
                leftTurn(unBalancedNode, parent);
        }
    }
}

#endif //CLIONCPP_BINARYSORTEDTREEANDBALANCETREE_H
