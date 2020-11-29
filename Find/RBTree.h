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

 红黑树一般是自平衡的，它并不是严格的AVL树，左右子树的相差值可能大于1，但避免许多旋转操作，适合插入、删除较多的情况

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
(5) 从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。(从P节点到以P为根的子树上所有的叶子节点的路径上拥有同样多的黑色节点)
 一棵含有n个节点的红黑树的高度至多为2log(n+1).

 这些特性可以与2-3-4树中的孩子同时有/无特性联动
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


//对当前节点node进行操作，node默认红色,表示要融合进一个节点
void dealWithNewNode(RBnode *node) {
    RBnode *parent = findParent(node->data); //找到当前节点的父节点

    //如果当前节点是根节点，那么只需将根节点颜色改为黑色即可
    //当前节点可能是分裂出来的grandParent，既然无人可融合，那就不融合
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
        grandParent->color = true; //分裂出来的中间数node试着与上层融合，如果grandParent是root，那么在之前的判断中会改为黑，表示向上再盖一层
        dealWithNewNode(grandParent); //grandParent则是分裂出来的中间数
    } else if (!parentBroColorIsRed && side) { //2、叔不红，当前节点node是parent左孩子
        if (parent == grandParent->leftChild) {//parent是grandPant的左孩子
            //父设为黑，祖父设为红，对祖父进行右旋
            parent->color = false;  //实际上 node,parent,grandparent此时就是一个4-,大小顺序也是如此
            // 为了让三者在树中表示，只需让node,grandParent成为parent的两个孩子(右旋Grand),然后node,grand都是红色
            grandParent->color = true;
            rightTurn(grandParent); //不需要考虑节点的孩子的BF值是否异号，直接右旋
        } else { //parent是祖父的右孩子
            rightTurn(parent); //父节点右旋，目的则是使 node,parent,grand一条线，并且grand,node,parent保持原本的大小顺序
            dealWithNewNode(parent); //父节点作为新节点处理
        }
    } else if (!parentBroColorIsRed && !side) {//2、叔不红，当前节点node是parent右孩子
        if (parent == grandParent->rightChild) {
            //实际上parent,node,grand就是一个 4-，将其转为两个红孩子的结构
            parent->color = false;
            grandParent->color = true;
            leftTurn(grandParent);
        } else { //使三者一条线
            leftTurn(parent);
            dealWithNewNode(parent);
        }
    }

}

//先正常添加节点，后面再想办法平衡
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


//红黑树的删除
template<typename T>
void deleteNode(int key) {
    RBnode *node = findNode(key); //待删除节点
    if (!node)
        return;
    dealWithDeletedNode(node);
}

//这里科普一下：红色节点必然同时有两个孩子，或同时没有，红色节点的两个孩子必然是 3-,或4-中的两个孩子，而3-,4-是孩子同时有/无的
//如果是黑色节点，那么黑色节点只有一个孩子的情况只有一种: 一个红色孩子，并且该红色孩子没有孩子
//黑色节点有两个孩子的情况 : 1\两个黑色   2\一个黑色,一个红色(该红色一定有2个黑色孩子)  3\两个红色(这两红一定同时有/无孩子)

//删除的真正处理
/*
 * 1. 红色节点
 *      1.1红色叶子
 *      1.2红色非叶子（必然有2黑色子树）
 * 2. 黑色节点(必然有非null兄弟节点)
 *      2.1黑色叶子
 *          2.1.1红色兄弟节点（不太可能存在这种情况，暂时没遇过, 转为2.1.2)
 *          2.1.2黑色兄弟节点
 *              2.1.2.1存在红色远侄子
 *              2.1.2.2存在红色近侄子(转为2.1.2.1)
 *              2.1.2.3父红，兄弟没孩子
 *      2.2黑色非叶子
 *          2.2.1有两个子树
 *          2.2.2只有一个子树（必然是红孩子，且没孩子）
 */
void dealWithDeletedNode(RBnode *node) {
    if (node->color) {  //1.删除的是红色节点
        if (!node->rightChild && !node->leftChild) { //1.1如果是红色叶子节点，只需删除就行
            freeNode(node);  //该红节点与parent组成3-还是4-都无所谓，直接剔除，不影响
            return;
        } else {  // 1.2 红色非叶子
            RBnode *replace = node->leftChild; //找到替代的直接前驱节点
            while (replace->rightChild) //从左孩子一路右下
                replace = replace->rightChild;
            //将代替节点的值赋给删除节点
            node->data = replace->data; //这时删除的节点就转变成了replace, 不必考虑replace还有子树，接下来的replace递归调用会考虑的
            dealWithDeletedNode(replace); //以replace为新的删除节点看待
            return;
        }
    } else { //2.如果删除节点是黑色节点
        RBnode *parent = findParent(node); //删除节点的父节点
        RBnode *bro; //node的兄弟节点
        if (!node->rightChild && !node->leftChild) { //2.1如果是黑色叶子节点
            if (!parent) { //此时根节点直接删除
                freeNode(node);
                return;
            }
            /*
             * 此时存在父节点P，不管P是红是黑，因为存在node这个黑色孩子，所以必然P的另一个孩子存在，并且红、黑都有可能
             */
            bro = parent->leftChild == node ? parent->rightChild : parent->leftChild; //node的兄弟节点
            if (bro->color) { // 2.1.1 兄弟节点是红色，(但我觉得 node黑，兄红的情况不存在把!!!!!!!,暂时没遇到过)
                //按照2-3-4树： 假如D为node，P为parent, B为bro， 那么 P B为3-, D是最左边的child1(D为P左孩子)或最右边的child3
                //3-的孩子一定同时有，所有B节点必然也有两个黑色节点(红节点孩子只能黑),其中离D最近的才是在2-3-4树中真正的兄弟节点
                parent->color = true; //父节点改为红
                bro->color = false; //bro变黑
                if (parent->leftChild == node) //如果node是左孩子，对parent左旋
                    leftTurn(parent);
                else
                    rightTurn(parent); //对parent右旋
                //实际上上面的代码执行后，对 D P B(Bl,Br) 三者间在2-3-4树中的结构没改变
                //实际上此时的D的new bro才是在2-3-4树中真正的bro
                dealWithDeletedNode(node); //此时node的兄弟节点变成了黑色
                return;
            } else { //2.1.2  兄弟节点是黑色
                //实际上此时bro如果有孩子，那么一定是红孩子, 为了不违法特性(5)，且红孩子都没孩子
                RBnode *distantNephew; //兄弟节点中离node最远的孩子
                RBnode *closeNephew;
                if (node == parent->leftChild) {
                    distantNephew = bro->rightChild;
                    closeNephew = bro->leftChild;
                } else {
                    distantNephew = bro->leftChild;
                    closeNephew = bro->rightChild;
                }
                //先看看远侄子是否为红色，如果不是即null，那就看近侄子是否为红色
                if (distantNephew) { //2.1.2.1 如果bro这个位置存在孩子节点
                    if (distantNephew->color) { //如果远侄子是红色
                        //此时bro的另一个孩子节点必然是null或者红色节点
                        //并且这两个孩子节点必然没有子节点,如果有(且是黑色)，违反了(5)特性,因为从parent到叶子节点要经过一样多的黑色节点

                        //假如要删除 node
                        //实际上以下步骤是从原先2-3-4树中 node的兄弟节点bro(3-,4-)中取出中间数即bro取代parent原先的位置(bro成为2-,或者3-)
                        //然后parent取代node的位置，很合理，如果bro有另一个孩子K,那么K与P为3-（右/左旋）, 而distantNephew成为了2-
                        bro->color = parent->color; //父、兄节点交换颜色
                        parent->color = false;
                        distantNephew->color = false; //该远侄子改为黑色
                        if (distantNephew == bro->leftChild) //如果node是P的右孩子，对P右旋
                            rightTurn(parent);
                        else
                            leftTurn(parent);

                        freeNode(node);
                        return; //此时直接删掉node即可
                    }
                } else if (closeNephew) { //2.1.2.2 看看近侄子是否为红色节点,实际上很上面的逻辑概念一样，所以这里将这个近红想办法转为远红即可
                    if (closeNephew->color) {
                        //此时只要将bro,close组成的3-换个表示就行，比如close取代bro,bro放在远侄位置，两者颜色互换，不影响原结构,这时按照上面的情况处理
                        bro->color = true;
                        closeNephew->color = false;
                        if (closeNephew == bro->rightChild)
                            leftTurn(bro);
                        else
                            rightTurn(bro);
                        dealWithDeletedNode(node); //按照远侄处理
                        return;
                    }
                }

                //运行到这，说明bro没有孩子，都是null,而这时需判断父节点的颜色
                if (parent->color) {  // 2.1.2.3 红父，node黑，bro黑
                    //这时红父必然是 3-/4-的成员,我们这里设想3-情形
                    //假如 P,K组成3-, 如果删除node,那么P不满足两个孩子同时有，所以3-无法维持，只需将3-变为2-(K),
                    //于是P与另一个孩子组成3-,作为K的孩子
                    parent->color = false;
                    bro->color = true;
                    freeNode(node);
                    return;
                }
                //我觉得三黑的情况 不可能存在吧，毕竟要融合
            }
        } else { //2.2 黑色非叶子节点
            if (node->leftChild && node->rightChild) { //2.2.1 有两个子树时
                RBnode *replace = findRepalce(node);  //意思就是找到node的直接前驱
                node->data = replace->data;
                dealWithDeletedNode(replace);
                return;
            } else { //2.2.2 只有一个子树，必然是红孩子，且红孩子没有孩子
                //即用其红孩子取代node原位置，然后将h红孩子改为黑（直接替换值也可以，不用改颜色）
                RBnode *child;
                if (node->leftChild)
                    child = node->leftChild;
                else
                    child = node->rightChild;
                node->data = child->data; //与孩子的值替换即可
                dealWithDeletedNode(child); //删除child即可
                return;
            }
        }
    }
}

//释放节点
void freeNode(RBnode *node) {
    RBnode *parent = findParent(node);
    if (parent) {
        if (parent->leftChild == node)
            parent->leftChild = nullptr;
        else
            parent->rightChild = nullptr;
    } else {
        root = nullptr;
    }
    free(node);
}

#endif //CLIONCPP_BTREE_H
