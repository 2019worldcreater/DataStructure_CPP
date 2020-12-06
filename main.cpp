#include <iostream>

#include "Map/mostShortPath.h"

using namespace std;


int main() {
    //请见大话数据结构 第455页
    NearByMatrix<char> *map = new NearByMatrix<char>(9);
    for (int i = 0; i < map->numVertexes; ++i) {
        map->vertex[i] = 'A' + i; //赋值与否都无所谓
    }
    map->addEdge(0, 1, 1);
    map->addEdge(0, 2, 5);
    map->addEdge(1, 2, 3);
    map->addEdge(1, 4, 5);
    map->addEdge(4, 2, 1);
    map->addEdge(5, 2, 7);
    map->addEdge(1, 3, 7);
    map->addEdge(3, 4, 2);
    map->addEdge(4, 5, 3);
    map->addEdge(4, 6, 6);
    map->addEdge(4, 7, 9);
    map->addEdge(6, 3, 3);
    map->addEdge(7, 5, 5);
    map->addEdge(7, 6, 2);
    map->addEdge(6, 8, 7);
    map->addEdge(7, 8, 4);

    bellNode p[map->numVertexes];
    if (SPFA(map, p, 0)) {
        for (int j = 0; j < map->numVertexes; ++j) {
            bellNode node = p[j];
            cout << node.parent << "," << node.next << "," << node.weight << endl;
        }

        cout << "if 0 -> 6" << endl;
        int parent = p[6].parent;
        while (parent != 0) { //不是源点，还有中转
            cout << parent << ",";
            parent = p[parent].parent; //3,4,2,1 --> 代表 0,1,2,4,3,6
        }
    }
    delete map;
    return 0;
}
