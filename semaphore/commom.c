#include "commom.h"

// 定义成静态在c语言中是为了不让外部文件访问到，只能在本文件中使用
static int common(int number, int flag)
{
    int key = ftok(PATHNAME, ID);
    if (key == -1)
    {
        perror("ftok error\n");
        return -1;
    }
    int semid = semget(key, number, flag);
    if (semid == -1)
    {
        perror("semget error\n");
        return -2;
    }
    return semid;
}

int CreateSem(int number)
{
    return common(number, IPC_CREAT | IPC_EXCL | 0666);
}

// 在初始化的时候，需要用到联合体
int InitSem(int semid, int number, int initvalue)
{
    // 创建一个联合体，然后在初始化val值
    union semun _un;
    _un.val = initvalue;
    int ctl = semctl(semid, number, SETVAL);
    if (ctl == -1)
    {
        perror("semctl error\n");
        return -1;
    }
    // 成功返回0
    return 0;
}

int GetSem(int number)
{
    return common(number, IPC_CREAT);
}

int commonPV(int semid, int who, int op)
{
    // 这里要注意，在用semop的时候，有定义好的结构体，自己实例化就行
    // who是哪个信号量编号
    // op 是对信号量的一次pv操作。一般是+1或者-1
    struct sembuf sbf;
    sbf.sem_num = who;
    sbf.sem_op = op;
    sbf.sem_flg = 0;
    if (semop(semid, &sbf, 1) < 1)
    {
        perror("semop error\n");
        return -1;
    }
    return 0;
}

int p(int semid, int who)
{
    return commonPV(semid, who, -1);
}

int v(int semid, int who)
{
    return commonPV(semid, who, 1);
}

int DestroySem(int semid)
{
    int ret = semctl(semid, 0, IPC_RMID);
    if (ret == -1)
    {
        perror("semctl error\n");
        return -1;
    }
    return 0;
}
