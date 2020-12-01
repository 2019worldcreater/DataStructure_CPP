cmake_minimum_required(VERSION 3.16)
project(ClionCPP)

set(CMAKE_CXX_STANDARD 11)

add_executable(hello main.cpp List/ArrayList.h List/LinkList.h StackAndQueue/Stack/LinkStack.h StackAndQueue/Stack/SequentialStack.h StackAndQueue/Queue/SequentialQueue.h Tool/LinkNode.h Tool/MathTool.h Tool/Implements/MathTool.cpp StackAndQueue/Stack/Tool/backExpression.h StackAndQueue/Stack/Tool/backExpression.cpp StackAndQueue/Queue/LinkQueue.h String/String.h String/String.cpp String/KMP/KMP.h String/KMP/KMP.cpp Tree/LinkChildTree.h Tree/BinaryTree.h Tree/HuffManTree.h Map/Map.h Map/DFS_OR_BFS.h Map/MinimumCostTree.h Map/mostShortPath.h Map/Topological.h Map/KeyPath.h Find/binarySortedTreeAndBalanceTree.h Find/HashList.h Sort/declaration/heap_sort.h Sort/declaration/shell_sort.h Sort/shell_sort.cpp Sort/declaration/direct_insert_sort.h Sort/direct_insert_sort.cpp Sort/declaration/utils/utils.h Sort/declaration/utils/utils.cpp Sort/declaration/bubble_sort.h Sort/bubble_sort.cpp Sort/declaration/swap_sort.h Sort/heap_sort.cpp Sort/declaration/choose_sort.h Sort/choose_sort.cpp Sort/declaration/merge_sort.h Sort/merge_sort.cpp Sort/declaration/quick_sort.h)
