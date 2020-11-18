
#ifndef CLIONCPP_STRING_H
#define CLIONCPP_STRING_H

#include <cstdlib>

/* 字符串的实现，实现：顺序结构，不过不是普通的数组，而是在堆中申请的一段空间
 * 这样当字符串拼接或者删减时可以动态改变其大小
 *
 * 约定'\0'为串的结束位，也可以将长度保存为一个值，都行
 *
 * 至于链式结构的字符串没卵用，当然不是每个节点只存储一个字符，可以存储多个字符，如果某节点字符未满，就以'\0'结束，标志整个结束
 * 但链式结构也就是拼接字符串有点用，其他的性能都不如顺序结构，所以我只实现顺序结构
 */

class MyString {
    //太多了，有些没写：  replace(str1,str2)、deletePart(str,start,len),insertInto(str1,str2,index),append(str)
private:
    char *head = nullptr; //指向堆中为字符串分配的空间

    //返回堆中指定字节数的空间首地址
    static char *createSpaceWithStr(const char *str) {
        char *ptr = (char *) malloc(strlen(str));
        strCopy(ptr, str); //复制内容
        return ptr;
    }

    //将str2在str1中num个下标，但保存的值是 (下标+1),保存在result中,并返回实际查找到的个数
    static int getIndexOfHowMany(const char *str1, const char *str2, int *result, int num);

public:
    //获取本字符串的长度(总字节数-1), -1代表初始化失败
    int getLength() {
        return strlen(head) - 1;
    }

    //如果是new创建的对象，delete时会自动调用
    ~MyString() {
        destroy();
    }

    //复制得到一样的string
    explicit MyString(const MyString *str) : MyString(str->head) { /*私有成员仅仅是其他类成员不能访问，这是自己人*/}

    //建立长度为0的空串，但实际上以'\0'结尾
    MyString() : MyString("") {}

    //根据初始串，建立字符串
    explicit MyString(const char *str);

    //释放该字符串的空间， 一旦释放，不可进行其他任何操作
    void destroy();

    //清空字符串,但实际上还有'\0'
    void clean();

    //返回字符串的长度（实际上是可视内容包括'\0'的总字节数），必须以'\0'结尾，可以通过类名::方法名调用， nullptr=0
    static int strlen(const char *str);

    //将str赋值给MyString
    void strCopyFrom(const char *str);

    //将str2复制给str1，请确保str1有足够空间
    static char *strCopy(char *str1, const char *str2);

    //将MyString复制给MyString,其长度会动态变化
    void strCopyFrom(const MyString *str) { strCopyFrom(str->head); }

    //将str2拼接在str1后面，自己考虑空间够不够
    static char *strCat(char *str1, const char *str2);

    //将str拼接在MyString后面
    void strCatWith(const char *str);

    //将str拼接在MyString后面,自我拼接也行
    void strCatWith(const MyString *str) { strCatWith(str->head); }

    //比较两个字符串(当然得是可以比较的,目前支持纯数字、字母)，1 : str1 > str2, -1 : <, 0 : =, 任何一方为null则返回-1
    static int strCompare(const char *str1, const char *str2);

    //比较MyString和str
    int strCompareWith(const char *str) { return strCompare(head, str); }

    //比较两个MyString
    int strCompareWith(const MyString *str) { return strCompare(head, str->head); }

    //将str字符串从第start个字符(字节)开始的长度为len的子串(不包括'\0')复制到str2中，返回str2, 如果接受的str2指向null或len <= 0,那就返回null
    static char *subString(const char *str, int start, int len, char *str2);

    //将MyString切割,从第start个字符(字节)开始的长度为len的子串(不包括'\0')
    void subString(int start, int len);

    //返回子串str2在str1出现的第一个字符的在第几个字符上，如果没有返回-1,任何一方是空串也返回-1
    static int indexOf(const char *str1, const char *str2);

    //在MyString中查找字符串下标+1,没有则-1
    int indexof(const char *str) {
        return indexOf(head, str);
    }

    //在MyString中查找字符串下标+1,没有则-1
    int indexof(const MyString *str) { return indexof(str->head); }

    //str2在str1中的下标的最多可能有多少个,任何一方为空串或str1没str2长，那么返回0
    static int maxPossibleIndexNum(const char *str1, const char *str2);

    //str2在MyString中的下标的最多可能有多少个
    int maxPossibleIndexNum(const char *str2) {
        return maxPossibleIndexNum(head, str2);
    }

    //str2在MyString中的下标的最多可能有多少个
    int maxPossibleIndexNum(const MyString *str2) {
        return maxPossibleIndexNum(head, str2->head);
    }

    //str2在str1中的所有(下标+1)保存在result中,，建议int result[MyString::maxPossibleIndexNum(str1,str2)];
    //返回值是最终找到的下标的个数，里面的元素值是下标+1, 任何一方空串返回0, result不能null
    static int findAllIndexOf(const char *str1, const char *str2, int *result);

    //返回个数，将str子串在MyString的下标保存在result中，建议 int result[mystring->maxPossibleIndexNum(str)];
    //返回值是最终找到的下标的个数，里面的元素值是下标+1
    int findAllIndexOf(const char *str, int *result);

    //返回个数，将str子串在MyString的下标保存在result中，建议 int result[mystring->maxPossibleIndexNum(mystring1)];
    //返回值是最终找到的下标的个数，里面的元素值是下标+1
    int findAllIndexOf(const MyString *str, int *result) { return findAllIndexOf(str->head, result); }

    //传入容纳Mystring内容的空间的首地址，将MyString内容复制进去，返回该地址, 建议 char str[mystring->getLength() + 1];
    //这样可以直接 cout<< myString->getContent(str);, 如果MyString已释放或接受的str指向null,返回Null
    char *getContent(char *str);

    //MyString是否初始化成功
    bool isInitSucceed() {
        return !isPointAtNull(head);
    }

    //是否是空串
    bool isEmpty() {
        return !isInitSucceed() || getLength() <= 0;
    }

    //字符串是否为空串，如果指向null,也为true, 所以 ！isEmpty满足非null且len>0
    static bool isEmptyStr(const char *str) {
        return strlen(str) < 2; //总字节数 < 2
    }

    //字符串str是否为空指针
    static bool isPointAtNull(const char *str) {
        return str == nullptr;
    }
};

#endif //CLIONCPP_STRING_H
