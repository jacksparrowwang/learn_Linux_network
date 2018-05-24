#include "common.h"

int common(int flag)
{
    key_t key = ftok(PATH, ID);
    if (key == -1)
    {
        perror("ftok\n");
        return -1;
    }

    // 消息队列的句柄
    int msg = msgget(key, flag);
    if (msg == -1)
    {
        perror("msgget\n");
        return -1;
    }
    return msg;
}

int CreateQueue()
{
    int msgid = common(IPC_CREAT | IPC_EXCL | 0666);
    return msgid;
}

int OpenQueue()
{
    int msgid = common(IPC_CREAT);
    return msgid;
}

void Send(int msgid, int type, char* msg_news)
{
    struct msgbuf_ msgp;
    strcpy(msgp.mtext, msg_news);
    msgp.mtype = type;
    int send = msgsnd(msgid, &msgp, sizeof(msgp.mtext), 0);
    if (send == -1)
    {
        perror("send fail\n");
        return;
    }
}

void Receive(int msg, int type ,char output[])
{
    struct msgbuf_ msgp;
    ssize_t receive = msgrcv(msg, &msgp, sizeof(msgp.mtext), type, 0);
    if (receive == -1)
    {
        return;
    }
    strcpy(output, msgp.mtext);
}

void Destory()
{
    key_t key = ftok(PATH, ID);
    if (key == -1)
    {
        perror("ftok\n");
        return;
    }
    int ret = msgctl(key, IPC_RMID, NULL);
    if (ret == -1 )
    {
        perror("msgctl\n");
        return;
    }
}

