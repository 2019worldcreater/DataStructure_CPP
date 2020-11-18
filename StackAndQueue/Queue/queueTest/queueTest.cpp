
#include <iostream>
#include <cstring>
#include "StackAndQueue/Queue/LinkQueue.h"

using namespace std;

int queueTest() {
    LinkQueue<int> queue;
    cout << queue.isEmpty() << endl;
    queue.queueIn(1);
    queue.queueIn(2);
    cout << queue.getLength() << endl;
    int i;
    if (queue.queueOut(&i)) {
        cout << queue.getLength() << endl;
        cout << i << endl;
    }
    queue.releaseQueue();
    return 0;
}