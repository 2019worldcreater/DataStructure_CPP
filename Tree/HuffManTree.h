//
// Created by HP on 2020-11-21.
//

#ifndef CLIONCPP_HUFFMANTREE_H
#define CLIONCPP_HUFFMANTREE_H

/*
 * 赫夫曼树即最优二叉树，同于减少数据的存储或处理次数
 *
 * 比如在判断语句中，if() else if(),else if()...语句他会先判断第一个if，如果是真就进入其中，否则往下，继续判断
 * 这种只有真、假的结果，并进入不同的方向，就很像二叉树的左右孩子，我们可以假设左孩子即为1的一方通路，右孩子为0
 * 那么上面的这一连串if,可以将第一个判断条件保存为根节点，其左孩子节点保存最终应该进入的状态，右孩子保存第二个判断条件，....依次类推
 * 最后叶子节点即是不同的状态，非叶子节点即判断，但每次从根节点开始， 那么从根节点到一个叶子叶子节点的路径长度 len（连接线的根数），即为判断次数
 *
 *  成绩等级划分：
 *  int grade = 0, status = 0;
    if (grade < 60) {
        status = 1;
    } else if (grade < 70) {
        status = 2;
    } else if (grade < 80) {
    }

    非叶子节点可以保存其分界分数，大于进入右孩子，小于进入左孩子


    通常我们希望判断次数尽可能的少，就比如第一个判断语句对于大部分人来说都是无用的判断，因为有几个人会不及格，如果我们能合适的改造一个二叉树，就能
    极大的减少总体判断次数，把大部分人处于的状态尽可能放在前面的判断

    如果我们给叶子节点设置一个权值，该权值即数据统计中该状态的百分占比，为了计算方便，统一乘100
    那么一个叶子节点的路径长度 * 权值 /100 即为进入该状态的平均比较次数
    所有叶子节点的路径长度 * 权值 / 100的总和即为计算所有出状态的平均比较次数

    其中WPL称为带权路径长度 = 路径长度 * 权值

    那么减少WPL总和就可以有效减少总处理次数，以此为目标构造出来的二叉树称为赫夫曼树，即WPL总和最少

    首先列出所有叶子节点，以及它们的权值,然后从中挑选权值最小的两个节点，创造一个根节点root，将较小的节点设为其左孩子，另一个为右孩子，
    然后将这个组合的根节点加入叶子节点序列，它的权值即为左右孩子权值的总和，...不断循环该过程，知道只剩下一个节点，此时赫夫曼树构造完成


    赫夫曼编码，如果只传输几种字符，比如ABCDEFG，那么使用三位二进制来表示它们是很常见的想法
    但是为什么我们一定要用三位二进制来表示每个字符呢，无非就是因为00\001这两个位数不同，前缀相同，导致接受方无法得知是传输00还是001

    如果我们构造成赫夫曼树，字符序列为ABCDEFG,即叶子节点，权值即字符出现的频率百分比，频率之和 = 1,但为了计算方便，我们乘以100
    构造完赫夫曼树后，我们取左孩子路径为0,右孩子为1，然后字符的编码即从根节点到对应叶子节点的路径组成的01序列，这样我们就得出了相应的编码

    由于每个字符都是叶子节点，这就保证了所有字符的编码，没有相同的前缀，比如因为 00到达了叶子，001不可能存在
    这就大大减少了发送方需要发送的数据量
    接受方需要遵守同样的约定，在生成的赫夫曼树上，接受01序列，从根节点开始按照左0右1的顺序遍历，如果遍历到叶子，则输出字符，然后再来解下一个字符

    在赫夫曼树中，所有非叶子节点都有2个孩子，即只存在度为2、0的节点
    当赫夫曼树构造完后得出所有字符的编码的算法：printCode(node *root, MyString *str, char path, queue *queueCode,queue *queueChar,int layer){
        if(root != null){
             if(layer != 0)
                生成一个新的MyString,复制str内容，将path拼接上去; 当然根节点不要
           if(root.leftChild != null) printCode(root.leftChild, str,'0',layer+1);
           if(root.rightChild != null) printCode(root.rightChild, str,'1',layer+1);
           if(是叶子节点){
                queueChar.queueIn(root.data);
                queueCode.queueIn(str);
           }
        }
        if(path == '1' && layer > 0) //
            delete str; //释放原数组内存

    }
 */
#include "BinaryTree.h"
#include "../List/ArrayList.h"
#include "../Tool/MathTool.h"

template<typename T>
//叶子节点序列，权值序列，返回最终生成的整棵树的root,
trackTree<T> *
constructHuffManTree(ArrayList<trackTree<T> *> *leaves, ArrayList<int> *weight) { //务必确保权值与叶子节点的下标一致
    if (leaves != nullptr && !leaves->isEmpty() && weight != nullptr && !weight->isEmpty()) {
        while (leaves->getLength() != 1) { //只剩一个节点
            trackTree<T> *root = createThread('h'); //用于组合两个最小权值节点
            int sum = 0; //组合的权值总和
            for (int j = 0; j < 2; ++j) { //找两次最小节点
                int min, value, index = 0; //最小权值，暂存值，最小权值节点下标
                weight->getDataByIndex(0, &min);
                for (int i = 1; i < weight->getLength(); ++i) {
                    weight->getDataByIndex(i, &value);
                    if (value < min) {
                        min = value;
                        index = i;
                    }
                }
                trackTree<T> *child; //获得权值最小的节点
                leaves->getDataByIndex(index, &child);
                if (root->leftChild == nullptr) //较小的作为左孩子
                    root->leftChild = child;
                else
                    root->rightChild = child;
                weight->deleteNodeByIndex(index, &value); //从线性表中删除该节点
                leaves->deleteNodeByIndex(index, &child);
                sum += min;
            }
            leaves->addItem(root); //将组合成的节点及权值加入节点序列
            weight->addItem(sum);
        }
        trackTree<T> *root = nullptr;
        leaves->getDataByIndex(0, &root); //返回最后一个剩下的节点
        return root;
    }
    return nullptr;
}

#endif //CLIONCPP_HUFFMANTREE_H
