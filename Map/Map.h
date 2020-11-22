//
// Created by HP on 2020-11-21.
//

#ifndef CLIONCPP_MAP_H
#define CLIONCPP_MAP_H

/*
 * 图G(V,E),V是有穷"非空"顶点集，E是边集（可以空）
 *
 *  无向图：图中所有边都是无向边，(A,D)==(D,A) == A--D
 *  有向图：图中所有边都是有向边（通常叫弧） <A,D> == A-->D != <D,A>
 *  简单图：不存在指向自身的边、不存在重复的边
 *  无向完全图：任意两个顶点都有一条边，n个顶点-->总边数 n(n-1)/2
 *  邮箱完全图：任意两个顶点之间都存在方向互为相反的两条弧,即须同时 <A,D> <D,A>, 含有n个顶点的有向完全图有n×(n-1)条边
 *  权：无向、有向边具有的数值，带有权的图为网，比如地图中两点间的权即为距离
 *  子图： G(V,E), G'<V',E'> 如果有 V' 是 V 的子集 且 E‘ 是 E 的子集，则 G' 是 G 的子图
 *
 *  无向图中，如果存在 (A,D),则称A、D互为邻接点，边(A,D)依附于A、D
 *      度：一个顶点关联的边数，即为 TD(V), 很明显图所有顶点的 总度数 / 2 = 总边数
 *
 *   有向图中：如果存在 <A,D>, 表示 A邻接到D，D邻接自A，
 *      度： 假如有 <A,D>,<C,D>,<C,A>，  A的入度为1,出度为1；D的入度为2，出度为0；C的出度为2，入度为0
 *          即有向边的前面即为出，前面即为入， 入度即为 ID、出度 OD， 度 = 出度 + 入度
 *
 *   路径：一个顶点到另一顶点过程中经过的顶点序列(包括两端)，且路径必须经过图中的边/弧，如果是有向图，就必须按照方向走
 *   路径长度：经过的边/弧数
 *   简单路径：路径序列中不出现重复的顶点
 *
 *   环：路径系列中第一个顶点和最后一个顶点相同
 *   简单环：环中只有序列两端的顶点才相同, <A,D,D,A> 就不是简单环
 *
 *   连通图：无向图中任意两顶点都能连通（即存在路径可到达）
 *   无向图G的极大连通子图(连通分量)G': G'是G子图; G’连通; G'如果有顶点D，那么G中D关联的所有边都要出现
 *
 *   强连通图：有向图中，任意两个顶点间都存在路径可到达
 *   有向图的强连通分量（极大强连通子图）： 子图、强连通
 *
 *   连通图的生成树：有n个顶点n-1条边的连通图
 *   如果一个图有n个顶点和小于n-1条边，则是非连通图，如果它多于n-1边条，必定构成一个环
 *
 *   如果一个有向图恰有一个顶点的入度为0，其余顶点的入度均为1，则是一个有向树
 *   一个有向图的生成森林由若干棵有向树组成，含有图中全部顶点，但只有足以构成若干棵不相交的有向树的弧， 即去掉一些弧使得图断开为几棵有向树
*/

/*
 * 第一种存储方法：邻接矩阵
 * 一个一维数组保存顶点。一个二维数组edgeMatrix保存边信息
 * edgeMatrix[i][j] = 1 ( 存在(Vi,Vj)[(Vj,Vi)] 或 <Vi, Vj> ), 0 不存在
 * 如果要存储带权值的图，只需将1-->权值，0-->不可能数，因为权值可能是0或负数
 *
 * 无向图的邻接矩阵是以左上-右下对角线为轴，两边对称。 即对称矩阵,即 matrix[i][j] = matrix[j][i]
*  无向图中顶点Vi的度，即matrix[i][0]~matrix[x][n-1]的和， 而检查是否Vi邻接于Vj，只需matrix[i][j]=1
 *
 *  有向图中的邻接矩阵不是对称矩阵，因为边是有向的, Vi的出度即matrix[i][0]~matrix[x][n-1]的和， 入度即 matrix[0][i]~matrix[n-1][i]的和
 */
template<typename T>
class NearByMatrix {
private:
    T vertex[4]; //保存顶点信息
    int edgeMatrix[4][4]; //nxn方阵
    int numVertexes, numEdges;
    /*
     * 创建和初始化，只需读入顶点，将matrix先全设为NoEdge
     * 然后输入numEdges条边的 i\j\权值， matrix[i][j]=权值
     * 如果是无向图,那么加一条语句，matrix[j][i]=matrix[i][j]
     */
};

/*
 * 第二种方法：邻接表，类似树的孩子表示法，参考LinkChildTree， 如果边数很少，那么nxn方阵会有很多浪费
 * 即一种顺序线性表用顺序表存储所有顶点，每个顶点还有一个单链表(边表)，存储与其邻接的顶点下标和边的权值
 * 意味着一条无向边在两个顶点的边表中都有存储
 *
 * 无向图中:    如果Vi是否邻接Vj只需遍历Vi的边表，是否有j下标
 *              Vi的度即为其边表节点个数
 *
 * 有向图的话，可以考虑使用两个边表，一个表保存以Vi指向的所有顶点下标，一个表保存指向Vi的所有顶点下标
 */
#include "../List/LinkList.h"
#include "../List/ArrayList.h"

struct AdjacencyNodeWithWeight {
    int index; //邻接顶点在顺序线性表中的下标
    int weight; //如果链表属于 Vi,那么边为 (V.i,V.index)， 权值
};
template<typename T>
struct AdjacencyListNode {
    T data; //顶点的信息
    LinkList<AdjacencyNodeWithWeight> *list; //边表，里面保存与该顶点邻接的顶点的下标以及两者之间边的权值
    //reverseList 有向图的逆邻接表，以此获得入度3
};

template<typename T>
class AdjacencyList {
    ArrayList<AdjacencyListNode<T>> *list; //存储所有顶点
    int numVertexes, numEdges; //初始化同样要输入i,j,权值，只不过其中涉及插入链表操作
};

/*
 * 第三种，十字链表, 对邻接表的改进， 适用于有向图3
 * 如果我们想知道入度，在上面的邻接表中就需要再存储入边，但这条边在对方看来就是出边，这样的话一条边被保存了两次，就因为分为入和出
 * 如果我们不另外存储顶点K的入边，而是指向别人出边表中的出边，我们就知道，这条出边的tail就是K，head就是该出边链表的拥有者
 * 之前的出边表节点只保存tail的下标或权值，但现在出边表中的节点同时需要存在于tail顶点的入边表中，所以之前的节点结构需要点扩充
 * 所以节点直接保存边的head，tail、权值，就知道这条边的两端，不需我们去找拥有者，单链表总不能往回找
 * 因为出边表、入边表中的节点也需要串联起来，所以同一个结点需要两个指针
 */
struct TenLinkNode {
    int head; // <A,D> 则 head是A顶点的下标
    int tail; // tail是D顶点的下标
    int weight; //权值
    TenLinkNode *nextInEdge; //指向下一个tail相同的节点，即tail顶点的下一个入边
    TenLinkNode *nextOutEdge; //指向下一个head相同的节点，即head的下一个出边
};
template<typename T>
struct TenNode {
    T data; //图顶点信息
    //现在的边表节点不再是 data,next结构，有两个next出口，所以不能用之前的LinkList来代替，要自己实现，不必实现全部功能
    TenLinkNode *inEdge; //入边表 ，指向该顶点的第一条作为被入端的边节点
    TenLinkNode *OutEdge; //出边表  指向该顶点的第一条作为出端的边节点
};

template<typename T>
class TenLink {
    ArrayList<TenNode<T>> *list;
    int numVertexes, numEdges;
    /*
     * 先输入所有节点，初始化TenNode，然后输入边信息: 出、入顶点、权值
     * 假如第一条边输入  A,D,100,   那么先生成一个边表节点node保存信息，在A顶点的出边表中加入该边节点outEdge = node,
     * D顶点的入边表中加入该边节点 inEdge=node，这个节点被两方指向
     * 如果还有 <B,D,20>， 创建节点 node2, B的outEdge = node2
     * 对D的入边表遍历，找到最后一个入边，此时即node, node->nextInEdge = node2,即tail相同的节点
     * 如果有<A,B,20>, node3, B的inEdge = node3, 遍历A的出边，找到最后一个即 node, node->nextOutEdge = node3,即head相同
     */
};


/*
 * 第四种：邻接多重表，适用于无向边，相当于一条边也不需要两次存储了
 * 所以一条边被2个指向，所以需要存储两段顶点，否则就无法的值指向者是否关联该边
 * 由于需要将一个顶点关联的边串联起来，所以可以考虑边分别指向两个端点关联的下一个边
 *
 * 初始化方法：输入顶点...
 * 第一个边，（A,D,20）, node{ivex=A.index,jvex=D.index}, A.edge=node, D.edge=node
 * (C,D,10),node1,C.edge=node1,搜索D的最后一条关联边,即node,node.jvex->D, 且jlink=null,令 node.jlink=node1
 * (B,A), node3. B.edge=node3, A的最后一个边为node,node.ivex->A, 并且ilink=null,所以 node.ilink=node3
 * (C,A), node4,  C.edge==node1, node1.ivex->C,node1.ilink==null ... node1.ilink=node4..... 很明显node3是A的最后一个边,node3.jlink=node4
 *
 * 所以其实就是加边时，分别对两个顶点的边表加上这个新边，如果edge为空，只需单纯指向它，否则先看第一个边的哪个vex指向该顶点，那么相应的link就是它的下一个关联边，
 * 如果link为null，说明当前边就是last edge, 否则前往link，继续上述操作
 *
 * 而像删除一个边也很简单，假如删除(A,D), 那么先找到A、D边表中指向(A,D)或(D,A)边节点的指针所在节点link
 * 假如A最后一个边的是ilink,(A,D)边节点中A是ivex，那么令ilink=(A,D).ilink,
 * 即指向下一个关联边
 * 最后free该边(A,D)节点
 */
//边表中的节点
struct duoChongNode {
    int ivex, jvex; //边的两段顶点在数组中的下标
    int weight;
    duoChongNode *ilink; //指向ivex关联的下一条边表节点
    duoChongNode *jlink; //指向jvex关联的下一条边表节点

};
template<typename T>
struct duochongVex { //顺序表中保存的节点
    T data;
    duoChongNode *edge;
};


/*
 * 方式5 边集数组，应该是最简单的保存方法了，但用着也很不方便
 *  即两个一维数组，一个保存顶点信息，一个保存边信息
 *  vexInfo vexs[], edgeInfo edges[]
 */
struct edgeInfo {
    int start, end, weight; //边的两顶点在数组中下标
};
#endif //CLIONCPP_MAP_H
