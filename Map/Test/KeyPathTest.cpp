//
// Created by HP on 2020-11-24.
//


#include <iostream>
#include "Map/KeyPath.h"

using namespace std;

int main() {
    tuopuMap<int> *map = new tuopuMap<int>(10);
    map->addVex(0, 0); //顶点信息,入度，保存的顺序即为下标
    map->addVex(1, 1);
    map->addVex(2, 1);
    map->addVex(3, 2); //写错一个都会失败
    map->addVex(4, 2);
    map->addVex(5, 1);
    map->addVex(6, 1);
    map->addVex(7, 2);
    map->addVex(8, 1);
    map->addVex(9, 2);

    map->addOuEdge(0, 1, 3); //0表示第一个加入的顶点
    map->addOuEdge(0, 2, 4);
    map->addOuEdge(1, 3, 5);
    map->addOuEdge(1, 4, 6);
    map->addOuEdge(2, 3, 8);
    map->addOuEdge(2, 5, 7);
    map->addOuEdge(3, 4, 3);
    map->addOuEdge(5, 7, 6);
    map->addOuEdge(4, 7, 4);
    map->addOuEdge(4, 6, 9);
    map->addOuEdge(7, 8, 5);
    map->addOuEdge(8, 9, 3);
    map->addOuEdge(6, 9, 2);

    SequentialQueue <edgeInfo> *queue = new SequentialQueue<edgeInfo>();
    findKeyPath(map, queue);
    edgeInfo edge;
    while (!queue->isEmpty()) {
        queue->queueOut(&edge);
        cout << edge.start << "," << edge.end << endl;
    }
    //0,2
    //2,3
    //3,4
    //4,7
    //7,8
    //8,9

    //关键路径(可能不止一条)上的所有边
    delete map;
    delete queue;
    return 0;
}