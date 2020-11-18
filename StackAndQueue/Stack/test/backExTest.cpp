
#include <iostream>
#include <cstring>
#include "StackAndQueue/Stack/Tool/backExpression.h"

using namespace std;

int backExTest() {
    char expression[] = "3  *(1 + 3) * 2 / (1 *(3 + 2*(1-1)))"; //输入算术表达式, 空格啥的可以接受
    char backExpression[strlen(expression) * 2]; //不可能比中缀的两倍还长
    getBackExpression(expression, backExpression);
    cout << backExpression << endl; //3 1 3 + * 2 * 1 3 2 1 1 - * + * /
    cout << calculateByBackExpression(backExpression); //8
    //SequentialStack<T> *stack = new SequentialStack<T>; //new 创建对象实例
    //delete stack;
    return 0;
}
