#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/socket.h>

#define SIZE 1024

void FdAdd(int sock, struct pollfd* fdlist, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (fdlist[i].fd == -1)
        {
            fdlist[i].fd = sock;
            fdlist[i].events = POLLIN;
            break;
        }
    }
}

int InitSock(const char *ip, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket error");
        return -1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);

    int b = bind(sock, (struct sockaddr*)&server, sizeof(server));
    if (b < 0)
    {
        perror("bind error");
        return -1;
    }

    int l = listen(sock, 5);
    if (l < 0)
    {
        perror("listen error");
        return -1;
    }
    printf("list_sock start\n");
    return sock;
}

void InitPollfd(struct pollfd* fdlist, int size)
{
    for (int i = 0; i < size; ++i)
    {
        fdlist[i].fd = -1;
        fdlist[i].events = 0;
        fdlist[i].revents = 0;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("Usag:./server_poll ip port");
        return 1;
    }

    int list_sock = InitSock(argv[1], atoi(argv[2]));
    if (list_sock < 0)
    {
        perror("InitSock error");
        return 2;
    }

    struct pollfd fdlist[SIZE];
    InitPollfd(fdlist, SIZE);
    FdAdd(list_sock, fdlist, SIZE);
    for (;;)
    {
        int ret = poll(fdlist, SIZE, 10000);
        if (ret < 0)
        {
            perror("poll error");
            continue;
        }

        if (ret == 0)
        {
            printf("timeout ...");
            continue;
        }
        for(size_t i = 0; i < SIZE; ++i)
        {
            if (fdlist[i].fd == -1)
            {
                continue;
            }
            if (fdlist[i].revents == 0)
            {
                continue;
            }
            if (fdlist[i].fd == list_sock)
            {
                // 有文件描述符来了
                struct sockaddr_in client;
                socklen_t len = sizeof(client);
                int connec_sock = accept(list_sock, (struct sockaddr*)&client, &len);
                if (connec_sock < 0)
                {
                    perror("accept error");
                    continue;
                }
                FdAdd(connec_sock, fdlist, SIZE);
            }
            else
            {
                char buf[1024] = {0};
                ssize_t readsize = read(fdlist[i].fd,  buf, sizeof(buf)-1);
                if (readsize < 0)
                {
                    perror("read error");
                    continue;
                }
                if  (readsize == 0)
                {
                    close(fdlist[i].fd);
                    fdlist[i].fd = -1;
                    fdlist[i].events = 0;
                }
                printf("client :%s\n", buf);
                write(fdlist[i].fd, buf, strlen(buf));
            }
        }
    }
    return 0;
}
