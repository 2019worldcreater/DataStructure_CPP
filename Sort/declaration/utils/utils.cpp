#include "utils.h"
#include "iostream"
#include "ctime"

void swap(int *a, int i, int j) {
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

void printProgramExecutionTime() {
    std::cout << "The run time is:" << (double) clock() / CLOCKS_PER_SEC << "s" << std::endl;
}
