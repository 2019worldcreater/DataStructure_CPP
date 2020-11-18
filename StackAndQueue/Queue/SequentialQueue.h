#ifndef CLIONCPP_SEQUENTIALQUEUE_H
#define CLIONCPP_SEQUENTIALQUEUE_H
#define MAX_QUEUE_SIZE 100

/*
 * 如果是普通的顺序结构队列，那么队头即是[0]，队尾用rear标出，入队O(1),出队O(n)
 * 此时如果出队时，队头后面的元素不需往前移，保持不变，入队时，当数组后面没空间后，就移到数组前面去
 * 这时出队、入队都是O(1), 效率大大提高， 于是需要两个指针指向队头和队尾，但考虑只有一个元素时，队尾队头指向一样的地方
 * 可以修改为 front指向队头，rear指向队尾的下一个元素
 *
 * 初始时front=rear=0,队空， 队满时依然 front=rear,这样的话可能不太好，于是可以做出以下约定：
 * 当还剩下一个元素空位置时就算队列已满，此时 (rear + 1)%MAX_SIZE = front
 * 而队列长度可以计算为 (rear-front+QueueSize)%QueueSize
 */
template<typename T>
class SequentialQueue { //循环队列
private:
    T data[MAX_QUEUE_SIZE];
    int front; //队头
    int rear; //队尾下一个元素
public:
    SequentialQueue() {
        front = rear = 0; //此时的data[front]不是队头，加了元素才有
        //增加元素时只需 data[rear]=item,如果队列为空，恰好 data[front]=item, 也就是队列中迎来第一个元素时front才有意义
    }

    bool getHead(T *data) {
        if (!isEmpty()) {
            *data = this->data[front];
            return true;
        }
        return false;
    }

    bool isEmpty() {
        return front == rear; //两者相等时为空
    }

    //规定还剩一个元素时即为满， 此时rear下一个就是front
    bool isFull() {
        //  [rear front...] 、 [front....,rear]
        return (rear + 1) % MAX_QUEUE_SIZE == front;
    }

    //队列长度
    int getLength() {
        //当rear在front后面时， len = 0 + rear + (MAX_SIZE - front)， 此时len % MAX_SIZE不改变结果
        // rear >= front 时 len = rear - front, (len + MAX_SIZE)%..也不改变结果
        return (rear - front + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    }

    //出队，返回队头元素
    bool queueOut(T *head) {
        if (!isEmpty()) {
            //只有一个元素时，rear = 1,front = 0,删除后 front = rear, 恰好为空
            *head = data[front];
            front = (front + 1) % MAX_QUEUE_SIZE; //指向下一个元素即可，循化队列的好处
            return true;
        }
        return false;
    }

    //入队
    bool queueIn(T tail) {
        if (!isFull()) {
            //队空时，rear = front，此时开始的front才有戏， 很巧妙的设计
            data[rear] = tail;
            rear = (rear + 1) % MAX_QUEUE_SIZE; //可能超界到前面去
            return true;
        }
        return false;
    }

    //清空队列
    void clearQueue() {
        front = rear = 0;
    }
};

#endif //CLIONCPP_SEQUENTIALQUEUE_H
