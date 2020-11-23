//
// Created by HP on 2020-11-22.
//

#ifndef CLIONCPP_DFS_OR_BFS_H
#define CLIONCPP_DFS_OR_BFS_H

#include "Map.h"
#include "../StackAndQueue/Queue/SequentialQueue.h"

/*
 * 具体而言，两者的效率没有太大差别，只是顺序不同
 *
 * 深度优先更适合目标比较明确，以找到目标为主要目的的情况，而广度优先更适合在不断扩大遍历范围时找到相对最优解的情况
 *
 * DFS 深度优先搜索, 类似树的前序遍历
 * 遍历到图中一个顶点i时，如果i关联许多顶点，这时我们选择一个固定位次（第一个、或最后一个）的未被访问的顶点，接来下来的遍历都按照这个规则
 * 如果当前顶点邻接的顶点都被访问，则返回到上一个遍历的节点，然后深搜其他邻接点，直到遇到新的节点，如果回到初始节点i，则一个连通子图遍历结束,很明显是个递归的过程
 * 一次深搜结束后，如果还有未被访问的话，说明有另外一个连通子图，只需继续对未访问顶点深搜
 *
 * 当然这么做的前提是图是连通图，如果不是，当一个连通子图遍历完后，只需对另一个未曾遍历的顶点开始新一轮的深搜
 */


//邻接表的DFS O(n+edgeNum)，主函数基本一致，递归函数中也就是查询邻接顶点是遍历单链表得到的,如果是有向图，必须按照出边方向

//邻接矩阵的深搜, O(n^2)
template<typename T>
void matrixDFS(NearByMatrix<T> *map, SequentialQueue<T> *queue, int vexIndex, bool *isVisited) { //图、队列、当前遍历顶点的下标、访问标志数组
    queue->queueIn(map->vertex[vexIndex]); //遍历到该节点
    isVisited[vexIndex] = true;
    for (int i = 0; i < map->numVertexes; ++i) { //列出所有邻接边
        if (map->edgeMatrix[vexIndex][i] == 1 && !isVisited[i]) {
            //说明优先选择顶点下标靠前的进行深搜，(即固定次序) 遍历到没有还没遍历的邻接接顶点时就会返回，然后看看其他邻接点是否还有
            matrixDFS(map, queue, i, isVisited); //这条语句的结束即是返回
        }
    }
}

//主调用方法，用于遍历前处理，以及控制连通子图的遍历， queue保存遍历到的顶点信息。同样是遍历顺序，有向图也可以用
template<typename T>
void matrixDFSMain(NearByMatrix<T> *map, SequentialQueue<T> *queue) {
    bool isVisited[map->numVertexes]; //顶点是否访问过的标志位,下标与顶点数组一致
    for (int i = 0; i < map->numVertexes; ++i) {
        isVisited[i] = false;
    }
    for (int j = 0; j < map->numVertexes; ++j) {
        if (!isVisited[j]) { //如果整个图不是连通图，这个if内语句才会有第二次执行的可能
            matrixDFS(map, queue, j, isVisited); //执行完这个说明一个连通子图遍历完了
            //主调用是第一个连通子图遍历的初始顶点
        }
    }
}


/*
 * BFS。广度优先遍历，类似树的层序遍历
 * 首先我们选中一个顶点放在第一层，与其相邻的顶点放在第二层，与第二层中的顶点相邻的放在第三层，依次类推，如果顶点已经在某层了（即已经遍历过），就不能放在其它层
 * 经过改造很容易看出这就是层序遍历的次序, 基本实现和树一致：队列
 *
 * 同样一次广度搜索只能遍历完一个连通子图，如果还有，再进行一轮
 */
template<typename T>
void BFSMatrix(NearByMatrix<T> *map, SequentialQueue<T> *queue, int vexIndex, bool *isVisited,
               SequentialQueue<T> *outQueue) {
    T head;
    queue->queueIn(vexIndex); //根顶点
    isVisited[vexIndex] = true;
    while (!queue->isEmpty()) {
        queue->queueOut(&head); //拓展顶点出队
        outQueue->queueIn(map->vertex[head]); //输出
        for (int i = 0; i < map->numVertexes; ++i) { //往外拓展
            if (map->edgeMatrix[vexIndex][i] == 1 && !isVisited[i]) {
                queue->queueIn(i); //将邻接顶点加入队列
                isVisited[i] = true; //其他层的扩展不能再包括该顶点
            }
        }
    }
}

//邻接矩阵的BFS搜索
template<typename T>
void BFSMain(NearByMatrix<T> *map, SequentialQueue<T> *outQueue) {
    SequentialQueue<T> *queue = new SequentialQueue<T>(); //树的层序遍历是拿队头节点
    // 放进其孩子节点，直到树空，所以我将那些出队的数据又保存在，另一个队列中，queue即保存数据的，这个用来遍历罢了
    bool isVisited[map->numVertexes]; //顶点是否访问过的标志位,下标与顶点数组一致
    for (int i = 0; i < map->numVertexes; ++i) {
        isVisited[i] = false;
    }
    for (int j = 0; j < map->numVertexes; ++j) {
        if (!isVisited[j]) { //如果整个图不是连通图，这个if内语句才会有第二次执行的可能
            BFSMatrix(map, queue, j, isVisited, outQueue); //一轮广度优先遍历
            //主调用是第一个连通子图遍历的初始顶点
        }
    }
    delete queue;
}


#endif //CLIONCPP_DFS_OR_BFS_H
