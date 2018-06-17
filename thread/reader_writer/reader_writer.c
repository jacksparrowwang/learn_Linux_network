#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define num 5

pthread_rwlock_t lock;

int count = 0;

void *Reader(void* arg)
{
    long i = (long)arg;
    while (1)
    {
        pthread_rwlock_rdlock(&lock);
        printf("Reader %lu count %d\n", i, count);
        pthread_rwlock_unlock(&lock);
        usleep(500000);
    }
    return NULL;
}

void *Writer(void* arg)
{
    long i = (long)arg;
    while (1)
    {
        pthread_rwlock_wrlock(&lock);
        ++count;
        printf("Writer %lu count %d\n", i, count);
        pthread_rwlock_unlock(&lock);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_rwlock_init(&lock, NULL);
    pthread_t th[num];
    long i = 0;
    for (; i < num-1; ++i)
    {
        pthread_create(&th[i], NULL, Reader, (void*)i);
    }
    pthread_create(&th[num], NULL, Writer, (void*)1);

    i = 0;
    for (; i < num; ++i)
    {
        pthread_join(th[i], NULL);
    }
    pthread_rwlock_destroy(&lock);
    return 0;
}
