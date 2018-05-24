#include <stdio.h>
#include <unistd.h>
#include "common.h"



int main()
{
    int msgid = OpenQueue();
    char buf[1024];
    while (1)
    {
        int ret = read(0, buf, sizeof(buf)-1);
        fflush(stdout);
        if (ret == -1)
        {
            perror("read error\n");
            return 1;
        }
        if (ret > 0)
        {
            buf[ret] = '\0';
        }
        Send(msgid,SEND,buf);
        printf("cilent: %s", buf);

        Receive(msgid, RECEIVE, buf);
        printf("cilent:%s", buf);
    }
    return 0;
}
