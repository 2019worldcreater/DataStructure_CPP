//
// Created by HP on 2020-11-27.
//

#include <iostream>
#include "Find/binarySortedTreeAndBalanceTree.h"

using namespace std;


int main() {
    sortedBinaryTree *tree = new sortedBinaryTree(10);
    tree->addValue(11);
    tree->addValue(12); //对10左旋
    tree->addValue(7);
    tree->addValue(8); //先对7右旋，在对10左旋
    tree->addValue(3); //对11右旋
    tree->addValue(2);  //7右旋
    tree->addValue(100);
    tree->addValue(101); //12左旋
    tree->deleteByKey(10); //删除10后，11成为了失衡点，对11左旋
    /*binaryNode<int> *node = tree->findData(8); //别通过*node更改节点数值，否则会出错
    tree->reCalculateHeight(node);
    cout << node->data;*/
    delete tree;
    return 0;
}