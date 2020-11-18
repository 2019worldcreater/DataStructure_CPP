//
// Created by HP on 2020-11-15.
//

#include "KMP.h"

//"abc",next[4],3  --> next[0]=-1,next[n]代表前n个字符的最长串长度
void calculateNext(const char *str, int *next, int len) {
    /*
     *  next[n]代表字符串前n个字符组成的子串的长度相同且相等的最长前后缀串的长度
     *  就比如 "abab"中前缀(以首字符开头不包括尾字符的子串)有: "a"、"ab","aba"; 后缀(概念相反)："b","ab","bab"
     *  那么"abab"中很明显长度相同且相等的最长前后缀串是 "ab", 那么 next[4] = 2
     *
     *  最简单求字符串的最长相等前后缀的方法： 从 index = len(str) - 1(先看看最长串是否一样)开始,
     *  比较str[0]~str[index] 和 str[len - index] ~ str[len - 1], 如果两个字符串相等则直接退出返回index+1，否则index--，直到index小于0
     *
     *  虽然上述方法简单，但太低效了,如果在计算next[k]时我们能利用之前算的next[0]~next[k-1],就好了
     *  推导： 如果 next[q] = k, 会有这个：(像两个板子尽量合在一起使重合部分一模一样但不能完全覆盖)
     *                                  str[0] ~ str[k-1] ~ str[q-1]
     *                    str[0] ~ str~[len-k] ~ str[q-1]
     *
     *   即 str[0] ~ str[k-1] 和 str~[q-k] ~ str[q-1] 相等
     *  1、 此时计算 next[q+1], 如果 str[k] = str[q], 那么 next[q+1] = next[q] + 1: 没毛病
     *
     *                                  str[0] ~ str[k-1]str[k] ~ str[q-1]str[q]
     *                    str[0] ~ str~[q-k] ~ str[q-1]str[q]
     *
     *   那你咋知道next[q+1]不能>K+1呢，确实，如果是一般的情况下，重合长度变长，有这个可能
     *   假如存在 next[q + 1] = j > k + 1 :
     *                          str[0] ~ str[j-2]str[j-1] ~ str[q]
     *                str[0] ~ str[q-j] ~ str[q-1]str[q]
     *   此时把上下两个str[q]都挡住，此时很明显： next[q] = j-1 > k, 所以不符， next[q+1] <= k+1
     *
     *  2、 如果 str[k] != str[q]，那么只能在 str[0]~str[k-1]中尽量找某个长度为j + 1的前缀串str[0]~str[j]
     *   使得其和 str[q-k+j]~str[q-1]（即在str[q-k]~str[q-1]中长度相等的后缀串）相等，并满足str[j+1] = str[q],此时 next[q+1] = j + 1
     *
     *                             str[0]   ~   str[j]str[j+1]~str[k-1]str[k] ~ str[q-1]str[q]
     *         str[0] ~ str~[q-k]~str[q-k+j]~str[q-1]str[q]
     *
     *   当然我们不需要一个个试j值，其实我们已经有了j的值的最佳候选，仔细看看，我们已经证明了str[0] ~str[k-1] = str[q-k]~str[q-1]
     *   如果要找前者的前缀串与后者的后缀串最大相等长度，再者两者相等，那就不是找str[0] ~str[k-1]长度相同且相等的最长前后缀串的长度*
     *   这不就是next[k]的定义吗，此时 k < q,所以 next[k] 已知
     *   所以我们只需要令 k = next[k], 此时 str[0]~str[k-1]和 str[q]之前的k个字符必然相等，如果此时 str[k] = str[q]
     *   那么 next[q + 1] = k + 1, 否则继续上述步骤找内部的最长串，k=next[k],直到k = 1,此时若还不满足, next[q+1] = 0,没辙了
     *
     *  所以实现的的是递归函数，当然没必要是参数递归，可以内部递归，递归并不一定要显式调用子身，需要用到以前计算的结果才叫递归
     */
    next[0] = -1; //递归结束位, k < 1不就是 k = next[k] <= -1
    next[1] = 0;
    int k = next[1]; //k是上一个next的结果
    for (int i = 2; i <= len; ++i) {
        while (str[k] != str[i - 1] && k > -1) {
            k = next[k]; //回溯
        }
        if (k > -1 && str[k] == str[i - 1]) { //找到了
            k++;
        } else {
            k = 0; //还没找到
        }
        next[i] = k;
    }
}

int KMP(const char *str1, const char *str2, int len1, int len2, int *result, int maxNum) {
    /*
     * 低效的查找：从str1的第一个字符开始，以此为起点逐个比对其后的字符，如果匹配失败，则以下一个字符为起点继续对比
     * 如果是 "00000000 0001" --> "0001", str前面的每个字符比对了4次才发现不对劲，这样话时间复杂度很大
     *
     * 如果我们匹配失败后，不是转到下一个字符然后又从头比对，而是有目的的跳转，并跳过不必要的对比步骤，将会更高效
     *
     * KMP算法利用的是子串的对称性，假如 str1[j -k + 1] ~ str1[j] = str2[0]~str2[k-1], 此时 str1[j+1]!=str2[k
     * 那么我们只需将 k = next[k], 即已匹配部分的相等前后缀最长长度, 然后继续比对 str1[j+1] 和 str2[k],如果依然失败,继续回溯next[k],直到k=0
     *
     * 推导 ：     str[j-k+1] ~ str[j]str[j+1]
     *               t[0]      ~  t[k-1]t[k]
     * 既然str[j+1]和t[k]匹配失败，那么子串肯定要往后移寻找相等串，移动过程中，我们又得从头开始，匹配长度为0，于是我们可以尽量保留一部分匹配
     * 不必每次从t[0]开始对比，而这个保留的部分就是子串已匹配部分最长相等前后缀， 假设 next[len(匹配长度)] = i
     *   str[j-i-1]~str[j] = t[0]~t[i-1], 这时主串的指针不变，子串的指针 k = next[len(..)]=i;
     *   然后再来对比 str[j+1]和 t[k], 失败则重复上述步骤
     *
     *   我们去掉了没必要的匹配过程
     *   根据最长相等前后缀的定义，你不可能找到一个值 i > next[k] 使得 t[0]~t[i-1] = s[j-i-1]~s[j]， 至于k值可以更小吗?可以，但你主串指针也要回溯
     */
    if (len1 < len2) {
        return 0;
    }
    int next[len2 + 1]; //计算子串的next数组
    calculateNext(str2, next, len2);
    int nextMax = next[len2];
    calculateNextValue(str2, next, len2); //优化后的next
    int k = 0, num = 0, index = 0; //k为当前指向

    for (int i = 0; i < len1; ++i) {
        if (len1 - i + k < len2) { //剩余字符不够凑成子串
            break;
        }
        while (k > 0 && str2[k] != str1[i]) { //ptr和str不匹配，且k>0（表示ptr和str有部分匹配）
            k = next[k];

            // “abad..”-->abab： 既避免了不相等元素的判断，也避免了相等元素的判断
            /*      abad     abad...
             *      abab       abab
             *         k        k
             */
        }
        if (str2[k] == str1[i]) {
            k++; //指向下一个
        }
        if (k >= len2) { //成功找到
            result[num++] = i - len2 + 1;
            if (num == maxNum) //够了
                break;
            k = nextMax; //重新开始匹配
            //运用一样的处理逻辑，其实可以看成 t[k--] != str[i],再找一个
            // “aaaa” -->"aa", 当匹配成功时，返回len-2个字符，万一又匹配了
        }
    }
    return num;
}

void calculateNextValue(const char *str, int *next, int len) {
    /*
     * 当kmp失败(s[j+1]!=t[k])的时候，会使k=next[k],然后对比s[j+1]和t[k],如果又失败，继续。。
     * 但我们仔细想想，如果 t[next[k]] = t[k], 何不干脆使next[k] = next[next[k]],这样少那么多跳转
     * 所以我们可以对之前的next数组改造 :
     *  if(str[j] == str[next[j]], 那么使 next[j]=next[next[j]]; 否则保持原样
     */

    //next[0] = -1,next[1]=0
    for (int i = 2; i <= len; ++i) {
        if (str[i] == str[next[i]]) {
            next[i] = next[next[i]] > 0 ? next[next[i]] : 0;
        }
    }
}
