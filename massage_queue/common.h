#pragma once

#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>


#define PATH "."
#define ID 1

#define SEND 1
#define RECEIVE 2


struct msgbuf_
{
    long mtype;
    char mtext[1024];
};

// 创建消息队列，存在就打开失败
int common(int flag);

// 创建消息队列 ,但不打开
int CreateQueue();

// 打开消息队列
int OpenQueue();

// 销毁消息队列
void Destory();
