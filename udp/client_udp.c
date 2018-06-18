#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char*argv[])
{
    if (argc != 3)
    {
        perror("usage: [][][]\n");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket error\n");
        return 2;
    }

    char buf[512];
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));
    socklen_t len = sizeof(server);
    while (1)
    {
        buf[0] = '\0';
        printf("please say#:\n");
        ssize_t r = read(0, buf, sizeof(buf)-1);
        if (r > 0)
        {
            buf[r-1] = '\0';
            // 发送完成
            sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&server, len);
            // 接收,这里接受后，自己定义一个sock_addr结构体，来记录服务器的IP和port
            ssize_t ret = recvfrom(sock, buf, sizeof(buf)-1, 0, NULL, NULL); // 这里的NULL可以换成接受对方的IP和port
            if (ret > 0)
            {
                buf[ret] = '\0';
                printf("server say# %s\n", buf);
            }
        }
    }
    close(sock);
    return 0;
}
