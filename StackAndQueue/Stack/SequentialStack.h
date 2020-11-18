//
// Created by HP on 2020-11-12.
//

#ifndef CLIONCPP_SEQUENTIALSTACK_H
#define CLIONCPP_SEQUENTIALSTACK_H
//用数组实现栈，缺点当然是空间可能浪费，好处是push pop方便
//数组组成的栈中，栈顶需要top指出其下标，栈底就是[0]元素，当栈空时 top = -1
//栈--杯子，杯底就是[0],杯中的水面[top]
/*
 *
 * 顺序栈还有一种结构： 两栈共享空间，即两个杯子合在一起使用一段空间
 * 如何实现： 将两个杯子的杯盖处合在一起，栈1的栈底就是[0], 栈2的杯底[n-1], n是共用的数组的长度
 * 需要两个指针 top1,top2指向相应的栈顶， push时需指明是往哪个栈push
 * 栈1空时，top1 = -1, 栈2空时top2 = n; 栈满时代表整个数组都满了，此时 top1 + 1 = top2;
 */
#define MAX_STACK_SIZE 50

template<typename T>
class SequentialStack {
private:
    T stack[MAX_STACK_SIZE];
    int top = -1; //栈顶的下标
public:
    SequentialStack() = default;

    //清空栈
    void clearStack() {
        top = -1; //对于普通数值类型数据而言
    }

    //获得栈顶元素->data,成功返回true
    bool getTop(T *data) {
        if (!isEmpty()) {
            *data = stack[top];
            return true;
        }
        return false;
    }

    //出栈，并将栈顶值给data
    bool pop(T *data) {
        if (getTop(data)) {
            top--;
            return true;
        }
        return false;
    }

    //进栈
    bool push(T data) {
        if (getLength() < MAX_STACK_SIZE) {
            top++;
            stack[top] = data;
            return true;
        }
        return false;
    }

    //栈是否为空
    bool isEmpty() {
        return top <= -1;
    }

    //栈中元素个数
    int getLength() {
        return top + 1;
    }
};

#endif //CLIONCPP_SEQUENTIALSTACK_H
