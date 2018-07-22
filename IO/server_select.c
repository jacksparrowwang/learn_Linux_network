#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>

typedef struct Setfd
{
    fd_set set;
    int maxfd;
}Setfd;

void InitSetfd(Setfd* setfd)
{
    if (setfd == NULL)
    {
        perror("INitSetfd error");
        return;
    }
    FD_ZERO(&setfd->set);
    setfd->maxfd = 0;
}

// 添加需要监听的文件描述符
void FdAdd(Setfd* setfd, int fd)
{
    FD_SET(fd, &setfd->set);
    if (setfd->maxfd < fd)
    {
        setfd->maxfd = fd;
    }
}

// 删除监听的文件描述位
void FdDel(Setfd* setfd, int fd)
{
    FD_CLR(fd, &setfd->set);
    int i = setfd->maxfd;
    if (fd == setfd->maxfd)
    {
        for (; i >= 0; --i)
        {
            if (FD_ISSET(i, &setfd->set))
            {
                setfd->maxfd = i;
            }
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

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("usage: ./server ip port");
        return 1;
    }

    int list_sock = InitSock(argv[1], atoi(argv[2]));
    if (list_sock < 0)
    {
        perror("InitSock error");
        return 5;
    }

    Setfd readfds;
    // 初始化结构体
    InitSetfd(&readfds);
    // 添加进去了
    FdAdd(&readfds, list_sock);
    for (;;)
    {
        Setfd tmpfds = readfds;
        // 在select中tmpfds是输入和输出参数，输入为要监控的文件描述符集，
        // 输出为就绪的文件描述符位，但是就会把没就绪的给清空。
        // 所以要进行临时保存
        printf("111\n");
        int sel = select(tmpfds.maxfd+1, &tmpfds.set, NULL, NULL, NULL);
        printf("2222\n");
        // 返回失败，因为错误等
        if (sel < 0)
        {
            perror("select error\n");
            continue;
        }
        // 时间超时
        if (sel == 0)
        {
            printf("timeout..");
            continue;
        }
        
        // 返回大于0为返回的是读就绪的文件描述符
        if (FD_ISSET(list_sock, &tmpfds.set))
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int new_sock = accept(list_sock, (struct sockaddr*)&client, &len);
            if (new_sock < 0)
            {
                perror("accept error\n");
                continue;
            }
            printf ("client %d\n", new_sock);
            // 传入结构体,进行设置
            FdAdd(&readfds, new_sock);
        }
        else
        {
            // 在select中，每次都要遍历就绪好的文件描述符。
            int i = 0;
            // 记住要等于,因为是此处是最大文件描述符，包括在内
            for (; i <= tmpfds.maxfd; ++i)
            {
                if (FD_ISSET(i, &tmpfds.set))
                {
                    char buf[1024] = {0};
                    ssize_t rd = read(i, buf, sizeof(buf)-1);
                    if (rd < 0)
                    {
                        perror("read error\n");
                        continue;
                    }

                    // 对端关闭了write
                    if (rd == 0)
                    {
                        perror("read done!");
                        close(i);
                        FdDel(&readfds, i);
                    }
                    // 接收客户端发来的信息
                    printf("client %s\n", buf);

                    // 写会服务器
                    write(i, buf, strlen(buf));
                }
            }
        }
    }

    return 0;
}
