//
// Created by HP on 2020-11-12.
//


#include <iostream>
#include "StackAndQueue/Stack/SequentialStack.h"

using namespace std;
typedef int T; //在这里声明数据类型 T

int SequentialStackTest() {
    T data;
    SequentialStack <T> *stack = new SequentialStack<T>; //new 创建对象实例
    cout << "stack len = " << stack->getLength() << endl;
    stack->push(4);
    cout << "stack len = " << stack->getLength() << endl;
    stack->getTop(&data);
    cout << "top = " << data << endl;
    stack->push(3);
    stack->push(2);
    stack->pop(&data);
    cout << "stack len = " << stack->getLength() << endl;
    cout << "pop = " << data << endl;
    stack->clearStack();
    cout << "empty = " << stack->isEmpty() << endl;
    delete stack;
    return 0;
}
