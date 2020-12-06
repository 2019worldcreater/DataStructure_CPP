//
// Created by HP on 2020-11-24.
//

#ifndef CLIONCPP_KEYPATH_H
#define CLIONCPP_KEYPATH_H

/*
 * 有向无环图问题
 *
 * 之前的拓扑序列是为了确定事件的完成顺序
 * 其实很多时候需要知道完成一个事件的最短时间，比如在一个造车工程中，造发动机时，其他的车间就在造外壳、底盘啥的
 * 而想组装车子就要等到所有零件等完成，而这个时间-->Max(造各种零件) + 组装时间, 其中的关键路径即 造最长时间的零件->组装，这条路径决定了项目的最短时间
 * 当工程的结构、分工更加复杂时，就需要一种有效手段求解关键路径
 *
 * 之前的AOV网图使用顶点表示事假，边表示制约关系
 * 但现在需要知道事件的持续时间，当然该时间应作为边的权值存储，可谁表示事件呢，是边的head?,不行，可能存在多边的情况
 * 于是我们可以引入“状态”(事件)作为顶点，而有向边本身就代表一个活动
 * 这样的图称为 AOE网，通常图中只会有一个源点(只有出边)和一个终点(只有入边)
 * 从源点到汇点具有最大长度的路径叫关键路径
 * 比如 V0代表项目启动状态, V1代表work1完成状态, <0,1>则代表完成work1的活动,权值即为时间， Vn-1项目结束状态
 *
 * 事件(状态)T的最早发生时间即从入度为0的事件K开始，执行到T的最早时间,记etv(T),其中etv[k]=0, 如果只有一个边edge1从I指向事件T
 * 那么 etv[T] = etv[I] + edge1.weight, 如果有多个边从 I,J,K指向T，则取Max[etv[?] + edge?.weight],毕竟要等到所有前驱事件完成
 * 按照拓扑序列计算，就一定符合计算正确性
 *
 * 事件的最晚发生时间即不影响项目延期的最晚时间，如果我们求得了项目终点 P的最早完成时间，那么 ltv[P]=etv[P],能尽早就尽早
 * 之前说过事件T可能要等到I,J,K全部完成才能发生，那么假如K最晚，那么etv[T]取决于etv[k]+edgeKT.weight, 那么这样的话
 * 其他顶点I,J就会存在一个最晚开始时间，只要不超过这个时间，那么etv[T]不会受影响,但同时T事件也可能会有一种拖延的机会即ltv[T],
 * Ltv[I]的值取决于ltv[T]-IT.weight, 如果I同时指向多个事件abc，那么则需要取ltv[a]-ia,ltv[b]-ib,ltv[c]-ic其中的最小值(其实本意就是让项目最早完成)
 * 所以最晚开始时间要从项目终点P开始往前计算（拓扑序列的反序列提供了这个顺序），只有这个项目最后的最晚时间，之前的事件才有了拖延的凭证
   汇点的最早开始时间即最晚开始时间
 *
 * 若活动(边)ai是由弧<vk , vj>表示，则活动ai的最早开始时间应等于事件vk的最早发生时间。因此，有：e[i]=etv[k]
 * 活动ai的最晚开始时间是指，在不推迟整个工期的前提下， ai必须开始的最晚时间。若ai由弧<vk，vj>表示
 *  则ai的最晚开始时间要保证事件vj的最迟发生时间不拖后。因此，有：l[i]=ltv[j]-<vk,vj>.weight
 *
 * 其中 e[i]=l[i]的活动边必然处在关键路径上---求解
 *
 * 其实事件T的etv和ltv相等的话，也必然处在关键路径上，但不能这么武断输出顶点，因为关键路径不止一条，只告诉顶点，很难确定具体路径

    根据结果中的活动边描绘，就可以找到关键路径，如果产生了分支，说明有多个关键路径，其中项目的最短完成时间，取决于所有关键路径
    只让一个关键路径变短也不行，于是可以直接让多个关键路径重复的活动边变短即可
 */
#include "Topological.h"
#include "../StackAndQueue/Stack/SequentialStack.h"

//求解map中的关键路径，并将路径上的边信息保存在queue中
template<typename T>
void findKeyPath(tuopuMap<T> *map, SequentialQueue<edgeInfo> *queue) {
    int etv[map->numVex]; //事件最早发生时间
    for (int i = 0; i < map->numVex; ++i) {
        etv[i] = 0; //初始化
    }

    SequentialStack<int> *stack = new SequentialStack<int>(); //反序列
    SequentialQueue<int> *queueOfTuopo = new SequentialQueue<int>(); //正序列

    //拓扑序列恰好能保证前者的etv已经求解
    topoLogicalSort(map, queueOfTuopo, etv); //求解拓扑序列，顺便计算etv, 请确保map参数都输入正确

    int ltv[map->numVex]; //事件最晚发生时间
    for (int j = 0; j < map->numVex; ++j) {
        ltv[j] = etv[map->numVex - 1]; //终点的最晚即是最早，其他的通过终点来反向计算
    }

    //由于要从终点往前计算，恰好拓扑序列的逆序列是个不错的参考，先保存在栈中，然后去栈顶就能反向了
    while (!queueOfTuopo->isEmpty()) {
        int head;
        queueOfTuopo->queueOut(&head);
        stack->push(head);
    }

    topoNode<T> node; //用来得到顶点原信息
    ArrayList<topoNode<T>> *list = map->list;
    LinkList<AdjacencyNodeWithWeight> *list1 = nullptr;
    AdjacencyNodeWithWeight adNode;
    //计算ltv,拓扑的反序列，恰好能保证前者的ltv已经被求解，即所有出边的另一个顶点的ltv已得出
    while (!stack->isEmpty()) {
        int head;
        stack->pop(&head); //出栈
        list->getDataByIndex(head, &node); //顶点信息
        list1 = node.outEdge; //出边表
        for (int i = 0; i < list1->getLength(); ++i) { //遍历每个出边
            list1->getDataByIndex(i, &adNode); //边信息
            if (ltv[head] > ltv[adNode.index] - adNode.weight) { //<head,tail>
                ltv[head] = ltv[adNode.index] - adNode.weight; //取最小值
            }
        }
    }

    for (int k = 0; k < map->numVex; ++k) { //遍历所有边(活动)ai，查看其e[i]和l[i]是否相等
        list->getDataByIndex(k, &node);
        list1 = node.outEdge;
        for (int i = 0; i < list1->getLength(); ++i) {
            list1->getDataByIndex(i, &adNode);
            int tail = adNode.index;   //<k,tail>
            int e = etv[k]; //活动的最早开始时间
            int l = ltv[tail] - adNode.weight; //活动的最晚开始时间
            if (e == l) { //关键活动
                edgeInfo edge; //将边信息保存
                edge.start = k;
                edge.end = tail;
                edge.weight = adNode.weight;
                queue->queueIn(edge);
            }
        }
    }
    delete stack;
    delete queueOfTuopo;
}

#endif //CLIONCPP_KEYPATH_H
