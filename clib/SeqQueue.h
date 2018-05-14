#pragma once
/*
 * 队列一般用链表，因为插入删除效率高
 * 本顺序队列采用静态循环队列，没有实现扩容，如果大数据处理用链表
 * */

#include<stdio.h>
#define MAXREPOSITORY 1000

#define TEAM_HEAD printf("\n===========%s==========\n",__FUNCTION__)

#if 0
typedef char QueueType;
#else
	struct TreeNode;
	typedef struct TreeNode* QueueType;
#endif
typedef struct SeqQueue{
	size_t tail;
	size_t count;
	size_t size;
	QueueType data[MAXREPOSITORY];
}SeqQueue;

// 初始化。
void InitSeqQueue(SeqQueue *seq);

// 入队。
void PushQueue(SeqQueue *seq,QueueType value);

// 出队。
void PopQueue(SeqQueue *seq);

// 取队首元素。
int FindHead(SeqQueue *seq, QueueType *value);

// 销毁。
void DestoryQueue(SeqQueue *seq);
