//
// Created by HP on 2020-11-20.
//

#include <iostream>
#include "Tree/BinaryTree.h"

using namespace std;

binaryNode<char> *create(char c) {
    binaryNode<char> *root = (binaryNode<char> *) malloc(sizeof(binaryNode < char > ));
    root->data = c;
    root->leftChild = nullptr; //必须确保一定是null
    root->rightChild = nullptr;
    return root;
}

trackTree<char> *createThread(char c) {
    trackTree<char> *root = (trackTree<char> *) malloc(sizeof(trackTree < char > ));
    root->data = c;
    root->leftChild = nullptr;
    root->rightChild = nullptr;
    root->isRightChild = isChild;
    root->isLeftChild = isChild;
    return root;
}

int mainSDas() {
    trackTree<char> *root = createThread('c');
    trackTree<char> *root1 = createThread('a');
    trackTree<char> *root2 = createThread('b');
    trackTree<char> *root3 = createThread('d');
    trackTree<char> *root4 = createThread('g');
    trackTree<char> *root5 = createThread('e');
    trackTree<char> *root6 = createThread('f');
    SequentialQueue<char> *queue = new SequentialQueue<char>();
    root->leftChild = root1;
    root1->leftChild = root2;
    root1->rightChild = root3;
    root3->rightChild = root4;
    root->rightChild = root5;
    root5->leftChild = root6;
    trackTree<char> *pre = nullptr; //必须是null，否则线索化
    trackTree<char> *head = createThread('h');
    head->rightChild = InOrderTraverse_Thr(root, &pre); //中序线索化，返回最后一个遍历的节点指针
    head->rightChild->rightChild = head; //头结点，作为一个标准，最后一个节点指向头指针
    head->leftChild = root; //指向树的root
    InOrderTraverse_Thread(head, queue); //b,a,d,g,c,f,e,
    while (!queue->isEmpty()) {
        char c;
        queue->queueOut(&c);
        cout << c << ",";
    }
    free(head);
    delete queue;
    free(root);
    free(root6);
    free(root5);
    free(root4);
    free(root3);
    free(root2);
    free(root1);
    return 0;
}

void testTreeOrder() {
    binaryNode<char> *root = create('c');
    binaryNode<char> *root1 = create('a');
    binaryNode<char> *root2 = create('b');
    binaryNode<char> *root3 = create('d');
    binaryNode<char> *root4 = create('g');
    binaryNode<char> *root5 = create('e');
    binaryNode<char> *root6 = create('f');
    root->leftChild = root1;
    root1->leftChild = root2;
    root1->rightChild = root3;
    root3->rightChild = root4;
    root->rightChild = root5;
    root5->leftChild = root6;
    auto *queue = new SequentialQueue<char>();
    //PreOrderTraverse(root, queue); //c,a,b,d,g,e,f
    //InOrderTraverse(root, queue); //b,a,d,g,c,f,e,
    PostOrderTraverse(root, queue); //b,g,d,a,f,e,c,
    while (!queue->isEmpty()) {
        char c;
        queue->queueOut(&c);
        cout << c << ",";
    }
    delete queue;
    free(root);
    free(root6);
    free(root5);
    free(root4);
    free(root3);
    free(root2);
    free(root1);
}