#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// 条件变量
pthread_cond_t cond;

// 互斥量
pthread_mutex_t mutex;

void* Fun1(void* arg)
{
    (void)arg;
    while (1)
    {
        // 这里要田间互斥量进行同步
        pthread_cond_wait(&cond, &mutex);
        printf("收到\n");
    }

    return NULL;
}

void* Fun2(void* arg)
{
    (void)arg;
    while (1)
    {
        pthread_cond_signal(&cond);
        printf("通知\n");
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t th1, th2;
    pthread_create(&th1, NULL, Fun1, NULL);
    pthread_create(&th2, NULL, Fun2, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    return 0;
}
