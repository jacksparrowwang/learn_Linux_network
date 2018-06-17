/*
 * 用循环队列来当作仓库
 * 用信号两来表示仓库中的数量和仓库中空闲的数量
 * 采用互斥锁来处理线程对资源的竞争
 * 
 * 注意线程之间的访问要写在全局区，或者malloc出来的，不能在栈上
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define buf 20

// 互斥锁
pthread_mutex_t mutex;

// 信号量
sem_t full;
sem_t empty;

int count = 0;

// 用数组来构建一个循环队列

typedef struct Queue
{
    int arr[buf];// 用来当仓库
    int size;// 仓库的大小
    int front; // 头插时候前面的下标
    int tail; // 后面来进行删除
}Queue;

Queue q;

void Init(Queue* q)
{
    q->size = 0;
    q->front = 0;
    q->tail = 0;
}

void Push(Queue* q, int value)
{
    if (q == NULL)
    {
        return;
    }
    if (q->size >= buf)
    {
        return;
    }
    if (q->front == buf)
    {
        q->front = 0;
    }
    q->arr[q->front] = value;
    ++q->front;
    ++q->size;
}

void Pop(Queue* q, int* top)
{
    if (q == NULL)
    {
        return;
    }
    if (q->size == 0)
    {
        return;
    }
    if (q->tail == buf)
    {
        q->tail = 0;
    }
    *top = q->arr[q->tail];
    ++q->tail;
    --q->size;
}

void* Produce(void* arg)
{
    (void)arg;
    while (1)
    {
        sem_wait(&empty);// 空的减少1,p操作

        // 在信号量中，必须先判断有无资源，后再加锁操作。
        // 在cond中因为wait有解锁等待再加锁的操作，所以就不会死锁
        pthread_mutex_lock(&mutex);
        Push(&q, count);
        printf("Produce %d\n", count);
        ++count;
        pthread_mutex_unlock(&mutex);

        sem_post(&full); // 有数据了，v操作
        usleep(10000);
    }
    return NULL;
}

void* Consume(void* arg)
{
    (void)arg;
    while (1)
    {
        int value = -1;
        sem_wait(&full);// 减少有数据，先要判断是都有资源，才能加锁
        pthread_mutex_lock(&mutex);

        Pop(&q, &value);
        printf("Consume %d\n", value);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t th1, th2;
    Init(&q);

    sem_init(&full, 0, 0);
    sem_init(&empty, 0, buf);

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&th1, NULL, Produce, NULL);
    pthread_create(&th2, NULL, Consume, NULL);
    
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    sem_destroy(&full);
    sem_destroy(&empty);

    pthread_mutex_destroy(&mutex);
    return 0;
}
