//
// Created by HP on 2020-11-23.
//


#include <iostream>
#include "Map/MinPath.h"

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

    int pre[map->numVertexes]; //preVexOf..[3]=2,要想从源点V(originalVex)最短到达顶点V3,就要经过V2，然后看看[2]是谁,以此找到路径
    // 如果是pre[i]=originalVex，则是与originVex直接相连,不需经过其它
    int len[map->numVertexes]; //shortPathLength保存源点originalVex(0~vexNum-1)到图中各顶点的最短路径长度,[i]即 VoriginalVex-Vi
    DijkstraMostShortPath(map, pre, len, 0); //计算从顶点0到达其它所有点的最短路径
    //结果 len = {0,1,4,7,5,8,10,12,16}, pre={0,0,1,4,2,4,3,6,7}
    delete map;
    return 0;
}


//如果要知道i-j的路径，只需依次index = [i][j], =[index][j],..直到[index][j]=j
template<typename T>
void findPath(int start, int end, int **pre, int **path, NearByMatrix <T> *map) {
    if (start > map->numVertexes - 1 || start < 0 || end > map->numVertexes - 1 || end < 0) {
        cout << "wrong";
        return;
    }
    SequentialQueue<int> *queue = new SequentialQueue<int>();
    int thEnd = pre[start][end];
    queue->queueIn(start);
    while (thEnd != end) {
        queue->queueIn(thEnd);
        thEnd = pre[thEnd][end];
    }
    queue->queueIn(end);

    int index;
    while (!queue->isEmpty()) {
        queue->queueOut(&index);
        cout << index;
        if (!queue->isEmpty()) {
            cout << "-->";
        }
    }
    cout << endl << "min = " << path[start][end];
    delete queue;
}

int floydtest() {
    NearByMatrix<char> *map = new NearByMatrix<char>(9);
    for (int i = 0; i < map->numVertexes; ++i) {
        map->vertex[i] = 'A' + i;
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

    int **pre = new int *[map->numVertexes]; //路径
    int **path = new int *[map->numVertexes]; //总长度
    for (int j = 0; j < map->numVertexes; ++j) {
        pre[j] = new int[map->numVertexes];
        path[j] = new int[map->numVertexes];
    }
    FloydMostShortPath(map, path, pre);

    findPath(0, 8, pre, path, map); //请确保
//0-->1-->2-->4-->3-->6-->7-->8
//min = 16
    for (int k = 0; k < map->numVertexes; ++k) {
        delete[] pre[k];
        delete[] path[k];
    }
    delete map;
    delete[]pre;
    delete[]path;
    return 0;
}

void bellFordTest() {
    //数据一样
    bellNode p[map->numVertexes];
    if (Bellman_Ford(map, p, 0)) {
        for (int j = 0; j < map->numVertexes; ++j) {
            bellNode node = p[j];
            cout << node.parent << "," << node.next << "," << node.weight << endl;
        }
    }

    cout << "if 0 -> 6" << endl;
    int parent = p[6].parent;
    while (parent != 0) { //不是源点，还有中转
        cout << parent << ",";
        parent = p[parent].parent; //3,4,2,1 --> 代表 0,1,2,4,3,6
    }
}