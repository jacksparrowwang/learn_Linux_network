
/*
 * posix线程的创建
 * 线程的等待
 * 线程的退出
 * 线程的分离
 * */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void *pthread_fun(void *arg)
{
    (void)arg;
    int i = 10;
    while (i--)
    {
        sleep(1);
        printf("i am thread\n");
        // 如果要退出需要返回一个值，这个值一定不能在栈上
    //    pthread_exit(NULL);
    }
    pthread_t th = pthread_self();
    printf("%lu\n", th);
    
    // 如果要退出需要返回一个值，这个值一定不能在栈上
    return NULL;
}

int main()
{
    pthread_t thr;
    // 线程创建成功返回0，失败返回错误码
    // 传入thr, 线程属性，线程函数返回值为void*，线程函数参数为void*
    int ret = pthread_create(&thr, NULL, pthread_fun, NULL);
    if (ret != 0)
    {
        printf("error %d\n", ret);
        exit(1);
    }
    int i = 5;
    while (i--)
    {
        sleep(1);
        printf("i am main thread\n");
    }
    // 线程的等待，第二参数为输出型参数，输入线程的返回值
    // 有返回值，成功返回0，失败返回错误码
    // 为阻塞式等待
    /* void* value; */
    // 这里可以接收pthread_exit(void* value)中的value,如果是空就必须在join中为NULL
    pthread_join(thr, NULL); //输入型参数类型为void**
    /* printf("join value %d\n", *(int*)value); // 这里要先强转为int*在解引用 */
    return 0;
}


