//
// Created by HP on 2020-11-28.
//

#ifndef CLIONCPP_BTREE_H
#define CLIONCPP_BTREE_H
/*
 * B树也是一种查找树，只不过树中的节点可以有多个孩子，可以有多个数据
 * B树的阶是树中最多孩子节点的孩子个数, 假如树中最大有 4-,那么阶为4
 * 比如 2-(有两个孩子，一个数据):    child1Ptr, data, child2Ptr, [ child1 < data, child2 > data]
 * 3-(三个孩子，2个数据) :  child1, data1, child2, data2, child3, [child1<data1, data1 < child2 <data2, child3>data2, data1 < data2]
 * n-(n个孩子，n-1个数据)
 * 其中childPtr可以指向任意节点(n-),可以指向2-,3-....
 * 标准的B树节点，往往会在节点第一个补充数据，记录数据个数
 * 其中树中的节点，假如一个 2-节点，那么2-节点的2个孩子要么同时有，要么同时没有
 * 树中的节点都是由分裂、融合（即n-之间可以相互转化）而来，root除外
 * B树的查找也很简单，与节点中的data比较，那么找出下一个指针域，以此类推，比如在3-查找时，如果key<data1,那么查找child1,如果data1<key<data2,在child2...
 *
 * B+树则是对B树的进一步补充，都差不多，只不过在每个孩子节点中添加了指向其兄弟的指针和其父节点中刚好大于或小于该孩子节点的data
 * 高阶B树则适合于外存的查找，因为外存一次读取需要尽量多读取一些信息
 *
 1、2-3树是3阶B树，树中只有 2-,3-;
 （1）树空时，插入 10， 生成2-节点node-2,node-2.data =10
  (2)假如查找停留在了一个 2-节点,即两边孩子均为null, 此时只需要将此2-节点--> 3-节点，两个data保持大小顺序 （融合）
  (3)查找停留在3-节点，此时 3-节点没有parent, 那么将data1,data2,key,按大小排列，假如是 data1,key,data2,为每个数生成一个2-
    其中 key所在 2-.child1指向data1, child2指向data2   (分裂)

  (4)查找停留在3-（node1），其parent为2-(node2)，假如 node2.child2 = node1， 说明 node1的数都大于node2,key查找停留在node1,说明key>node2
    还是和上面一样，node1和key分为三个2-，假设中间数的2-为node3, 那么此时的 node3需要和node2融合为一个3-，假设为node4
    那么 node4.data1 = node2.data, node4.child1 = node2.child1
        node4.data2 = node3.data, node4.child2 = node3.child1, node4.child3 = node3.child2

  (4)停留在 3-,node1,  parent也为3-,node2, node2.child1 = node1
    对node1分裂为 3个2-，中间数2-为node3

    此时node3要与node2融合，由于node2是3-，所以也要分裂
    node3就保持现状，由于node3<node2.data1, 那么node2+key的中间数必然是 node2.data1
    只需让 node2.data2生成一个2-称为node4,  node4.child1=node2.child2
    node4.child2 = node2.child3，  而中间数node2.data1的2-称为node5, node5.child1= node3, node5.child2=node4

    然后再看原先node2的parent是否为2-，如果是，只需让 node5 与 该2-融合为3-,处理已经讲过了
    如果parent还是3-, 那么按照上面那几行的步骤重新构造，直到遇到2-,或者说遇到root,如果root此时是3-,那么分裂出来的中间数2-充当root，往上盖一层


 按照这个步骤插入数据，最后生成的树，一定是满的
 至于删除操作，自己去查找相关资料
 2-3树在最坏情况下仍有较好的性能。每个操作中处理每个结点的时间都不会超过一个很小的常数，且这两个操作都只会访问一条路径上的结点
 所以任何查找或者插入的成本都肯定不会超过对数级别。
完美平衡的2-3树要平展的多。例如，含有10亿个结点的一颗2-3树的高度仅在19到30之间。我们最多只需要访问30个结点就能在10亿个键中进行任意查找和插入操作。

 同样的还有 2-3-4树，都差不多原理，分裂则是分为 2-,3-，融合就不多说

 这里的红黑树是指与2-3-4树等价的二叉查找树

 2、红黑树: 通过二叉查找树表示2-3-4树，  还有一种简化红黑树，即红节点只处于左孩子，与2-3树等价
 红黑树的时间复杂度为: O(lgn)

 假如node1有两个红色节点孩子，那么这三者形成 4-, 如果只有1个，那么形成3-,只有黑色，则是普通的2-
 即可以将红色节点与其父节点组装在一起形成 3-/4-,而原来红色节点的两个孩子在3-/4-也有一席之地
 假如有两个红色孩子，那么形成的4-节点，中child1,child2即是左红的两个孩子，child3,child4则是右红的两个孩子
 如果只有一个红色孩子（右），那么形成的3-节点，child2,child3则是右红的两孩子

如果将红黑树中的红节点放置到与父节点一个高度，即本应该在右下位置的红孩子，放在与父节点的正右方，这时平铺而成的红黑树很容易看出原本的2-3-4树结构

 规则：，节点中另外附加 bool color,true红
(1) 每个节点或者是黑色，或者是红色。
(2) 根节点是黑色。
(3) 每个叶子节点是黑色。 [注意：这里叶子节点，是指为空的叶子节点！]--比如 node.left=null,这个node.left才叫叶子
(4) 如果一个节点是红色的，则它的子节点必须是黑色的。  (红节点已与其parent组装，子节点不能与红节点组装)
(5) 从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。
 一棵含有n个节点的红黑树的高度至多为2log(n+1).

 在过程中
 *
 */

/*
 * 以下实现代表只是为了表达红黑树中节点的添加、删除大概操作，其中有些操作没有实现，实际上在平衡二叉树中讲过了
 */
struct RBnode {
    int data = 0;
    RBnode *leftChild = nullptr, *rightChild = nullptr;
    bool color = true;
};
RBnode *root;


//对当前节点node进行操作
void dealWithNewNode(RBnode *node) {
    RBnode *parent = findParent(node->data); //找到当前节点的父节点

    //如果当前节点是根节点，那么只需将根节点颜色改为黑色即可
    if (parent == nullptr) {
        node->color = false; //根节点改为黑色
        return;
    }

    //如果存在父节点，并且是黑色父节点，啥也不用做，维持红色
    if (!parent->color) //融合,parent节点就是一个2-,或3-（另一个孩子）
        return;  //do nothing


    //当父节点为空色时，有三个情形：1、叔叔节点红色  2、叔不红,当前节点为父的右孩子    3、叔不红,当前节点为父的左孩子
    RBnode *grandParent = findParent(parent); //找到父节点的父节点，即祖父节点，不用担心没有(root才没有,但root是黑色，已经判断)
    RBnode *parentBro = (grandParent->leftChild == parent) ? grandParent->rightChild
                                                           : grandParent->leftChild; //父节点的兄弟节点，即叔叔

    bool side = (node == parent->leftChild); //如果当前节点是父节点的左孩子，即true,右孩子false


    bool parentBroColorIsRed = false; //考虑到没有叔叔节点，就默认黑色
    if (parentBro)
        parentBroColorIsRed = parentBro->color;

    //1\叔叔是红色 ： 将叔叔、父节点设为黑色，祖父节点设为红色，以祖父节点为新的"当前节点"进行递归操作
    if (parentBroColorIsRed) {
        //叔红，说明 parent,bro,grand原本就是4-
        //按照2-3-4树的理解 ： 4-分裂为2-,2-,3-  ,3-就是插入的node和其父节点parent
        parent->color = false;
        parentBro->color = false;
        grandParent->color = true; //分裂出来的中间数node试着与上层融合
        dealWithNewNode(grandParent); //grandParent则是分裂出来的中间数
    } else if (!parentBroColorIsRed && side) { //2、叔不红，当前节点是左孩子
        //父设为黑，祖父设为红，对祖父进行右旋
        parent->color = false;  //实际上 node,parent,grandparent此时就是一个4-
        // 为了让三者在树中表示，只需让node,grandParent成为parent的两个孩子(右旋Grand),然后node,grand都是红色
        grandParent->color = true;
        rightTurn(grandParent); //不需要考虑节点的孩子的BF值是否异号，直接右旋
    } else if (!parentBroColorIsRed && !side) {//2、叔不红，当前节点是右孩子
        //实际上parent,node,grand就是一个 4-，将其转为两个红孩子的结构,左旋是为了使三者在一条线上
        RBnode *newNode = parent;
        leftTurn(newNode); //对父节点左旋，左旋后，父节点parent是node的孩子了，但parent的数据不变，所以还是能通过key查找
        dealWithNewNode(newNode);  //原来的父节点作为当前节点，继续处理,此时newNode的parent是node
        //转为了上面的第二种情况
    }

}

//红黑树添加
template<typename T>
void addNodeInRedBlackTree(int key) {
    if (findData(key)) //存在key节点
        return;
    RBnode *parent = findParent(key); //找到key要插入的父节点
    RBnode *node = createNode(key); //创建新节点，默认红色
    if (parent) {
        //正常将新节点插入到相应位置
        if (key > parent->data)
            parent->rightChild = node;
        else
            parent->leftChild = node;
    } else
        root = node; //作为根结点

    dealWithNewNode(node); //对当前节点node进行操作
}

#endif //CLIONCPP_BTREE_H
