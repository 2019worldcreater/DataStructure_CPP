//
// Created by HP on 2020-11-23.
//

#ifndef CLIONCPP_MOSTSHORTPATH_H
#define CLIONCPP_MOSTSHORTPATH_H

#include "../StackAndQueue/Queue/SequentialQueue.h"
#include "Map.h"

/*
 * 求最短路径问题，通常是指图中的源点到某终点的最小权值之和的到达路径， 有向图也可以,但不常用
 * 所以是对有权值的网才有一定意义，当然没有权值，就是最少边数，权值也最好是正数，不是连通图也可以
 *
 * 1、迪杰斯特拉（Dijkstra）算法：  假设有个V0(假设是源点，源点可以是任意点)保存到各顶点最小路径(时常更新)的数组 V0i表示数组中已知的V0-Vi的最短路径
     * 开始时V0只知道与其直接相连的顶点的各路径长度，假如只有 V0i,V0k,Voj，其他不直接连接的点假设到其的路径长度为极大65535
     * 假如V0-Vi的边是其中权值最小的，那么V0已经可以知道V0-Vi的最短路径即是V0i，毋庸置疑
     * 那么接着对这个刚刚找到最短路径的Vi进行展开,Vk是与Vi邻接的，如果V0i + Vik < V0k（V0可能之前来过VK,又从未遍历到），那么更新V0k, 并认为要想最短到Vk,就要经过Vi
     * 此时又可以得到一个真正的最短路径，即最小路径数组中最小值所在，假设为 V0b
     * 为什么此时的最小路径一定是会是真正的最短路径呢，因为如果有其它点能到达的话，必然要经过另一条顶点然后到达Vb，即 V0c-Vn,此时V0c + Vcb > V0b,所以不成立，即便Vc没有拓展到（Vc也要经过已知的顶点），也可以证明V0c-Vb不是最短
     *
     *  一共有 n-1次循环，每次找到一个最短路径
     *  每次循化总的来说就两步：1、当前的最小路径即可以确定为最短路径，该顶点i即为找到     2、根据i拓展其邻接顶点，并更新最小路径数组
     可以看出这是一个深度遍历的过程

    2、弗洛伊德（Floyd）算法 (带负权环路的会错误)：  假如存在 Vij = 12,如果此时引入一个顶点k,使得 Vik + Vkj < Vij, 那么这个中转顶点使得路径变短了
      所以说这时可以知道i想要最短到j，就要先到k，至于i怎么到k，k怎么到j又是后话了，其实很多时候，ik,kj中也可能存在中转节点，使得各自的路程缩短
      也就是说： 每个顶点都有可能成为某两个顶点间的中转顶点, 所以只要对每个顶点k，遍历所有Vij,看看k能不能帮上忙，即三层循环
      为此我们需要保存任意i->J最短路径的矩阵path[1][2]=3,代表1-2最短为3，既然知道了最短路径长度，当然需要得知其具体路径
      与上面的preNode想法一样，我们只需保存上一个顶点，即起到中转效果的顶点
      preVex[1][2] = 3,说明1要到2，就要最先经过3,1-3-...-2(这么做的好处，使得[i][j]与i一定是直连的), 至于3-2在[3][2]也会有说明，如果[i][k]=k,说明已经到达
      如果要知道i-j的路径，只需依次index = [i][j], =[index][j],不断找
      
      之前我一直在想： 如果i中转了 jk( j - i - k),之后又有 l 中转了 ji(j - l - i), 那么jk的长度不是同时应该发生变化吗，但随后我发现之前 i 可能会中转 lk, 所以 l 也会中转 jk
 */



//邻接矩阵图、用来保存在最短路径中的前驱顶点，比如preVexOf..[3]=2,要想从源点V(originalVex)最短到达顶点V3,就要经过V2，然后看看[2]是谁,以此找到路径
// 如果是pre[i]=originalVex，则是与originVex直接相连,不需经过其它
//shortPathLength保存源点originalVex(0~vexNum-1)到图中各顶点的最短路径长度,[i]即 VoriginalVex-Vi
//函数结束后，会得到originalVex到其他所有顶点的最短路径， 如果不可到达，即为65535， 想要知道任意两点间的path，只需为每个顶点调用该函数
//邻接矩阵：有边就为权值，没边即为0
template<typename T>
void DijkstraMostShortPath(NearByMatrix<T> *map, int *preVexOfEachVexInShortPath, int *shortPathLength, int originalVex) {
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


struct bellNode {
    int parent; // 源点->next需最后经过parent, 存在 start->...->parent->next,
    int next;
    int weight;
};

/*
 * 主要思想：所有的边进行n-1轮松弛，因为在一个含有n个顶点的图中，任意两点之间的最短路径最多包含n-1条边。
 * 换句话说，第1轮在对所有的边进行松弛操作后，得到从1号顶点只能经过一条边到达其余各定点的最短路径长度
 * 第2轮在对所有的边进行松弛操作后，得到从1号顶点只能经过两条边到达其余各定点的最短路径长度，........
 
 松弛操作就是通过已有的

此外，Bellman-Ford算法可以检测一个图是否含有负权的回路：如果经过n-1轮松弛后任然存在dst[e[i]]>dst[s[i]]+w[i].
 */
template<typename T>
bool Bellman_Ford(NearByMatrix<T> *map, bellNode *pathMatrix, int start) { //可用于含有负权的图
    //假设有一张表  parent, next, weight, 表示 源点 start,想要最短到达next，就需要经过先parent,weight表示路径
    //同时parent也可能是在其他中充当next, 所以最终结果中，如果想要知道start->'C',就先查找next='C'
    // 然后看parent是否为start,如果不是,假设是 K,就再去查找 next=K, 以此类推
    for (int k = 0; k < map->numVertexes; ++k) { //初始化表
        pathMatrix[k].parent = start;
        pathMatrix[k].weight = 65535; //假设无穷远
        pathMatrix[k].next = k;
    }
    //这里没有对start直连的顶点初始化，因为下面的循环体中第一轮会起到初始化作用的
    pathMatrix[start].weight = 0; //自己到自己

    //为什么要进行n-1次循环: 实际上第一轮帮得上忙的边只有与start直连的边, 即存在 start->i
    // 第2轮有用的只有以start为中心,内2层的边： start->i, i->k, 那么 start-i为内一层，i-k为内2层
    //其他轮也一样，因为假如有边 ij,但 i没遍历到，那么 dist[i] + ij < dist[j]的可能性你觉得呢,除非ij是负数，但根本毫无意义
    //即每一轮用的上的边应该至少有一个端点已经遍历到了，否则松弛操作没意义
    //至于为什么要对所有边遍历，因为图是复杂的，鬼知道哪条边是帮的上忙的,谁能判断哪条边是内几层呢
    //同一条边可能会进行多次松弛，比如 ij,第一次帮dist[j]松弛了,后面有条边 ki,帮dist[i]松弛，这时 ij又可以松弛dist[j]
    //其实松弛ij操作就是 i 作为start->j的中转点,start->j最多可能有n-1个中转点,i中转了j,k中转了i,ij又可松弛,l中转了k,ki松弛了,那么ij松弛了，所以最多有n-1次循环
    //所以事实上每一轮帮上忙的边：1.有一端点已遍历到即（ij松弛了,j遍历到了）  2.假如ij可松弛，那么需要有 ki 已松弛,即刚松弛点的关联边才有用
    //所以这个算法是值得改进的，见 SPFA 算法
    for (int i = 0; i < map->numVertexes - 1; ++i) {//每一轮对所有边进行遍历，共numVex-1轮
        bool isDone = true; //如果一轮中没有发生更新，可以结束
        for (int j = 0; j < map->numVertexes; ++j) { //遍历所有边
            for (int k = 0; k < map->numVertexes; ++k) {
                int edge = map->edgeMatrix[j][k];
                if (edge != 0) {
                    //假如遇到边 4->6,此时看看这条边能不能使 start->6缩短, 即start->4 + 4->6 < start->6, 然后将parent改为4,路径长度更新
                    if (pathMatrix[j].weight + edge < pathMatrix[k].weight) {  //松弛操作
                        pathMatrix[k].parent = j; //中转站
                        pathMatrix[k].weight = pathMatrix[j].weight + edge;
                        isDone = false; //进行了松弛
                    }
                }
            }
        }
        if (isDone)
            return true;
    }

    //如果还能找到，说明存在负权回路
    bool flag = false;
    for (int j = 0; j < map->numVertexes; ++j) {
        for (int k = 0; k < map->numVertexes; ++k) {
            int edge = map->edgeMatrix[j][k];
            if (edge != 0) {
                if (pathMatrix[j].weight + edge < pathMatrix[k].weight) {
                    flag = true;
                    break;
                }
            }
        }
    }

    return !flag;
}

//贝尔曼算法的优化版本, 考虑到每一轮的松弛边只有部分有用,见152行
template<typename T>
bool SPFA(NearByMatrix<T> *map, bellNode *pathMatrix, int start) {
    for (int k = 0; k < map->numVertexes; ++k) { //初始化表
        pathMatrix[k].parent = start;
        pathMatrix[k].weight = 65535;
        pathMatrix[k].next = k;
    }
    pathMatrix[start].weight = 0;

    //既然已松弛过的边ij中的 j? 才需要松弛,并且已遍历到的点的关联边才有资格松弛, 可以用队列实现
    SequentialQueue<int> *queue = new SequentialQueue<int>();

    queue->queueIn(start); //初始化,只有start的关联边有资格松弛
    bool isVisited[map->numVertexes]; //是否在队列中
    int softTime[map->numVertexes]; //每个顶点的关联边的松弛次数，如果超过定点数，则存在负权环路
    for (int j = 0; j < map->numVertexes; ++j) {
        isVisited[j] = false;
        softTime[j] = 0;
    }
    isVisited[start] = true;
    int head;
    while (!queue->isEmpty()) { //队空时结束，表示没有边需要松弛了
        queue->queueOut(&head); //出队
        isVisited[head] = false;
        softTime[head]++; //松弛次数+1
        if (softTime[head] >= map->numVertexes) { //松弛次数最多只能为 n-1
            delete queue;
            return false; //存在负权环路
        }
        for (int i = 0; i < map->numVertexes; ++i) { //对所有关联边松弛
            //松弛 head->i, dist[head] + head-i < dist[i]
            if (map->edgeMatrix[head][i] != 0 && //差点因为没加这个 0 判断就出错了,有边才行
                pathMatrix[head].weight + map->edgeMatrix[head][i] < pathMatrix[i].weight) {
                pathMatrix[i].weight = pathMatrix[head].weight + map->edgeMatrix[head][i]; //更新
                pathMatrix[i].parent = head;

                //即head-i已松弛,那么i的关联边可以松弛试试
                //如果存在负权环路
                if (!isVisited[i]) { //如果没在队列中
                    queue->queueIn(i);
                    isVisited[i] = true; //记得改
                }
            }
        }
    }
    delete queue;
    return true;
}

#endif //CLIONCPP_MOSTSHORTPATH_H
