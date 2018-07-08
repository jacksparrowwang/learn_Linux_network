#pragma once

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

typedef struct Coordinate
{
    int x;
    int y;
}Coordinate;


// 建立连接
int InitSocket(char *ip, int port);

// 线程函数入口1
void *Fighting1(void* arg);

// 线程函数入口2
void *Fighting2(void* arg);

// 创建线程
void CreateWorker(int sock);
