//
// Created by HP on 2020-11-24.
//


#include <iostream>
#include "Map/Topological.h"

using namespace std;

int main() {
    tuopuMap<int> *map = new tuopuMap<int>(14);
    map->addVex(0, 0);
    map->addVex(1, 0);
    map->addVex(2, 2);
    map->addVex(3, 0);
    map->addVex(4, 2);
    map->addVex(5, 3);
    map->addVex(6, 1);
    map->addVex(7, 2);
    map->addVex(8, 2);
    map->addVex(9, 2); //V9的入度为2，大话数据结构468页中那个in=1是错误的，但图是对的
    map->addVex(10, 1);
    map->addVex(11, 2);
    map->addVex(12, 1);
    map->addVex(13, 2);

    //增加出边0->11
    map->addOuEdge(0, 11);
    map->addOuEdge(0, 5);
    map->addOuEdge(0, 4);
    map->addOuEdge(1, 8);
    map->addOuEdge(1, 4);
    map->addOuEdge(1, 2);
    map->addOuEdge(2, 9);
    map->addOuEdge(2, 6);
    map->addOuEdge(2, 5);
    map->addOuEdge(3, 13);
    map->addOuEdge(3, 2);
    map->addOuEdge(4, 7);
    map->addOuEdge(5, 12);
    map->addOuEdge(5, 8);
    map->addOuEdge(6, 5);
    map->addOuEdge(8, 7);
    map->addOuEdge(9, 11);
    map->addOuEdge(9, 10);
    map->addOuEdge(10, 13);
    map->addOuEdge(12, 9);

    SequentialQueue<int> *queue = new SequentialQueue<int>(); //保存序列
    topoLogicalSort(map, queue); //拓扑排序, 如果输出顶点序列的数量小于总顶点数，表示存在回路，不合标准
    while (!queue->isEmpty()) {
        int index;
        queue->queueOut(&index);
        cout << index << ",";
    }
    //0,1,3,4,2,6,5,12,8,9,7,11,10,13,  这种顺序不是唯一的，比如 0 1 3随便谁先
    delete map;
    delete queue;
    return 0;
}