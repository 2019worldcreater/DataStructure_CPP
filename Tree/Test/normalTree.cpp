//
// Created by HP on 2020-11-18.
//


#include <iostream>
#include "Tree/LinkChildTree.h"

using namespace std;

int normalTreeTest() {
    LinkChildTree<int> *tree = new LinkChildTree<int>();
    cout << "first of all: add root " << tree->addTreeRootNode(3) << endl; //先添加根节点
    cout << "root can't have brother : " << tree->addLeftBrotherNode(tree->getRootIndex(), 2) << endl;
    int root = tree->getRootIndex();
    cout << "root has the first child : " << tree->addChildNode(root, 2) << ",its index = "
         << tree->getFirstChildIndex(root) << endl;
    int firstChildOfRoot = tree->getFirstChildIndex(root);
    int rightBro = 0;
    cout << "firstChildOfRoot has a right brother " << tree->addRightBrotherNode(firstChildOfRoot, 21)
         << ", its index = " << (rightBro = tree->getRightBrotherIndex(firstChildOfRoot)) << endl;
    cout << "right bro parent is Root ? " << (tree->getParentIndex(rightBro) == tree->getRootIndex()) << endl;
    int leftBro;
    cout << "right bro has a left bro" << tree->addLeftBrotherNode(rightBro, 4)
         << ", its index = " << (leftBro = tree->getLeftBrotherIndex(rightBro))
         << ", it also is firstChildOfRoot's right bro :(true1).. "
         << (tree->getRightBrotherIndex(firstChildOfRoot) == leftBro)
         << endl;
    cout << "right bro has no right bro : -1(no)," << tree->getRightBrotherIndex(rightBro)
         << ", means it is root last child :(true1)," << (tree->getLastChildIndex(root) == rightBro) << endl;
    int sonOfSon = 0;
    cout << "firstChildOfRoot has a child," << tree->addChildNode(firstChildOfRoot, 3)
         << ", its index = " << (sonOfSon = tree->getFirstChildIndex(firstChildOfRoot))
         << ", meanwhile, it has no bro, right bro ? (false -1) :" << tree->getRightBrotherIndex(sonOfSon)
         << ", left bro : " << tree->getLastChildIndex(sonOfSon);
    cout << "now root { firstChild {sonOfSon}, leftbro, rightBro}, treeDepth = " << tree->getTreeDepth()
         << " , tree node num = " << tree->getTreeNodeNum() << endl;
    int deadData;
    cout << "unfortunately, sonOfSon has gone, " << tree->deleteLeafNode(sonOfSon) << ", means firstOfChild has "
         << tree->getChildNumOfDataNode(firstChildOfRoot) << " child , we can't get data from dead node any more"
         << tree->getTreeNodeByIndex(sonOfSon, &deadData) << endl;

    tree->changeTreeNodeValue(firstChildOfRoot, 4); //firstChild = 4
    int childIndexOfRoot[tree->getChildNumOfDataNode(root)];
    int realNum = tree->getAllChildIndexByDataNodeIndex(root, childIndexOfRoot);
    int dataNodeValue = 0;
    for (int i = 0; i < realNum; ++i) {
        tree->getTreeNodeByIndex(childIndexOfRoot[i], &dataNodeValue);
        cout << " root has child values " << dataNodeValue << ","; //2，4,21 --> 4,4,21
    }
    cout << endl;

    tree->addChildNode(leftBro, 4);
    tree->addChildNode(leftBro, 4);
    tree->addChildNode(leftBro, 4);
    tree->addChildNode(leftBro, 4);
    int leftBroFirstChild = tree->getFirstChildIndex(leftBro);
    tree->addChildNode(leftBroFirstChild, 4);
    cout << "left bro has " << tree->getChildNumOfDataNode(leftBro) << " child, his fistChild has "
         << tree->getChildNumOfDataNode(leftBroFirstChild) << " child" << ", means leftBro's subTree depth = "
         << tree->getSubTreeDepth(leftBro) << endl;
    /*cout << "left bro 灭族成功? " << tree->deleteSubTree(leftBro) << endl; //leftBro为根的子树中的节点全部删除
    cout << "left bro' family all gone, root has " << tree->getChildNumOfDataNode(tree->getRootIndex())
         << " child, tree has " << tree->getTreeNodeNum() << " node" << endl; // 2 child, 3 node*/

    cout << "avoid leftBro subTree get deleted, left bro rename to RightBro Cbild "
         << tree->CutFormAndMoveTo(leftBro, rightBro) << ", right bro depth = " << tree->getSubTreeDepth(rightBro)
         << endl; //表示rightBro 有了一个子树正是 leftBro(depth = 3)为根的子树
    cout << "leftBro frist child is right bro generation: "
         << tree->isGenerationsOf(leftBroFirstChild, rightBro) << endl;
    cout << "subTree rightBro has " << tree->getSubTreeNodeNum(rightBro) << " node " << endl;
    int subTreeNodeIndex[tree->getSubTreeNodeNum(rightBro)]; //保存
    int realIndexNum = tree->floorPrintSubTree(rightBro, subTreeNodeIndex); //层序遍历
    cout << "right bro floor print : ";
    int data;
    for (int j = 0; j < realIndexNum; ++j) { // 目前整棵树 3{4, 21{4 { 4 { 4 { 4 },4,4,4 } }} }
        tree->getTreeNodeByIndex(subTreeNodeIndex[j], &data);
        cout << data << ",";
    }
    cout << endl;
    tree->clearTree(); //可以再次加根节点,...操作
    tree->destroy(); //树不能再干任何事
    cout << tree->getTreeNodeNum(); //-1
    delete tree;
    return 0;
}