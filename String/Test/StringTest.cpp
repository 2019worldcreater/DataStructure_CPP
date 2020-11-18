//
// Created by HP on 2020-11-16.
//


#include <iostream>
#include "String/String.h"

using namespace std;

int stringTest() {
    //使用 MyString的好处是过程中不必考虑具体空间大小
    // 其中strCopy\strCat\strCompare\indexOf\findAllIndex\strlen都是可以通过类名调用的，用来处理非MyString的
    MyString *str1 = new MyString("i am a good boy, right ? good boy");
    MyString *str2 = new MyString(str1);
    cout << "str1.length = " << str1->getLength() << "; str2.len = " << str2->getLength() << endl; //33
    str2->clean(); //""
    cout << "clean, str2.len = " << str2->getLength() << endl; //0
    str2->strCopyFrom("21"); //原样
    cout << "copyForm, str2.len = " << str2->getLength() << endl; //2
    str2->strCopyFrom("dsa");//dsa
    str2->strCatWith(str2);
    char c[str2->getLength() + 1]; //还有'\0'
    cout << "str2.content = " << str2->getContent(c) << endl; //dsadsa

    str2->strCopyFrom("i am a goot boy, right ? good boy");
    cout << "str1 vs str2 = " << str1->strCompareWith(str2) << endl;

    char findStr[] = "goot";
    int index = str2->indexof(findStr);
    cout << "str2.indexOf(findStr) = " << index << endl;

    str2->subString(index, MyString::strlen("good") - 1);
    cout << "str2.content = " << str2->getContent(c) << endl; //goot

    int allIndex[str1->maxPossibleIndexNum("good")];
    int totalNum = str1->findAllIndexOf("good", allIndex);
    for (int i = 0; i < totalNum; ++i) {
        cout << allIndex[i] << ","; //8,26
    }

    delete str2;
    delete str1; //自动调用 析构，析构调用destroy
    return 0;
}
