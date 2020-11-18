//
// Created by HP on 2020-11-15.
//

#ifndef CLIONCPP_KMP_H
#define CLIONCPP_KMP_H

//计算字符串str的next数组, 规定next[0] = -1, next[1] = 0;
void calculateNext(const char *str, int *next, int len);

//改良版getNext()
void calculateNextValue(const char *str, int *next, int len);

//只查找子串str2在str1中maxNum个下标，保存在数组result中， 返回总个数, len是可视字符的长度
int KMP(const char *str1, const char *str2, int len1, int len2, int *result, int maxNum);

#endif //CLIONCPP_KMP_H