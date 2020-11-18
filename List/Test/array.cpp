
#include <iostream>
#include "List/ArrayList.h"

using namespace std;

int ArratTest() {
    ArrayList<int> *list = new ArrayList<int>();
    cout << "len = " << list->getLength() << endl;
    list->addItem(1);
    list->addItem(2);
    list->addItem(3);
    list->insertInto(1, 4);
    int k;
    list->deleteNodeByIndex(2, &k);
    cout << "len = " << list->getLength() << endl;

    int index = list->getIndexOfData(4);
    if (list->isValid(index)) {
        int i;
        list->getDataByIndex(index, &i);
        cout << i;
    }
    list->clean();
    cout << "len = " << list->getLength() << endl;
    delete list;
    return 0;
}