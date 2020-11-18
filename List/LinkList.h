/*还有一种链表的形态：静态链表，用数组来模拟链表，即数组中的元素为Node，Node{data、index}; index为下一个Node在数组中的下标
 * 这是为那些没有指针特性的语言准备的：
 * 优点： 相比于链表的增删改，静态链表只需要改index值
 * 缺点：链表长度被限制、失去了顺序数组的特性
 * 主要是理解其巧妙的思想
 *
 * 还存在一些 双向循环链表（fore、next）、单向循环列表（tail->next=top， 经典问题：约瑟夫环）啥的，自己了解就行，举一反三
 * 循环链表中如果不用头指针而用尾指针，那么可以很方便查找尾结点，也可以很方便合并两个链表（一个人的尾指向另一个的头）
 *
 * 不同的链表有其适合的应用领域
 * */

#ifndef CLIONCPP_LINKLIST_H
#define CLIONCPP_LINKLIST_H

#include <cstdlib>
#include "../Tool/Status.h"
#include "../Tool/LinkNode.h"

using namespace StatusTool;

template<typename T>
class LinkList {
private:
    //链表中的节点即 Node<T>,包含数据域data和指针域next, data用于保存真正信息，next指向下一个节点
    typedef Node<T> *nodePtr; //nodePtr === Node<T> *
    nodePtr head = nullptr; //指向头结点的头指针， 有了头节点，有些操作逻辑更适用
    int length = 0; //除了头结点的元素个数

    //返回下标index的节点的指针,失败返回null,不能以此获得头结点
    nodePtr getNodePtrByIndex(int index) {
        nodePtr node = nullptr;
        if (isValid(index)) {
            int i = -1;
            node = head;
            bool isFind = true;
            while (++i <= index) {
                if (node->next == nullptr) { //可能出错
                    isFind = false;
                    break;
                }
                node = node->next;
            }
            node = isFind ? node : nullptr; //每次都成功，才会有 i == index
        }
        return node;
    }

    //在堆中创建链表节点，并赋值数据域, 返回其指针，注意失败的话返回nullptr
    nodePtr createNode(T data) {
        nodePtr node = (nodePtr) malloc(sizeof(Node<T>)); //不能返回指向栈内存的指针
        if (node != nullptr) {
            node->data = data;
            node->next = nullptr;
        }
        return node;
    }

public:
    //下标index是否有效(0~length-1),一版是函数查找数据时调用的，所以head!=nullptr And top->next!=nullptr也算作条件
    bool isValid(int index) {
        //利用短路&&的特点，如果head!=nullptr,其余判断不会进行，所以head->next不会出错
        return isLinkListInitDone() && head->next != nullptr && index < length && index > -1;
    }

    //传入节点Node中存储的T型数据数组, 总个数
    LinkList(T *elements, int length) {
        head = createNode(elements[0]); //初始化头结点，因为一定要传参，所以随便传了个
        if (head != nullptr) {
            //尾插法插入节点
            nodePtr tail = head; //最后一个节点
            int len = length;
            while (length--) { //length次
                tail->next = createNode(elements[len - length - 1]); //尾部节点的next指向新节点, 不申请空间不能用
                if (tail->next != nullptr) {
                    tail = tail->next; //更新尾部节点
                    this->length++;
                }
            }
        }

    }

    LinkList() {
        T item;
        head = createNode(item); //为头结点分配空间
    }

    //链表头结点分配是否成功
    bool isLinkListInitDone() {
        return head != nullptr;
    }

    ~LinkList() {
        releaseLinkList();
    }

    //清空链表，但保留头结点
    void cleanLinkList() {
        if (isLinkListInitDone()) { //top->next可能报错
            if (head->next != nullptr) {
                int len = releaseLinkNodeFrom(&(head->next), this->length); //传入指向指针的指针,才能在内部改变指针的值
                if (head->next == nullptr && len != -1) {
                    this->length = len;
                }
            }
        }
    }

    //清空所有
    void releaseLinkList() {
        if (isLinkListInitDone()) {
            int len = releaseLinkNodeFrom(&head, this->length);
            if (head == nullptr && len != -1) {
                this->length = len;
            }
        }
    }

    //用新值改变旧节点值，旧值赋值给old
    bool modifyDataByIndex(int index, T data) {
        nodePtr node = getNodePtrByIndex(index);
        if (node != nullptr) {
            node->data = data;
            return true;
        }
        return false;
    }

//增加节点到链表后面,true为成功
    bool addNode(T data) {
        nodePtr tail;
        if (length == 0)
            tail = head;
        else
            tail = getNodePtrByIndex(length - 1); //最后一个节点
        if (tail != nullptr) {
            nodePtr node = createNode(data);
            if (node != nullptr) {
                tail->next = node;
                this->length++;
                return true;
            }
        }
        return false;
    }

//删除下标为index的节点，即逻辑上第index+1个，并将节点值赋值给data，成功true
    bool deleteNodeByIndex(int index, T *data) {
        nodePtr preNode = nullptr;
        if (index == 0) //getNode不能返回头结点
            preNode = head;
        else
            preNode = getNodePtrByIndex(index - 1); //删除节点的前一个
        if (preNode != nullptr) {
            if (preNode->next != nullptr) {
                nodePtr node = preNode->next;
                nodePtr next = node->next;
                *data = node->data;
                preNode->next = next;
                free(node); //释放index节点
                this->length--;
                return true;
            }
        }
        return false;
    }

//插入节点到下标index处，原位置的节点靠后一位，成功返回true
//此时的index可以为链表长度，即addNode
    bool insertInto(int index, T data) {
        if (index == length)
            return addNode(data);
        else {
            nodePtr preNode = nullptr;
            if (index == 0)
                preNode = head;
            else
                preNode = getNodePtrByIndex(index - 1);
            if (preNode != nullptr) {
                nodePtr next = preNode->next; //原位置节点
                if (next != nullptr) {
                    nodePtr node = createNode(data);
                    if (node != nullptr) {
                        node->next = next;
                        preNode->next = node;
                        this->length++;
                        return true;
                    }
                }
            }
        }
        return false;
    }

//查找index(0~len-1)节点的数据，成功返回true
    bool getDataByIndex(int index, T *data) {
        nodePtr node = getNodePtrByIndex(index);
        if (node != nullptr) {
            *data = node->data;
            return true;
        }
        return false;
    }

//链表长度，-1代表链表初始化失败，0 代表空链表
    int getLength() {
        return isLinkListInitDone() ? length : -1;
    }

//链表未初始化或空时都为true
    bool isEmpty() {
        return !isLinkListInitDone() || length < 1;
    }

//返回节点中第一个数据与data相等的节点的下标，即他是头结点后面第index+1个，返回值只能是 0~len-1
//推荐返回的下标使用 isValid检验
    int getIndexOfData(T data) {
        int index = 0;
        nodePtr node = getNodePtrByIndex(index);
        while (node != nullptr && node->data != data && node->next != nullptr) {
            node = node->next;
            index++;
        }
        if (node != nullptr && node->data == data)
            return index;

        return -1;
    }

//将链表中所有值按顺序存储在all中，建议 T r[getLength()], 返回总个数
    int getAllData(T *all) {
        if (!isEmpty()) {
            int i = 0;
            nodePtr node = head;
            while (i < getLength()) {
                node = node->next;
                if (node == nullptr)
                    break;
                else
                    all[i] = node->data;
                i++;
            }
            return i;
        }
        return 0;
    }

};

#endif //CLIONCPP_LINKLIST_H
