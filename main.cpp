
#include <iostream>
#include "Map/DFS_OR_BFS.h"
#include "Map/MinimumCostTree.h"

using namespace std;


int main() {
    edgeInfoList<char> *map = new edgeInfoList<char>(5);
    char c[] = {'A', 'B', 'C', 'D', 'E'};
    for (int i = 0; i < 5; ++i) {
        map->vexList[i] = c[i];
    }
    SequentialQueue<edgeInfo> *queue = new SequentialQueue<edgeInfo>();
    map->addEdge(1, 2, 10);
    map->addEdge(2, 4, 10);
    map->addEdge(0, 1, 20);
    map->addEdge(0, 4, 30);
    map->addEdge(0, 3, 40);
    map->addEdge(3, 4, 50);
    map->addEdge(2, 3, 50);
    KrusKal_MinimumTree(map, queue);

    int sum = 0;
    for (int j = 0; j < map->vexNum - 1; ++j) {
        edgeInfo edgeInfo;
        queue->queueOut(&edgeInfo);
        cout << edgeInfo.start << "," << edgeInfo.end << endl;
        sum += edgeInfo.weight;
    }
    cout << sum << "dsas";
    delete queue;
    delete map;
    return 0;
}