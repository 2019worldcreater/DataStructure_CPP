//
// Created by HP on 2020-11-13.
//
#include <iostream>
#include "String.h"
#include "KMP/KMP.h"

int MyString::strlen(const char *str) {
/*
 * 字符编码：ASCII、UNICODE
 * 标准的ASCII编码字符集用一个字节的7位二进制位表示一个字符，即 00000000~01111111, 0-127,供128个字符
 * 恰好是8位补码的所有非负数,那么ASCII字符的整数值必然是0-127
 * 后面又有了扩展的ASCII码，把8位全用了，能表示255个字符，那么其中一些字符转为整数值必然是负数：字符在内存中以整数保存，整数以补码保存
 *
 * 后来Unicode编码方案则为每一个字符定义了属于他的二进制制代码，Unicode只是字符集：一种方案，只定义了其值，没定义如何存储该值（多少字节）
 * 所以真正实践的Unicode编码规则则有 Utf-8,utf-16,utf-32....
 * 比如 UTF-8是变长编码，原先标准ASCII码的内容都一样保存
 *
 * 类似的还有GBK编码
 *
 * 本来打算实现为不同的编码下，相同字符串的长度都一样的功能， strlen("我") = 1, 虽然"我"在不同编码下用不同字节数存储
 * 后来发现C语言自带的strlen函数都做不到，也只能返回总字节数，于是我就放弃了。。。。。。
 * main.cpp的文件编码方式你改了之后，可以调试一下，查看字符串的字符值，编码不同，值也可能会变
 */
    if (isPointAtNull(str)) {
        return 0;
    }
    int index = -1, len = 0;
    char ch;
    while ((
                   ch = str[++index]
           ) != '\0') {
        len++;
    }
    return len + 1;
}

void MyString::clean() {
    if (!isPointAtNull(head)) { //防止destroy后死灰复燃
        destroy(); //先摧毁
        head = createSpaceWithStr(""); //申请一段空间恰好容纳"\0"
    }
}

void MyString::destroy() {
    if (!isPointAtNull(head)) {
        free(head);
        head = nullptr;
    }
}

MyString::MyString(const char *str) {
    if (!isPointAtNull(str)) {
        head = createSpaceWithStr(str); //创建空间
    }
}

char *MyString::strCopy(char *str1, const char *str2) {
    if (!isPointAtNull(str1) && !isPointAtNull(str2)) {
        int len = strlen(str2) - 1, index = len;
        while (--len >= 0) {
            str1[len] = str2[len]; //不会复制'\0'
        }
        str1[index] = '\0';
    }
    return str1;
}

void MyString::strCopyFrom(const char *str) {
    if (!isPointAtNull(str) && !isPointAtNull(head)) {
        destroy();//再造一个
        head = createSpaceWithStr(str);
    }
}

char *MyString::getContent(char *str) {
    if (!isPointAtNull(str) && !isPointAtNull(head))
        strCopy(str, head);
    return str == nullptr ? nullptr : str;
}

char *MyString::strCat(char *str1, const char *str2) {
    if (!isEmptyStr(str2) && !isPointAtNull(str1)) { //空串就不用了管了
        char *tail = str1 + strlen(str1) - 1; //str1中'\0'的地址
        strCopy(tail, str2); //相当于tail可看成新的串,然后复制str2过去，自然实现了拼接
    }
    return str1;
}

void MyString::strCatWith(const char *str) {
    if (!isEmptyStr(str) && !isPointAtNull(head)) {
        //可以先生成拼接结果，然后复制给MyString
        char *result = new char[getLength() + strlen(str)]; //正好的空间
        strCopy(result, head); //原字符串
        strCat(result, str); //拼接字符串
        strCopyFrom(result);
        delete[] result;
    }
}

int MyString::strCompare(const char *str1, const char *str2) {
    if (isPointAtNull(str1) || isPointAtNull(str2)) {
        return -1;
    }
    //其实就好比英语字典排序，大的字符串排在后面
    int result = 0, len1 = strlen(str1) - 1, len2 = strlen(str2) - 1;
    int maxIndex = len1 > len2 ? len2 : len1, index = -1;
    while (++index < maxIndex) { //逐个比较
        result = str1[index] - str2[index];
        if (result != 0) {
            result = result > 0 ? 1 : -1; //可能差值太大
            break;
        }
    }
    if (result == 0 && len1 != len2) { //考虑到类似 "12" "123"的情形
        result = len1 > len2 ? 1 : -1;
    }
    return result;
}

char *MyString::subString(const char *str, int start, int len, char *str2) {
    if (!isEmptyStr(str) && start <= strlen(str) - 1 && start >= 1 && !isPointAtNull(str2) &&
        len > 0) { //空串截个锤子， start不可以超标，len可以
        int strSize = strlen(str);
        //start后面的字符可能没有len长，所以只需要剩下的子串
        len = (strSize - start) > len ? len : (strSize - start);
        char *temp = new char[strlen(str)]; //复制一个str
        strCopy(temp, str);
        temp[start + len] = '\0'; //截止标志,这样strCopy的时候只会copy'\0'之前的内容
        strCopy(str2, temp + start - 1); //开始位第Start个字符
        delete[] temp;
        return str2;
    }
    return nullptr;
}

void MyString::subString(int start, int len) {
    if (!isEmpty() && start <= getLength() && len > 0 && start > 0) { //本身是空串是就不切了
        int realLen = (strlen(head) - start) > len ? len : (strlen(head) - start);
        char *subStr = new char[realLen + 1]; //head截取结果暂存
        subString(head, start, len, subStr);
        if (!isPointAtNull(subStr))
            strCopyFrom(subStr); //直接覆盖
        delete[] subStr;
    }
}

int MyString::getIndexOfHowMany(const char *str1, const char *str2, int *result, int num) {
    int indexNum = 0;
    if (maxPossibleIndexNum(str1, str2) > 0 && result != nullptr && num > 0) { //可能个数>0才行
        indexNum = KMP(str1, str2, strlen(str1) - 1, strlen(str2) - 1, result, num);
        for (int i = 0; i < indexNum; ++i) {
            result[i]++; //这里返回的是下标
        }
    }
    return indexNum;
}

int MyString::indexOf(const char *str1, const char *str2) {
    if (maxPossibleIndexNum(str1, str2) > 0) {
        int index = 0;
        if (getIndexOfHowMany(str1, str2, &index, 1) == 1) {
            return index;
        }
    }
    return -1;
}

int MyString::findAllIndexOf(const char *str1, const char *str2, int *result) {
    return getIndexOfHowMany(str1, str2, result, maxPossibleIndexNum(str1, str2)); //最多不超过len1-len2个
}

int MyString::maxPossibleIndexNum(const char *str1, const char *str2) {
    int len = 0;
    if (!isEmptyStr(str1) && !isEmptyStr(str2)) {
        len = strlen(str1) - strlen(str2) + 1;
        len = len > 0 ? len : 0;
    }
    return len;
}

int MyString::findAllIndexOf(const char *str, int *result) {
    return findAllIndexOf(head, str, result);
}


