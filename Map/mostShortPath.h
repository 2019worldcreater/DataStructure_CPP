//
// Created by HP on 2020-11-23.
//

#ifndef CLIONCPP_MOSTSHORTPATH_H
#define CLIONCPP_MOSTSHORTPATH_H

#include "Map.h"

/*
 * 求最短路径问题，通常是指图中的源点到某终点的最小权值之和的到达路径， 有向图也可以
 * 所以是对有权值的网才有一定意义，当然没有权值，就是最少边数，权值也最好是正数，不是连通图也可以
 *
 * 1、迪杰斯特拉（Dijkstra）算法：  假设有个V0(假设是源点)保存到各顶点最小路径(时常更新)的数组 V0i表示数组中已知的V0-Vi的最短路径
     * 开始时V0只知道与其直接相连的顶点的各路径长度，其他不直接连接的点假设到其的路径长度为极大65535
     * 假如V0-Vi的边是其中权值最小的，那么V0已经可以知道V0-Vi的最短路径即是V0i边的权值，毋庸置疑
     * 那么接着对这个刚刚找到最短路径的Vi展开，如果V0i + Vik < V0k，那么更新V0k, 并认为要想最短到Vk,就要经过Vi
     * 此时又可以得到一个真正的最短路径，即最小路径数组中最小值所在，假设为 V0b
     * 为什么此时的最小路径一定是会是真正的最短路径呢，因为如果有其它点能到达的话，必然要经过另一条顶点然后到达Vb，即 V0c-Vn,此时V0c + Vcb > V0b,所以不成立，即便Vc没有拓展到（Vc也要经过已知的顶点），也可以证明V0c-Vb不是最短
     *
     *  一共有 n-1次循环，每次找到一个最短路径
     *  每次循化总的来说就两步：1、当前的最小路径即可以确定为最短路径，该顶点i即为找到     2、根据i拓展顶点，并更新最小路径数组

    2、弗洛伊德（Floyd）算法 (带负权路径的图则会陷入死循环)：  假如存在 Vij = 12,如果此时引入一个顶点k,使得 Vik + Vkj < Vij, 那么这个中转顶点使得路径变短了
      所以说这时可以知道i想要最短到j，就要先到k，至于i怎么到k，k怎么到j又是后话了，其实很多时候，ik,kj中也可能存在中转节点，使得各自的路程缩短
      也就是说： 每个顶点都有可能成为某两个顶点间的中转顶点, 所以只要对每个顶点k，遍历所有Vij,看看k能不能帮上忙，即三层循环
      为此我们需要保存任意i->J最短路径的矩阵path[1][2]=3,代表1-2最短为3，既然知道了最短路径长度，当然需要得知其具体路径
      与上面的preNode想法一样，我们只需保存上一个顶点，即起到中转效果的顶点
      preVex[1][2] = 3,说明1要到2，就要最先经过3,1-3-...-2(这么做的好处，使得[i][j]与i一定是直连的), 至于3-2在[3][2]也会有说明，如果[i][k]=k,说明已经到达
      如果要知道i-j的路径，只需依次index = [i][j], =[index][j],不断找
 */



//邻接矩阵图、用来保存在最短路径中的前驱顶点，比如preVexOf..[3]=2,要想从源点V(originalVex)最短到达顶点V3,就要经过V2，然后看看[2]是谁,以此找到路径
// 如果是pre[i]=originalVex，则是与originVex直接相连,不需经过其它
//shortPathLength保存源点originalVex(0~vexNum-1)到图中各顶点的最短路径长度,[i]即 VoriginalVex-Vi
//函数结束后，会得到originalVex到其他所有顶点的最短路径， 如果不可到达，即为65535， 想要知道任意两点间的path，只需为每个顶点调用该函数
//邻接矩阵：有边就为权值，没边即为0
template<typename T>
void
DijkstraMostShortPath(NearByMatrix<T> *map, int *preVexOfEachVexInShortPath, int *shortPathLength, int originalVex) {
    int infinity = 65535; //不可能之数
    bool isFindTheMostShortPath[map->numVertexes]; //是否找到真正最短路径的标志位
    for (int i = 0; i < map->numVertexes; ++i) {
        //先初始化源点连接的各个点的直接路径
        if (map->edgeMatrix[originalVex][i] != 0)
            shortPathLength[i] = map->edgeMatrix[originalVex][i];
        else
            shortPathLength[i] = infinity; //如果没有边是等于65535即不可能到达
        isFindTheMostShortPath[i] = false;
        preVexOfEachVexInShortPath[i] = originalVex; //初始化
    }
    shortPathLength[originalVex] = 0; //到自身必定是0长度
    isFindTheMostShortPath[originalVex] = true;

    for (int i = 1; i < map->numVertexes; i++) { //n-1循环，每一轮确定真正的一个最短路径
        int minWeight = infinity, vex = 0; //最小路径中最小的, 被确定的顶点

        //找到已知到各顶点路径中最小的
        for (int j = 0; j < map->numVertexes; ++j) {
            if (!isFindTheMostShortPath[j] && shortPathLength[j] < minWeight) { //已经找到了的不需要对比
                minWeight = shortPathLength[j];
                vex = j;
            }
        }
        isFindTheMostShortPath[vex] = true; //可以确定已经找到了从originVex到vex的最短路径

        //展开vex顶点，重新整理最短路径
        for (int k = 0; k < map->numVertexes; ++k) {
            if (!isFindTheMostShortPath[k] && map->edgeMatrix[vex][k] != 0 &&
                shortPathLength[k] > shortPathLength[vex] + map->edgeMatrix[vex][k]) {
                shortPathLength[k] = shortPathLength[vex] + map->edgeMatrix[vex][k]; //更新最短路径
                preVexOfEachVexInShortPath[k] = vex; //要想最短，就要借道vex
            }
        }
    }
}

//Floyd算法计算任意两个顶点间的最短路径，长度保存在pathMatrix,而路径保存在preVexMatrix中（只是前驱顶点）
//path[1][2]=3,代表1-2最短为3
//preVex[1][2] = 3,说明1要到2，就要最先经过3,1-3-...-2, 至于3-2在[3][2]也会有说明，如果[i][k]=k,说明直连
template<typename T>
void FloydMostShortPath(NearByMatrix<T> *map, int **pathMatrix,
                        int **preVexMatrix) {
    int infinity = 65535;
    //pathMatrix初始与map中的邻接矩阵一样, preVexMatrix初始化[i][k] = k(甭管对不对)
    for (int i = 0; i < map->numVertexes; ++i) {
        for (int j = 0; j < map->numVertexes; ++j) {
            if (map->edgeMatrix[i][j] != 0 || i == j) {  //path做些改动，边无设为65535,而自身到自身设为0(否则自身到自身会有中转的可能)
                pathMatrix[i][j] = map->edgeMatrix[i][j];
            } else {
                pathMatrix[i][j] = infinity;
            }
            preVexMatrix[i][j] = j;
        }
    }

    for (int k = 0; k < map->numVertexes; ++k) { //k作为中转顶点
        for (int i = 0; i < map->numVertexes; ++i) { //里两层则用来遍历任意两边Vij
            for (int j = 0; j < map->numVertexes; ++j) {
                if (pathMatrix[i][k] + pathMatrix[k][j] < pathMatrix[i][j]) { //k可以缩短ij的路程, i - k - j
                    pathMatrix[i][j] = pathMatrix[i][k] + pathMatrix[k][j];
                    preVexMatrix[i][j] = preVexMatrix[i][k];
                    //先有[i][h]=l, 再后来h又可以中转[i][j],那么[i][j]=h 或是 l,如果写成h,无非就是表示最后一次中转
                    //而i-h,h-j却要分开考虑，很麻烦，到不如一开始就将[i][j]=l,从起源开始找，这样就只是单向搜索了
                }
            }
        }
    }
}

#endif //CLIONCPP_MOSTSHORTPATH_H
