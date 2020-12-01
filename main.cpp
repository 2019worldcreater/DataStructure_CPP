#include <iostream>
#include "Sort/declaration/merge_sort.h"

using namespace std;


int main() {
    int a[10000] = {1, 5, 2, 4, 2, 3, 1, 21, 3};
    merge_sort_dieDai(a, sizeof(a) / sizeof(int));
    return 0;
}

