#include <stdio.h>
#include <unistd.h>

#include "common.h"

// 这是一个利用共享内存的，将两个进程的虚拟地址空间挂接到同一块的物理空间上。
int main()
{
    int shmid = GetShm(1024);
    // 这是clent共享内存链接到进程的地址空间。
    char* addr = (char*)shmat(shmid, NULL, 0);
    int i = 0;
    while (i < 10)
    {
        // 这里的输入就直接是在虚拟地址空间上写数据
        addr[i] = 'a';
        i++;
        sleep(3);
    }
    shmdt(addr);
    return 0;
}
