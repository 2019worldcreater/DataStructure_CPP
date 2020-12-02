#include <iostream>
#include <ctime>
#include "Sort/declaration/quick_sort.h"
#include "Sort/declaration/merge_sort.h"
#include "Sort/declaration/heap_sort.h"
#include "Sort/declaration/shell_sort.h"
#include "Sort/declaration/counting_sort.h"

using namespace std;


int main() {
    int a[500000];
    int size = sizeof(a) / sizeof(int);
    srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        a[i] = i;
    }
    counting_sort(a, size);
    printProgramExecutionTime();
    return 0;
}

