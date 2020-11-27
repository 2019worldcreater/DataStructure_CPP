//
// Created by HP on 2020-11-24.
//

#ifndef CLIONCPP_TOPOLOGICAL_H
#define CLIONCPP_TOPOLOGICAL_H

/* 有向无环图问题
 *
 * [这样的理念也适用于工程建设（有很多个子活动）]
 * 当我们尝试去学习一个新的技术T时，往往会思考学习T之前需要掌握哪些技术，假如是 I,J,K
 * 然后呢，我们又会去思考I,J,K的前驱技术是什么，当然有可能I是K的前驱技术之一，而不需要前驱技术的 J 可以无门槛直接学习
 * 当我们整理好所有技术路线后，就会得到一个有向图，其中<I,K>代表 需等到 I 完成才能去完成 K, 于是当被多个边指向时，就要完成之前的所有
 * 这样的有向图,用顶点表示活动，用弧表示活动之间的优先关系，称为AOV网
 * 一个合格的AOV网，自然不能存在回路，总不能吃面在泡面之前吧
 *
 * 所以说找到一个合适的学习路线(活动完成次序)是很常见的一种问题，而这个过程则称为拓扑排序
 * 拓扑排序生成一个顶点序列，并保证按这个序列去执行必然不会出错, 即如果有 <i,j>那么i的位置必然在j之前
 *
 * 算法：找到入度为0(可以直接完成)的顶点J输出，删掉与其关联的边，即代表J完成，其他活动不必受其制约
 * 重复上述过程，直到没有入度为0的顶点(实际上和层序遍历很像), 如果最后成功输出了全部顶点则证明是合格的AOV，否则出现回路
 *
 * 既然需要查找入度，我就干脆将入度的值告诉每个顶点即可，而结构也沿用之前的邻接表，因为邻接表的节点保存了tail顶点的下标
 * 删掉边(其它的处理后期再说)即将[tail].inWay--,此时如果tail的in也是0，则加入待完成序列
 */
#include "../List/ArrayList.h"
#include "../List/LinkList.h"
#include "../StackAndQueue/Queue/SequentialQueue.h"
#include "Map.h"

template<typename T>
struct topoNode {
    int inWay; //入度值
    T data; //顶点信息
    LinkList<AdjacencyNodeWithWeight> *outEdge = nullptr; //出边表{{index,weight},next}
};

template<typename T>
class tuopuMap {
public:
    ArrayList<topoNode<T>> *list;
    int numVex;

    tuopuMap(int vex) {
        this->numVex = vex;
        list = new ArrayList<topoNode<T>>();
    }

    //增加顶点
    void addVex(T data, int in) {
        topoNode<T> node;
        node.data = data;
        node.inWay = in;
        node.outEdge = new LinkList<AdjacencyNodeWithWeight>();
        list->addItem(node); //结构体也可以赋值添加
    }

    //增加边<vex,next>
    void addOuEdge(int vex, int next, int weight = 0) {
        topoNode<T> node;
        list->getDataByIndex(vex, &node);
        AdjacencyNodeWithWeight Linknode = {next, weight};
        LinkList<AdjacencyNodeWithWeight> *list1 = node.outEdge;
        list1->addNode(Linknode);
    }

    ~tuopuMap() {
        topoNode<T> node;
        for (int i = 0; i < list->getLength(); ++i) {
            list->getDataByIndex(i, &node);
            LinkList<AdjacencyNodeWithWeight> *Linklist = node.outEdge;
            Linklist->releaseLinkList();
            delete Linklist;
        }
        delete list;
    }
};


//拓扑排序，传入邻接表Map和保存顶点序列的队列， etv是保存事件状态的最早成型时间（用于求关键路径，求拓扑时可不传）
template<typename T>
void topoLogicalSort(tuopuMap<T> *map, SequentialQueue<int> *queue, int *etv = nullptr) {
    //如果你使用栈，输出的顺序也会有一点变化
    SequentialQueue<int> *queue1 = new SequentialQueue<int>(); //过程中用到的
    topoNode<T> node; //用来得到顶点原信息
    ArrayList<topoNode<T>> *list = map->list; //直接操作方便
    //先将一开始都入度为0的顶点下标加入输出队列
    for (int i = 0; i < list->getLength(); ++i) {
        if (list->getDataByIndex(i, &node)) {
            if (node.inWay == 0) {
                queue1->queueIn(i);
            }
        }
    }

    if (etv != nullptr)
        for (int j = 0; j < list->getLength(); ++j) {
            etv[j] = 0; //如果是合格的AOE网图，那么入度为0的点只有一个I,etv[I]=0,其他顶点的etv都是基于该点的
        }

    int head; //队头
    while (!queue1->isEmpty()) {
        queue1->queueOut(&head);
        queue->queueIn(head); //输出
        list->getDataByIndex(head, &node); //对消除该顶点的出边，逻辑上的
        LinkList<AdjacencyNodeWithWeight> *Linklist = node.outEdge;
        for (int i = 0; i < Linklist->getLength(); ++i) {
            AdjacencyNodeWithWeight node1;
            int point; //<head,point>
            Linklist->getDataByIndex(i, &node1);
            point = node1.index;
            list->getDataByIndex(point, &node);
            node.inWay--; //相当于少了一条边指向自己
            list->changeDataByIndex(point, node); //更改node信息
            if (node.inWay == 0) { //如果入度为0，加入队列
                queue1->queueIn(point);
            }

            //顶点i的最早开始时间要取最大的值，所以每次都要判断当前<head,index>
            //此时的etv[head]必然已经求解出来，因为其度为0
            if (etv != nullptr && (etv[head] + node1.weight) > etv[point])
                etv[point] = etv[head] + node1.weight; //前一个点的最早开始时间和一个边活动的持续时间
        }
    }

    delete queue1;
}

#endif //CLIONCPP_TOPOLOGICAL_H
