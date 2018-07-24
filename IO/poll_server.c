#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/socket.h>

#define SIZE 1024

// 设置就绪好的读事件
// 每个struct pollfd 结构体中，都有fd 监控的文件描述符 events 输入的监听事件
// revents 输出的就绪好的实践
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

// 创建socket
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

// 初始化pollfd结构体的数组
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

    // 构建出fdlist结构体
    struct pollfd fdlist[SIZE];
    // 初始化
    InitPollfd(fdlist, SIZE);
    // 添加事件
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
        // 循环的去遍历就绪事件
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
                // 添加连接上的文件描述符
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
                // 写回客户端
                write(fdlist[i].fd, buf, strlen(buf));
            }
        }
    }
    return 0;
}
