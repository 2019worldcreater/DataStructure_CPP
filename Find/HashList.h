//
// Created by HP on 2020-11-30.
//

#ifndef CLIONCPP_HASHLIST_H
#define CLIONCPP_HASHLIST_H
/*
 * 哈希表存储：  一般用连续的存储空间
 * 即根据一个散列函数计算数据项在存储空间中的下标   index = f(key)，key为关键码
 * 不论是添加还是查找，都通过散列函数寻找下标，这种方式大大提高了查找效率，但也存在着缺点
 * 哈希表不能表示数据间关系，都是独立的；  哈希表不能存储很重复的关键码， 即保存一个班学生信息时，不能用性别作为关键码，应改为学号之类的
 *
 *
 * 1、散列函数的构造：
 *      （1）直接定址，即  f = a * key + b, 即线性存储，适用于已知关键码分布，且哈希表较小且连续， 不常用
 *      (2)数字分析法，即从关键码中取出一部分，对这个部分设计散列函数，即抽取， 适合key位数长，但关键字若干位分布均匀
 *      (3)平方取中：对key取平方数，抽取中间3位数作为地址，适合key位数不大，又不知道分布
 *      (4)折叠法：将key分为几个长度相等的部分(最后一部分位数不够也行),然后将这几个部分相加，根据哈希表长，取和的后几位数作为下标
 *              也可以先将这几部分中，第一部分和倒数第二部分取反(123-->321)
 *              适合key位数大
 *
 *      (5)取模mod法 (很常用) ----  8 = 2 * 3 + 2, 8 mod 3 = 2(余数)
 *          假如哈希表长度为 m
 *          那么 f(key) = key mod P( P <= m)
 *          P的选择也很重要，一般选择接近 m 的质数或不包含小于20质因子的合数，而 P~m-1的下标就浪费了，不可到达
 *
 *      （6）随机数法， f(key) = random(key)
 *
 * 2、解决散列值重复，即地址冲突
 *      (1)开放地址， fi(key)=(f(key)+di)MOD m(di=1,2,3,......,m-1), m为表长     (单向勘测)
 *          即第一次 f(key) = key mod m,  如果此时的 f(key)存在元素，那么就继续计算  f1(key) = (f(key) + 1)mod m
 *          如果还有，f2(key) = (f(key) + 2) mod m, 直到找到空的,或已满
 *          类似的还有 fi(key)=(f(key)+d i )MOD m(d i =1 2 ,-1 2 ,2 2 ,-2 2 ,...,q 2 ,-q 2 ,q≤m/2)  （双向勘测）
 *          f i (key)=(f(key)+d i )MOD m(d i 是一个随机数列) , 随机勘测
 *
 *       (2)再散列，即第一种散列函数不行，就用第二种散列函数，我就不信找不到
 *       fi(key)=RHi(key)(i=1,2,...,k)   RHi代表第i种散列函数
 *
 *      （3）链地址法 ： 哈希表中的每个每个元素是个链表，链表中的元素拥有一样的额hash值
 *      不另外找位置，而是和该位置的元素组成一个链表，加在链表后面
 *
 *      （4）公共溢出区法： 用另外一个顺序表(溢出表)存储那些存在冲突的元素，比如 key=32时，位置被占了，那我就将32加进溢出表中
 *      查找时，首先按散列值查找，如果没有，则取溢出表中顺序查找
 *      对于冲突少的情况很实用
 */
#define MAX_HASH_SIZE 101

#include "../List/LinkList.h"

/*
 * 这里采用 mod 散列，和链表法解决冲突
 */

class HashList {
private:
    LinkList<int> *list[MAX_HASH_SIZE]; //每个元素都是链表，这里需要保存关键码 key
public:
    HashList() {
        for (int i = 0; i < MAX_HASH_SIZE; ++i) {
            list[i] = nullptr;
        }
    }

    ~HashList() {
        for (int i = 0; i < MAX_HASH_SIZE; ++i) {
            if (list[i])
                list[i]->releaseLinkList();
            delete list[i];
        }
    }

    //散列函数
    int hashCode(int key) {
        return key % MAX_HASH_SIZE; // k mod m
    }

    //添加key，如果有则放弃
    bool add(int key) {
        int index = hashCode(key); //下标
        if (list[index] == nullptr) { //说明该位置不存在元素
            list[index] = new LinkList<int>();
        }
        if (list[index]) {
            if (list[index]->isValid(list[index]->getIndexOfData(key))) //存在key了
                return false;
            list[index]->addNode(key); //加入链表
            return true;
        }
        return false;
    }

    //是否有key
    bool findKey(int key) {
        int index = hashCode(key); //下标
        if (list[index] == nullptr) { //说明该位置不存在元素
            return false;
        } else {
            return list[index]->isValid(list[index]->getIndexOfData(key));
        }
    }
};

#endif //CLIONCPP_HASHLIST_H
