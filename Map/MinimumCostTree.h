//
// Created by HP on 2020-11-22.
//

#ifndef CLIONCPP_MINIMUMCOSTTREE_H
#define CLIONCPP_MINIMUMCOSTTREE_H

/*
 * 一般应用都是无向边
 *
 * 有些人想花最少时间周游全中国，那么全中国可以看成是一个连通图(你能在任意地方到达任意地点)，其中边的权值即距离
 * 最少时间，即遍历图的所有顶点过程中经过的路径权值总和最少，意味着一条边都不能重复，否则就会导致总和不是最小
 * 之前有个概念即连通图的生成树，n个顶点，n-1个边，恰恰符合现在的情形，而其中最小代价的生成树称为最小生成树
 *
 * 构造连通图的最小生成树的算法有 普里姆算法(Prim)和克鲁斯卡尔算法
 *
 * 克鲁斯卡尔算法主要是针对边来展开，边数少时效率会非常高，所以对于稀疏图有很大的优势；而普里姆算法对于稠密图，即边数非常多的情况会更好一些。
 *
 * Prim: 设 G{V,E}, MinCostTree{V',E'}, 首先MinCostTree顶点集、边集都为空
 * 先随便将一个G中的顶点加入V‘, 然后在图中挑选依附于该顶点的最小权值的边，将边的另一顶点加入 V', 该边加入 E'
 * 然后在图中找到与V'中的任意顶点依附的最小权值的边，注意该边的顶点另一方不能属于V'即需是 V-V',然后还是一样的操作，直到V'中拥有全部n个顶点
 * 此时最小生成树{V',E'}构造完成
 *
 * KrusKal: 与Prim找到当前位置最小权值边不同，KrusKal是直接去找图中最小权值边
 * 为此Kruskal的图存储结构最好是边集数组{start,end,weight}, 并且严格按照end>start,weight从小到大排序，到时就好操作
 * 算法：一开始所有顶点都是互不干涉的一个分量（团体），首先找到图中最小权值边，如果边的两个顶点I,K是属于不同分量，那么该边纳入E';
 * I所属团体,k所属团体融合成为一个团体分量，然后继续找最小权值边（如果两顶点属于同一分量，则找第二小的），直到所有顶点属于一个分量
 * 如何得知他们是不同分量,我一开始的想法是给各自设置一个不同的代号，同一团体代号相同，但书上又给出了一种挺好的方法：
 * int group[n]{0..0}; group[i]=k,代表k与i属于同一分量，然后继续看 group[k]=L,如果L>0，那么L也与K,I同一分量，直到L=0，表示分量就这些了
 * 一开始group[i]=0,各自为伍
 */
#include "Map.h"
#include "../StackAndQueue/Queue/SequentialQueue.h"

struct edgeOfMinTree { //生成的最小生成树，n个顶点不用猜也知道，我只需返回n-1条边数据即可
    int vex1, vex2; //边3的两端顶点在map.顶点数组的下标
    int weight; //权值
};

//普里姆算法构造map的最小生成树，生成树的边信息通过queue保存，你也可以生成一个map返回
// 存储结构为邻接矩阵，对于其他存储结构也是一样的，有向边中出点属于V',入点属于V-V'
template<typename T>
void PrimMatrix(NearByMatrix<T> *map, SequentialQueue<edgeOfMinTree> *queue) {  //queue即树的边集
    int treeVex[map->numVertexes]; //生成树的顶点集，即顶点在mao中的下标
    bool isBelongToTree[map->numVertexes]; //[i]=true; 代表i顶点属于V'
    for (int k = 0; k < map->numVertexes; ++k) {
        isBelongToTree[k] = false;
    }
    int vexNumOfTree = 0; //V'中顶点数
    treeVex[vexNumOfTree++] = 0; //先随便将第一个顶点加入V'
    isBelongToTree[0] = true;
    int minValue, vex1, vex2;
    //直到所有顶点都在顶点集中
    while (vexNumOfTree < map->numVertexes) {
        minValue = 65535;//我觉得没人大于它吧, 记得初始化
        //寻找最小权值边
        for (int i = 0; i < vexNumOfTree; ++i) { //找到一个最小权值边，该边的一个顶点属于V'，另一个属于 V-V'
            for (int j = 0; j < map->numVertexes; ++j) { //所有顶点V
                if (!isBelongToTree[j]) { //但只寻找V-V'的顶点
                    int weight = map->edgeMatrix[treeVex[i]][j]; //V--(V-V')
                    if (weight != 0 && weight < minValue) {
                        minValue = weight;
                        vex2 = j; //该边在V-V'中的顶点
                        vex1 = treeVex[i]; //该边在V‘中的顶点
                    }
                }
            }
        }
        edgeOfMinTree edge{vex1, vex2, minValue};
        treeVex[vexNumOfTree++] = vex2;
        isBelongToTree[vex2] = true;
        queue->queueIn(edge);
    }
}


//如果两个顶点属于同一分量，那么他们分量中最后一个顶点必然是一样的
//找到start顶点所在分量的最后一个顶点
template<typename T>
//不写成泛型，就要在cpp文件中实现
int find(int start, int *group) {
    while (group[start] > -1) {
        start = group[start];
    }
    return start;
}

//克鲁斯卡尔算法，存储结构为边集数组{start,end,weight},且已经按start<end,weight从小到大排好序, queue保存边信息，顶点下标从0开始
template<typename T>
void KrusKal_MinimumTree(edgeInfoList<T> *map, SequentialQueue<edgeInfo> *queue) {
    int group[map->vexNum]; //分量标记, group[i]=k表示i,k同分量，-1则结束， 每个人只需知道另一个顶点，就跟排队一样，我只需知道前后左右，就能知道位置
    for (int i = 0; i < map->vexNum; ++i) {
        group[i] = -1; //有顶点下标为0，所以改成-1
    }
    while (queue->getLength() < map->vexNum - 1 && !map->list->isEmpty()) { //也可以直接遍历所有边
        edgeInfo edgeInfo{0, 0, 0}; //最小权值边
        map->list->getDataByIndex(0, &edgeInfo); //第一个最小权值
        //泛型方法，需要制定类型，如果参数不是泛型的话
        int m = find<int>(edgeInfo.start, group); //start所属分量最后一个顶点
        int n = find<int>(edgeInfo.end, group); //end所属分量最后一个顶点
        if (m != n) {
            group[n] = m; //只需让n所属分量知道分量中还有一个顶点m，相当于汇流，n所属分量不需知道m所属分量其他顶点，
            // 到时候寻找分量最后一个顶点自然会在过程中汇集在同一顶点m
            queue->queueIn(edgeInfo);
        }
        map->list->deleteNodeByIndex(0, &edgeInfo); //删除最小的边,即便没找到,没用了已经
    }
}


#endif //CLIONCPP_MINIMUMCOSTTREE_H
