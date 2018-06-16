#if 0
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define num 3

int ticket = 100;

void* MyFun(void* arg)
{
    int j = (int)arg;
    while (ticket >= 0)
    {
        usleep(1000);
        printf("thread %d ticket %d\n", j, ticket);
        ticket--;
    }
    return NULL;
}

int i = 0;
int main()
{
    pthread_t th[num];
    for (;i < num; ++i)
    {
        int ret = pthread_create(&th[i], NULL, MyFun, (void*)i);
        if (ret != 0)
        {
            perror("pthread_create error\n");
            exit(1);
        }
    }

    i = 0;
    for (; i < num; ++i)
    {
        pthread_join(th[i], NULL);
    }
    return 0;
}

#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define num 3

// 互斥锁
pthread_mutex_t mutxe;

int ticket = 100;

void* MyFun(void* arg)
{
    long j = (long)arg;
    while (1)
    {
        usleep(1000);
        pthread_mutex_lock(&mutxe);
        if (ticket > 0)
        {
            printf("thread %lu ticket %d\n", j, ticket);
            ticket--;
            pthread_mutex_unlock(&mutxe);
        }
        else
        {
            pthread_mutex_unlock(&mutxe);
            break;
        }
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&mutxe, NULL);
    pthread_t th[num];
    long i = 0;
    for (;i < num; ++i)
    {
        int ret = pthread_create(&th[i], NULL, MyFun, (void*)i);
        if (ret != 0)
        {
            perror("pthread_create error\n");
            exit(1);
        }
    }

    i = 0;
    for (; i < num; ++i)
    {
        pthread_join(th[i], NULL);
    }
    pthread_mutex_destroy(&mutxe);
    return 0;
}
