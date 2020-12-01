
#ifndef CLIONCPP_MERGE_SORT_H
#define CLIONCPP_MERGE_SORT_H
/*
 * 归并排序 ： 稳定，时间复杂度O(nlog2n),过程中需要分成left,right部分
 * 归并排序主要通过分割、规定到达最终结果
 *
 * 一开始我们将待排序的数组arr分为两个部分left,right
 * 然后分别left,right再进行归并排序，即对他两再分为两部分
 * 以此类推，直到此时有一个需要归并的数组长度为1，此时直接返回
 * 那么返回后，原先一分为二的某个数组t,此时必然要面对将left,right合并为一个t的操作
 * t合并完后返回，t可能又是某个数组tt的一部分，接着合并tt.........
 *
 * 此时我们有两个排好序的左右数组,他们原先就是一个整体，此时需要合并
 * 我们只需要比较left,right第一个元素，谁更小，然后将最小值放在原数组arr的第一个
 * 然后比对left第一个和right第二个元素，将更小的值放在arr第二个
 * 假如此时 right没有第三个元素，就直接将left所有元素依次放在arr后面，以此类推，left,right的元素本就是arr的，可以填满，放心
 *
 * 我们可以模拟一下，假设存在arr{1,3,4,5,2,21,7,9},len = 8
 * 1.分为 left1{1,3,4,5}, right1{2,21,7,9}
 *    1.1 对于left1{1,3,4,5},分为l.l{1,2}, l.r{4,5}
 *         1.1.1 对于l.l{1,3}, 分为 l.l.l{1}, l.l.r{3}
 *                l.l.l和l.l.r会直接返回，因为长度为1
 *                这时将 {1},{3}合并为 {1,3}
 *         1.1.2 对于l.r{4,5},分为l.r.l{4},l.r.r{5}
 *               合并为{4,5}
 *
 *    对{1,3}, {4,5}合并为 {1,3,4,5}
 *
 *    1.2 对于r1{2,21,7,9}
 *          1.2.1 r1.l 分割合并为 {2,21}
 *          1.2.2 r1.r 分割合并为 {7,9}
 *
 *    {2,21},{7,9}合并为{2,7,9,21}
 *
 * 最终对{1,3,4,5}, {2,7,9,21} 合并 --> {1,2,3,4,5,7,9,21}, 归并结束
 *
 * 归并排序过程中只需要知道 low,high,mid即可，不需要真正的分割，我们设置了mid分割线，两边都不会干扰对方
 *
 */
#include "utils/utils.h"

//合并操作,将分别排好序的left{low~mid}部分和right{mid+1,high}部分合并到arr{low~high}中
//我只需告诉数组的界限即可合并,合并的操作范围可能只是arr的一部分
void merge(int *arr, int low, int mid, int high);

//归并排序，递归实现，真正的逻辑处理, low,high是数组的最小的小标和最大下标
void merge_sort(int *arr, int low, int high);

//迭代实现的归并排序
void merge_sort_dieDai(int *arr, int len);

//归并排序，升序,传入数组arr,及其长度len，调用的是递归实现的归并排序
void merge_sort_main(int *arr, int len);

#endif //CLIONCPP_MERGE_SORT_H
