#include <stdio.h>
#include <unistd.h>
#include "common.h"



int main()
{
    CreateQueue();
    int msg = OpenQueue();
    if (msg == -1)
    {
        perror("CreateQueue\n");
        return -1;
    }
    char buf[1024];
    while (1)
    {
        Receive(msg, SEND, buf);
        printf("server:%s ", buf);
        Send(msg, RECEIVE, buf);
        printf("server:%s ", buf);
    }
    Destory();
    return 0;
}
