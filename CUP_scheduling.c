#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum Status
{
    R,
    E,
    X
}Status;

typedef struct task_struct
{
    const char* pid;
    struct task_struct* next;
    int work_time;
    int pre_conunt;
    Status status;
}PcbNode;

typedef struct List
{
    PcbNode* head;
}List;

PcbNode* Create(const char pid[], int work_time, int pre_conunt, Status status)
{
    PcbNode* new_node = (PcbNode*)malloc(sizeof(PcbNode));
    if (new_node == NULL)
    {
        perror("Create Node error\n");
        return NULL;
    }
    new_node->pid = pid;
    new_node->next = NULL;
    new_node->work_time = work_time;
    new_node->pre_conunt = pre_conunt;
    new_node->status = status;
    return new_node;
}

void Init(List* h)
{
    if (h == NULL)
    {
        return;
    }
    h->head = Create("p0", 0, 0, X);
    return;
}

void PushFront(List* h, const char pid[], int work_time, int pre_conunt, Status status)
{
    if (h == NULL)
    {
        return;
    }
    PcbNode* new_node = Create(pid, work_time, pre_conunt, status);
    new_node->next = h->head->next;
    h->head->next = new_node;
    /* PcbNode* cur = h->head; */
    /* while (cur->next != NULL) */
    /* { */
    /*     cur = cur->next; */
    /* } */
    /* cur->next = new_node; */
}

PcbNode* PopFront(List* h, PcbNode* cur)
{
    if (h == NULL)
    {
        return NULL;
    }
    if (h->head->next == NULL)
    {
        // 队列为NULL
        return NULL;
    }
    PcbNode* to_delete = cur->next;
    cur->next = to_delete->next;
    free(to_delete);
    return h->head;
}

void Print(List* h)
{
    printf("\n==================\n");
    if (h == NULL)
    {
        return;
    }
    PcbNode* cur = h->head->next;
    while (cur != NULL)
    {
        printf("[进程pid：%s][工作时间：%d] [优先数：%d] [状态：%d]\n",cur->pid, cur->work_time, cur->pre_conunt, cur->status);
        cur = cur->next;
    }
    printf("\n==================\n");
}

int main()
{
    List h;
    Init(&h);
    // 采用倒序头插
    PushFront(&h, "p1", 2, 1, R);
    PushFront(&h, "p2", 4, 2, R);
    PushFront(&h, "p3", 1, 3, R);
    PushFront(&h, "p4", 2, 4, R);
    PushFront(&h, "p5", 3, 5, R);
    /* Print(&h); */
    while (1)
    {
        PcbNode* cur = h.head->next;
        PcbNode* pre_max = h.head->next;
        while (pre_max != NULL)
        {
            if (cur->pre_conunt <  pre_max->pre_conunt)
            {
                cur = pre_max;
            }
            pre_max = pre_max->next;
        }
        printf("[进程pid：%s][工作时间：%d] [优先数：%d] [状态：%d]\n",cur->pid, cur->work_time, cur->pre_conunt, cur->status);
        --cur->pre_conunt; 
        --cur->work_time;
        if (cur->work_time ==  0)
        {
            cur->status = E;
            PcbNode* to_find_pre = h.head;
            while (to_find_pre->next != NULL)
            {
                if (to_find_pre->next == cur)
                {
                    break;
                }
                to_find_pre = to_find_pre->next;
            }
            PcbNode* ret = PopFront(&h, to_find_pre);
            if (ret == NULL)
            {
                break;
            }
        }
        if (cur->pre_conunt <= 0)
        {
            cur->pre_conunt = 0;
        }
        Print(&h);
        sleep(5);
        if (h.head->next == NULL)
        {
            break;
        }
        /* sleep(3); */
    }
    return 0;
}

