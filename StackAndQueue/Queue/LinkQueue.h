
#ifndef CLIONCPP_LINKQUEUE_H
#define CLIONCPP_LINKQUEUE_H

#include <cstdlib>
#include "../../Tool/LinkNode.h"

/*
 * 这个没啥好说的，普通的单链表结构
 */
template<typename T>
class LinkQueue {
private:
    //为方便操作一致性，引入头结点
    typedef Node<T> *nodePtr;
    nodePtr front; //指向队头
    nodePtr rear; //指向队尾
    int length{};

    nodePtr createNode() {
        nodePtr node = (nodePtr) malloc(sizeof(Node<T>)); //不能返回指向栈内存的指针
        node->next = nullptr;
        return node;
    }

public:
    LinkQueue() {
        nodePtr node = createNode();
        front = rear = node; //一开始都指向头结点
        this->length = 1;
    }

    void releaseQueue() {
        if (front != nullptr) {
            if (int len = releaseLinkNodeFrom(&front, this->length) != -1) {
                this->length = len;
            }
        }
    }

    void cleanQueue() {
        if (!isEmpty()) {
            if (int len = releaseLinkNodeFrom(&(front->next), this->length) != -1) {
                this->length = len;
            }
        }
    }

    void queueIn(T data) {
        nodePtr node = createNode();
        node->data = data;
        node->next = nullptr;
        rear->next = node;
        rear = node;
        this->length++;
    }

    bool queueOut(T *data) {
        if (!isEmpty()) {
            nodePtr node = front->next;
            *data = node->data;
            front->next = node->next;
            free(node);
            this->length--;
            return true;
        }
        return false;
    }

    bool isEmpty() {
        return front->next == nullptr && length <= 1;
    }

    int getLength() {
        return length - 1;
    }
};

#endif //CLIONCPP_LINKQUEUE_H
