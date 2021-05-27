#### DataStructure_CPP
#### 介绍

部分数据结构 C++实现

之前一段时间我尝试系统性的学习数据结构这门课程，我所选用的教材是程杰老师的《大话数据结构》

这本书怎么说呢，前面部分章节还行，后面的章节就有点让人匪夷所思了:sweat_smile:

图之前看着还阔以,后面的许多地方都是贴很长一段代码，没有实际参考价值，于是我直接去浏览相应章节知识点的博客内容，一步步搞懂。看完这本书后，我又去看了些别的资料补充一下。

 我这项目的代码写的的确有点乱，等有时间再仔细修订一下，目前自己看的懂就行



#### 难点

要说哪里最难的话，当属KMP算法和红黑树那块，真的难，看了2、3天才看懂，锻炼心态

其他地方的话，我想没啥特别难的地方，当然排序那块还有图的最短路径也思考了挺久的



#### 开发环境

C++11

CLion2020.1

win10

说实话cmake语法我到现在还没搞懂



#### 项目结构

注：*.txt都是额外的笔记说明

```yaml
DataStructure_CPP
|   .gitignore
|   CMakeLists.txt  
|   main.cpp          #自行调用相应模块
|   README.md
|   广义表.txt  	   #额外笔记
|   特殊矩阵.txt 	#额外笔记
|   
+---Find    #查找算法
|   |   ArrayWhichIsSorted.cpp    #简单的查找
|   |   binarySortedTreeAndBalanceTree.h #二叉平衡树
|   |   HashList.h   #简单散列表
|   |   RBTree.h  	#红黑树
|   |   其他查找结构.txt
|   |   查找应用.txt
|   |   
|   \---Test	#测试代码
|           balanceTreeTest.cpp
|           
+---List	#线性表
|   |   ArrayList.h #顺序表
|   |   LinkList.h #单链表
|   |   
|   \---Test
|           array.cpp
|           Link.cpp
|           
+---Map #图
|   |   DFS_OR_BFS.h #深搜和广搜
|   |   KeyPath.h   #关键路径
|   |   Map.h	#图的存储结构
|   |   MinimumCostTree.h	#最小生成树
|   |   mostShortPath.h #最短路径
|   |   Topological.h #拓扑序列
|   |   
|   \---Test
|           KeyPathMap.png
|           KeyPathTest.cpp
|           minPathMap.png
|           minPathTest.cpp
|           tuopoSort.cpp
|           拓扑.png
|           
+---Sort  #排序，自己文件名
|   |   bubble_sort.cpp
|   |   bucket_sort.cpp
|   |   choose_sort.cpp
|   |   counting_sort.cpp
|   |   direct_insert_sort.cpp
|   |   heap_sort.cpp
|   |   merge_sort.cpp
|   |   quick_sort.cpp
|   |   radix_sort.cpp
|   |   shell_sort.cpp
|   |   十大排序算法的复杂度.png
|   |   排序总结.txt
|   |   
|   \---declaration
|       |   bubble_sort.h
|       |   bucket_sort.h
|       |   choose_sort.h
|       |   counting_sort.h
|       |   direct_insert_sort.h
|       |   heap_sort.h
|       |   merge_sort.h
|       |   quick_sort.h
|       |   radix_sort.h
|       |   shell_sort.h
|       |   swap_sort.h
|       |   
|       \---utils
|               utils.cpp
|               utils.h
|               
+---StackAndQueue
|   +---Queue #队列
|   |   |   LinkQueue.h
|   |   |   SequentialQueue.h
|   |   |   
|   |   \---queueTest
|   |           queueTest.cpp
|   |           
|   \---Stack #栈
|       |   LinkStack.h
|       |   SequentialStack.h
|       |   
|       +---test
|       |       backExTest.cpp
|       |       LinkStackTest.cpp
|       |       SequentialStack.cpp
|       |       
|       \---Tool
|               backExpression.cpp
|               backExpression.h
|               
+---String #字符串
|   |   String.cpp
|   |   String.h
|   |   
|   +---KMP #KMP模式匹配
|   |       KMP.cpp
|   |       KMP.h
|   |       
|   \---Test
|           StringTest.cpp
|           
+---Tool #项目中用到的小工具
|   |   LinkNode.h
|   |   MathTool.h
|   |   Status.h
|   |   
|   \---Implements
|           MathTool.cpp
|           Status.cpp
|           
\---Tree #树
    |   BinaryTree.h #二叉树
    |   HuffManTree.h #赫夫曼树
    |   LinkChildTree.h #树的某种存储，我忘了...，看看下面的normalTree.cpp
    |   priorityQueue.txt #优先队列
    |   
    \---Test
            binaryTreeTest.cpp
            normalTree.cpp  
            
```



