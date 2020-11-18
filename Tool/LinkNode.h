//
// Created by HP on 2020-11-12.
//

#ifndef CLIONCPP_LINKNODE_H
#define CLIONCPP_LINKNODE_H
template<typename T>  //template<typename T>   泛型
struct Node {
    T data;  //Node只是载体，T data才是真正的数据
    Node<T> *next;
};

//释放从*ptr（包括）指向的节点开始的链表剩下的所有，返回剩余长度，最后该使*ptr指向null
//start==nullptr返回-1
template<typename T>
int releaseLinkNodeFrom(Node<T> **ptr, int length) { //泛型函数如果分开写定义，那么使用者还要额外include实现文件.cpp
    Node<T> *start = *ptr; //*ptr保存指向node的指针
    if (start == nullptr) {
        return -1;
    }
    Node<T> *node = start;
    Node<T> *next;
    while (node->next != nullptr) {
        next = node->next;
        free(node);             //freeNode后node的值不一定是nullptr，只不过指向的空间释放了而已
        node = next;
        length--;

    }
    free(node);
    length--;
    *ptr = nullptr; //修改指向start节点的指针的值
    return length;
    //如果传入 *start参数
    //start = nullptr; //实际上这样修改不会影响到原值，因为实质上指针也是传值引用，把地址传进来而已，我直接修改地址的值有屁用
    //我这里的本意是修改指针的值，而非指针所指向的数据，所以需要二级指针才能做到
}

#endif //CLIONCPP_LINKNODE_H