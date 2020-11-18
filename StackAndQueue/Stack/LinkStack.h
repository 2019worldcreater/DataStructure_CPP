//
// Created by HP on 2020-11-12.
//

#ifndef CLIONCPP_LINKSTACK_H
#define CLIONCPP_LINKSTACK_H

#include <cstdlib>
#include "../../Tool/LinkNode.h"

/*
 * 栈的应用：四则运算，在考虑算术优先级时，如果使用后缀表达式通过栈计算则会很简单，计算机没有人聪明
 *
 * 栈是一种只允许一边插入和取出的线性表
 *
 *单链表栈中不需要头结点，头指针指向栈顶节点即可
 * 实现和单链表基本一致，只不过需要的功能不同，结构相同
 */
template<typename T>
class LinkStack {
private:
    typedef Node<T> *nodePtr;
    nodePtr top = nullptr; //指向栈顶，没有头结点这个东西
    int length = 0; //栈的长度

public:
    LinkStack() = default;

    ~LinkStack() {
        clearStack();
    }

    //清空栈
    void clearStack() { //没有头结点，所以只需要清空
        if (top != nullptr && length != 0) {
            int len = releaseLinkNodeFrom(&top, length);
            if (top == nullptr && len != -1) {
                this->length = len;
            }
        }
    }

    //栈中有多少个元素
    int getLength() {
        return this->length;
    }

    //栈是否为空
    bool isEmpty() {
        return (this->length <= 0 || top == nullptr);
    }

    //获取栈顶元素
    bool getTop(T *data) {
        if (top != nullptr) {
            *data = top->data;
            return true;
        }
        return false;
    }

    //出栈
    bool pop(T *data) {
        if (top != nullptr) {
            nodePtr next = top->next;
            if (getTop(data)) {
                free(top);
                top = next;
                this->length--;
                return true;
            }
        }
        return false;
    }

    //是否存在该元素
    bool isHave(T data) {
        if (!isEmpty()) {
            nodePtr node = top;
            for (int i = 0; i < getLength(); ++i) {
                if (node == nullptr) {
                    return false;
                }
                if (node->data == data) {
                    return true;
                }
                node = node->next;
            }
        }
        return false;
    }

    //进栈
    bool push(T data) {
        nodePtr node = (nodePtr) malloc(sizeof(Node<T>));
        if (node != nullptr) {
            node->data = data;
            node->next = top;
            top = node;
            this->length++;
            return true;
        }
        return false;
    }
};

#endif //CLIONCPP_LINKSTACK_H
