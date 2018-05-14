#pragma once

#include<stdio.h>
#include<stdlib.h>

typedef char TypeLinkQueue;


typedef struct LinkQueueNode{
	TypeLinkQueue data;
	struct LinkQueueNode *next;
}LinkQueueNode;

typedef struct LinkQueue{
	LinkQueueNode *head;
	LinkQueueNode *tail;
}LinkQueue;


// 队列的初始化
void InitLinkQueue(LinkQueue **q);

// 入队
void PushLinkQueue(LinkQueue **q, TypeLinkQueue value);

// 取队首元素，成功返回1，失败返回0，top为输出型参数
int TopFindLinkQueue(LinkQueue *q, TypeLinkQueue *top);

// 出队
void PopLinkQueue(LinkQueue **q);

// 判断队列是否为空
int EmptyLinkQueue(LinkQueue *q);

// 队列的长度
size_t SizeLinkQueue(LinkQueue *q);

// 销毁队列
void DestoryLinkQueue(LinkQueue **q);
