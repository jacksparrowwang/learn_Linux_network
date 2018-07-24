#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

int SetNoBlock(int fd)
{
    int fc = fcntl(fd, F_GETFL);
    if (fc < 0)
    {
        perror("fcntl get");
        return -1;
    }
    // 设置为非阻塞
    int ret = fcntl(fd, F_SETFL, fc | O_NONBLOCK);
    if (ret < 0)
    {
        perror("fcntl set");
        return -1;
    }
    return ret;
}

int InitSock(const char* ip, short port)
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
    int ret = bind(sock, (struct sockaddr*)&server, sizeof(server));
    if (ret < 0)
    {
        perror("bind error");
        return -1;
    }

    ret = listen(sock, 5);
    if (ret < 0)
    {
        perror("listen error");
        return -1;
    }

    return sock;
}

// 建立连接处理
void ProcessConnect(int epoll_fd, int listen_sock)
{
    // 这里是处理 一次多个客户端连接上的处理
    while (1)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int new_sock = accept(listen_sock, (struct sockaddr*)&client, &len);
        // 触发非阻塞读，发现没有数据
        if (new_sock < 0 || errno == EAGAIN)
        {
            perror("accept error");
            return;
        }

        // 设置非阻塞
        SetNoBlock(new_sock); // 改为ET模式

        struct epoll_event ev;
        ev.data.fd = new_sock;
        // 用ET模式，水平触发
        ev.events = EPOLLIN | EPOLLET; // 注意
        int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_sock, &ev);
        if (ret < 0)
        {
            perror("epoll_ctl error");
            return;
        }
    }
}
//
ssize_t myread(int new_sock, char* buf, size_t size)
{
    size_t total = 0;
    for (;;)
    {
        // 这里读是为了一次最多可以读1k的数据
        size_t buf_size = size - total < 1024 ? size-total : 1024;
        ssize_t rd = read(new_sock, buf+total, buf_size);
        if (rd < 0 && errno == EAGAIN)
        {
            // 此时说明非阻塞读取已经结束
            break;
        }
        if (rd == 0)
        {
            break;
        }
        total += rd;
        if (total >= size)
        {
            break;
        }
    }
    return total;
}
//
// 请求处理
void ProcessRequest(int epoll_fd, int fd)
{
    char buf[512] = {0};
    ssize_t rd = myread(fd, buf, sizeof(buf)-1);
    if (rd < 0)
    {
        perror("read error");
        return;
    }
    if (rd == 0)
    {
        printf("read done!\n");
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
        return;
    }
    printf("%s \n", buf);
    write(fd, buf, strlen(buf));
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        perror("Usage : ./server ip port");
        return 1;
    }

    int listen_sock = InitSock(argv[1], atoi(argv[2]));
    if (listen_sock < 0)
    {
        perror("InitSock error");
        return 2;
    }

    // 创建epoll对象
    // 返回的是问价描述符
    int epoll_fd = epoll_create(10);
    if (epoll_fd < 0)
    {
        perror("epoll_create error");
        return 3;
    }

    // 设置为非阻塞
    SetNoBlock(listen_sock); // 改为非阻塞

    // 设置需要监听的事件
    struct epoll_event event;
    // 用ET模式，水平触发
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = listen_sock;

    // 进行调用epoll_ctl 进行设置
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &event);
    if (ret < 0)
    {
        perror("epoll_ctl error\n");
        return 4;
    }

    for (;;)
    {
        // 这是一个用来输出型参数
        struct epoll_event events[10];
        // 如果同时并发很高，活跃的也很高，就会效率也会下降
        int size = epoll_wait(epoll_fd, events, 10, -1);// 最后一个参数-1 表示阻塞等待
        if (size < 0)
        {
            perror("epoll_wait error");
            continue;
        }
        if (size == 0)
        {
            printf("time out..\n");
            continue;
        }
        // 返回大于0，为数组中有效的文件描述符个数，为前面的几个
        // 如100个就绪10个就会在下标0~9中返回
        int i = 0;
        for (; i < size; ++i)
        {
            if (!(events[i].events & EPOLLIN))
            {
                continue;
            }
            if (events[i].data.fd == listen_sock)
            {
                // 进行accept
                ProcessConnect(epoll_fd, listen_sock);
            }
            else
            {
                // 请求处理
                ProcessRequest(epoll_fd, events[i].data.fd);
            }
        }
    }

    return 0;
}
