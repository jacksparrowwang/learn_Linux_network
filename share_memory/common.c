#include "common.h"

int Common(int size, int flag)
{
    int key = ftok(PATHNAME, ID);
    if (key == -1)
    {
        perror("ftok error\n");
        return -1;
    }
    int shm = shmget(key, size, flag);
    if (shm == -1)
    {
        perror("shmget error\n");
        return -1;
    }
    return shm;
}

// 创建的内存大小交给用户
int CreateShm(int size)
{
   return Common(size, IPC_CREAT | IPC_EXCL | 0666);
}

int GetShm(int size)
{
    return Common(size, IPC_CREAT);
}

void DestroyShm(int shmid)
{
    int result = shmctl(shmid, IPC_RMID, NULL);
    if (result == -1)
    {
        perror("shmctl error\n");
        return;
    }
}
