#include <cstring>
#include "../SequentialStack.h"
#include "../../../Tool/MathTool.h"

//
// Created by HP on 2020-11-12.
//
//中缀表达式转后缀表达式
void getBackExpression(char *middleExpression, char *backExpression) {
    int len = strlen(middleExpression);//字符串长度
    SequentialStack<char> stack; //顺序结构栈, 高效
    int index = -1;
    int backIndex = 0;
    char top; //栈顶字符
    while (++index < len) { //循环中缀表达式
        top = '\0';
        char ch = middleExpression[index];
        if (isNum(ch)) { //如果是数字字符，就直接输出
            backExpression[backIndex++] = ch;
            if (!isNum(middleExpression[index + 1])) //如果是连续的数字，不需要隔开它们
                backExpression[backIndex++] = ' ';
        } else if (isNumAndMathSymbol(ch)) { //如果是其他字符就跳过
            //处理 + - * / ( )
            //如果栈是空的，或当前字符是'(',不需进行任何操作，直接进栈
            if (!stack.isEmpty() && !isLeftBracket(ch)) {
                stack.getTop(&top); //获取栈顶字符
                //如果当前字符是')',就要将栈中'('上面的所有元素包括其其自身全部出栈，而 )不入栈，当然也不输出
                //于是我将(、)的优先级设为最小值，就可以基本运用一样的处理逻辑了
                while (isBetterOREqual(top, ch)) {  //只有当栈顶元素的优先级 < 当前字符时才停下，亦或者栈空时
                    stack.pop(&top); //出栈并输出
                    if (!isLeftBracket(top)) {  // 左括号出栈不需要输出
                        backExpression[backIndex++] = top;
                        backExpression[backIndex++] = ' ';
                    }
                    //栈空时或者(、)匹配完成或者栈顶元素优先级 < 当前字符时就可以结束了
                    if ((isRightBracket(ch) && isLeftBracket(top)) || stack.isEmpty())
                        break;
                    stack.getTop(&top); //获取新的栈顶
                }
            }
            //右括号不入栈
            if (!isRightBracket(ch))
                stack.push(ch);
        }
    }
    //遍历中缀表达式后，栈可能还没空，此时直接全部出栈输出
    while (!stack.isEmpty()) {
        stack.pop(&top);
        backExpression[backIndex++] = top;
        backExpression[backIndex++] = ' ';
    }
    backExpression[backIndex] = '\0';
}

//根据后缀表达式计算结果
int calculateByBackExpression(char *backExpression) {
    int len = strlen(backExpression);
    int index = -1;
    char ch;
    int value; //连续数字字符的值
    //数字字符连续读入的话，我们不知道其最终长度，前几位字符的权值就会不断变大，此时用栈可以很好的解决，因为最大位最先进去
    SequentialStack<char> numStack; // 用来计算连续数字字符的值，“12”-->12
    SequentialStack<int> calculateStack; //用来计算四则运算的值
    while (++index < len) {
        //大概原理: 依次遍历，如果是数字则入栈，如果是运算符，则将栈中的前两个元素依次出栈，分别作为操作数x、y(初始栈顶)
        // 计算 result = x op y， 将结果入栈，直至遍历完成
        ch = backExpression[index];
        if (!isNumAndMathSymbol(ch))
            continue;
        if (isNum(ch)) {
            //先统一push，当下一个不是num时就全部pop，如此一来，单个数字字符或是连续的数字字符都能识别为数字
            value = 0;
            numStack.push(ch);
            if (!isNum(backExpression[index + 1])) {
                int valuePer = 1;
                char temp;
                while (!numStack.isEmpty()) {
                    numStack.pop(&temp);
                    value += getNum(temp) * valuePer;
                    valuePer *= 10;
                }
                calculateStack.push(value); //操作数入栈
            }
        } else { //如果是运算符
            int x, y;
            calculateStack.pop(&y);
            calculateStack.pop(&x);
            int result = calculate(x, y, ch);
            calculateStack.push(result);
        }
    }
    int sum;
    calculateStack.getTop(&sum); //最终栈中只保留一个元素，即结果
    return sum;
}
