#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "gobang.h"

void FirstTalk(int sock)
{
    Coordinate coor;
    while (1)
    {
        printf("请输入要下的坐标~ 如：x y\n");
        scanf("%d%d", &(coor.x), &(coor.y));

        coor.x = htonl(coor.x);
        coor.y = htonl(coor.y);
        write(sock, &coor, sizeof(coor));

        printf("对方下子中......\n");
        ssize_t rd = read(sock, &coor, sizeof(coor));
        if (rd < 0)
        {
            perror("read");
            return;
        }

        if (rd == 0)
        {
            printf("read done!");
            return;
        }
        coor.x = ntohl(coor.x);
        coor.y = ntohl(coor.y);
        printf("server: x = %d, y = %d", coor.x, coor.y);
    }
}

void SecondTalk(int sock)
{
    Coordinate coor;
    while (1)
    {
        printf("对方下子中......\n");
        ssize_t rd = read(sock, &coor, sizeof(coor));
        if (rd < 0)
        {
            perror("read");
            return;
        }

        if (rd == 0)
        {
            printf("read done!");
            return;
        }
        coor.x = ntohl(coor.x);
        coor.y = ntohl(coor.y);
        printf("对方: x = %d, y = %d", coor.x, coor.y);

        printf("请输入要下的坐标~ 如：x y\n");
        scanf("%d%d", &(coor.x), &(coor.y));

        coor.x = htonl(coor.x);
        coor.y = htonl(coor.y);
        write(sock, &coor, sizeof(coor));

    }
}
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("usage: ./client [ip] [port]");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return 2;
    }


    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));
    int n = connect(sock, (struct sockaddr*)&server, sizeof(server));
    if(n < 0)
    {
        perror("connect");
        return 3;
    }

    int i = 0;
    read(sock, &i, sizeof(int));
    printf("%d", i);
    if (i == 0)
    {
        FirstTalk(sock);
    }
    else if (i == 1)
    {
        SecondTalk(sock);
    }
    else
    {
        perror("先后顺序错误");
        return -1;
    }
    return 0;
}

