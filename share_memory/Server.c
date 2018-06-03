#include <stdio.h>
#include<unistd.h>

#include "common.h"

int main()
{
    // 创建出共享内存空间，大小为1024
    int shmid = CreateShm(1024);
    char* addr = (char*)shmat(shmid, NULL, 0);
    int i = 10;
    while (i--)
    {
        printf("%s\n", addr);
        sleep(3);
    }
    shmdt(addr);
    // 销毁
    DestroyShm(shmid);
    return 0;
}
