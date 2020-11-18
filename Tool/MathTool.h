//
// Created by HP on 2020-11-12.
//

#ifndef CLIONCPP_MATHTOOL_H
#define CLIONCPP_MATHTOOL_H

//普通函数的声明和定义最好不要直接写在头文件中，否则多个文件引用其时会造成重复定义


//获取优先级数
int getPriority(char ch);

//c1的优先级Priority是否比c2大
bool isBetterOREqual(char c1, char c2);

bool isLeftBracket(char ch);

bool isRightBracket(char ch);

bool isNum(char ch);

//+ - * /
bool isMathSymbol(char ch);

//是否是数字或+-/*()的之一
bool isNumAndMathSymbol(char ch);

//获取字符对应的int值 '1'-->1
int getNum(char ch);

//返回list数组中最大数
int getMax(const int *list, int len);

//操作数1、操作数2、运算符
int calculate(int x, int y, char op);

#endif //CLIONCPP_MATHTOOL_H
