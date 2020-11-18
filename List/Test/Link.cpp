
#include <iostream>
#include "List/LinkList.h"

using namespace std;

int LinkTest() {
    LinkList<int> *list = new LinkList<int>();
    cout << list->isLinkListInitDone() << endl;
    list->addNode(1);
    list->addNode(1);
    list->addNode(2);
    cout << "len = " << list->getLength() << endl;
    int j;
    list->deleteNodeByIndex(2, &j);
    int index = list->getIndexOfData(2);
    int i = 0;
    if (list->isValid(index)) {
        list->getDataByIndex(index, &i);
        cout << "i = " << i << endl;
    }

    list->cleanLinkList();
    cout << "len = " << list->getLength() << endl;
    list->releaseLinkList();
    cout << "len = " << list->getLength() << endl;
    cout << list->isLinkListInitDone() << endl;
    delete list;
    return 0;
}
