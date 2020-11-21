
#include <iostream>
#include "Tree/BinaryTree.h"

using namespace std;


int main() {
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