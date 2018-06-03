#pragma once

// 这里信号量的个数为1个。

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PATHNAME "."
#define ID 1
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

static int common(int number, int flag);

int CreateSem(int number);

int InitSem(int semid, int number, int initvalue);

int GetSem(int number);

// 这里要注意，在用semop的时候，有定义好的结构体，自己实例化就行
// who是哪个信号量编号
// op 是对信号量的一次pv操作。一般是+1或者-1
int commonPV(int semid, int who, int op);

int p(int semid, int who);

int v(int semid, int who);

int DestroySem(int semid);
