//
// 顺序存储结构相比于单链表存储结构来说，查找最高效
/*malloc 和
new 的区别：
malloc需要明确指定字节大小、返回void*， 用free释放,申请失败返回NULL
new 只需要指定类型和数量、返回相应指针，用delete释放，申请失败抛出异常，new对类实例化时会调用构造，delete析构*/

//还有一些方法没实现：getIndex(item)查找与之相等值的下标
#ifndef CLIONCPP_ARRAYLIST_H
#define CLIONCPP_ARRAYLIST_H

#include <algorithm>

#define MAX_SIZE 100

template<typename T>
class ArrayList {
private:
    T array[MAX_SIZE]; //数据随意
    int length = 0; //拥有元素个数,也可以看成是末尾元素的下标+1

    //改变节点值，尽量不展示细节在公共函数中
    bool setArrayDataByIndex(int index, T data, bool isAdd) {
        if (isValid(index) || (isAdd && index == length)) { //如果是新增的话，会被isValid删掉
            array[index] = data;
            return true;
        }
        return false;
    }

    //返回节点值
    bool getArrayDataByIndex(int index, T *data) {
        if (isValid(index)) {
            *data = array[index];
            return true;
        }
        return false;
    }

public:
    //数组还能装吗
    bool isFull() {
        return length >= MAX_SIZE;
    }

    //下标是否合法，可以直接取的那种
    bool isValid(int index) {
        return index > -1 && index < length;
    }

    //本ArrayList对于非指针型数据都可以完美运行，如果是指针型数据或内含指针，那么clean\changeData方法需要自己重写
    ArrayList() = default;

    //传入数组及其长度
    ArrayList(T *list, int length) {
        std::copy(list, list + length, array); //copy方法用于复制数组
        this->length = length;
    }


    //将下标为index的数据赋值给item,成功则返回true
    bool getDataByIndex(int index, T *item) {
        return getArrayDataByIndex(index, item);
    }

    //添加元素到最后一个
    bool addItem(T item) {
        if (!isFull()) {
            setArrayDataByIndex(length, item, true);
            this->length++;
            return true;
        }
        return false;
    }

    //clean就要就事论事了，如果保存的是一般数值，len = 0就行，如果保存是地址，就要一个个free，如果是结构体中数据
    // 内含指针，又是不同的解法
    void clean() {
        length = 0; //末尾指针重新指向-1
    }

    //插入元素item到下标index处,index可以等于len
    bool insertInto(int index, T item) {
        if (index == length) {
            return addItem(item);
        }
        if (isValid(index)) {
            T preItem;
            for (int i = length; i >= index + 1; --i) {
                getArrayDataByIndex(i - 1, &preItem);
                setArrayDataByIndex(i, preItem, true);
            }
            setArrayDataByIndex(index, item, false);
            this->length++;
            return true;
        }
        return false;
    }

    //更新index下标的值
    bool changeDataByIndex(int index, T data) {
        if (isValid(index)) {
            setArrayDataByIndex(index, data, false);
            return true;
        }
        return false;
    }

//删除下标为index的元素，返回其值给*item
    bool deleteNodeByIndex(int index, T *item) {
        if (isValid(index)) {
            if (getDataByIndex(index, item)) {
                T nextItem;
                for (int i = index; i < length - 1; ++i) {
                    getArrayDataByIndex(i + 1, &nextItem);
                    setArrayDataByIndex(i, nextItem, false);
                }
                this->length--;
                return true;
            }
        }
        return false;
    }

//返回数组中第一个等于data的元素的下标，没有-1
    int getIndexOfData(T data) {
        T item;
        for (int i = 0; i < getLength(); ++i) {
            getArrayDataByIndex(i, &item);
            if (item == data) { //直接比较
                return i;
            }
        }
        return -1;
    }

//元素总数
    int getLength() {
        return length;
    }

//是否为空
    bool isEmpty() {
        return getLength() < 1;
    }

};

#endif //CLIONCPP_ARRAYLIST_H
