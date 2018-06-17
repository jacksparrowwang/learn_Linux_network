#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

// 互斥量
pthread_mutex_t mutex;

// 条件
pthread_cond_t cond;

// 用与记录次数
int count = 0;

// 用一个链表来当作仓库
typedef struct ListNode
{
    struct ListNode* next;
    int data;
}ListNode;

// 带头节点
ListNode head;

ListNode* CreateNode(int value)
{
    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    new_node->next = NULL;
    new_node->data = value;
    return new_node;
}

void Init(ListNode* head)
{
    head->data = 0;
    head->next = NULL;
}

void Push(ListNode* head, int value)
{
    if (head == NULL)
    {
        return;
    }
    ListNode* node = CreateNode(value);
    ListNode* nex = head->next;
    head->next = node;
    node->next = nex;
}

void Pop(ListNode* head, int *top)
{
    if (head == NULL)
    {
        return;
    }
    if (head->next == NULL)
    {
        return;
    }
    *top = head->next->data;
    ListNode* node = head->next;
    head->next = node->next;
    free(node);
}

void* Producer(void* arg)
{
    (void)arg;
    while (1)
    {
        // 加锁
        pthread_mutex_lock(&mutex);
        Push(&head, count);
        printf("Producer %d \n", head.next->data);
        ++count;
        // 产生一个数据，就要唤醒等待的线程
        pthread_cond_signal(&cond);
        // 解锁
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* Consumer(void* arg)
{
    (void)arg;
    while (1)
    {
        int value = -1;
        pthread_mutex_lock(&mutex);

        // 这里用while是因为有可能被信号打断，
        // 当再次返回时候，就可以重新判断
        // 也有可能被其它异常信号唤醒
        while (head.next == NULL)
        {
            // 没有数据，就要进行
            // 解锁
            // 等待
            // 加锁
            pthread_cond_wait(&cond, &mutex);
        }
        Pop(&head, &value);
        printf("consumer %d\n",value);

        pthread_mutex_unlock(&mutex);
        usleep(100000);
    }
    return NULL;
}

int main()
{
    Init(&head);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t producer, consumer;
    pthread_create(&producer, NULL, Producer, NULL);
    pthread_create(&consumer, NULL, Consumer, NULL);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    return 0;
}
