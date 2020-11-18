//
// Created by HP on 2020-11-12.
//

#ifndef CLIONCPP_BACKEXPRESSION_H
#define CLIONCPP_BACKEXPRESSION_H

//中缀表达式、后缀表达式接收的空间的地址
void getBackExpression(char *middleExpression, char *backExpression);

//根据后缀表达式计算结果
int calculateByBackExpression(char *backExpression);

#endif //CLIONCPP_BACKEXPRESSION_H
