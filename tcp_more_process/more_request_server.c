#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void PrcessRequest(int sock, struct sockaddr_in* client)
{
    char buf[521] = {0};
    while (1)
    {
        ssize_t rd = read(sock, buf, sizeof(buf)-1);
        if (rd < 0)
        {
            perror("read error\n");
            return;
        }
        if (rd == 0)
        {
            printf("read done!\n");
            return;
        }
        buf[rd] = '\0';

        printf("client %s:%d say# %s\n",inet_ntoa(client->sin_addr), ntohs(client->sin_port), buf);

        write(sock, buf, strlen(buf));
    }
}

void CreatWorker(int sock, struct sockaddr_in* client)
{
    int pid = fork();
    if (pid < 0)
    {
        perror("fork error\n");
        return;
    }
    if (pid == 0)
    {
        // 子进程
        if (fork() == 0)
        {
            // 用孙子进程，为了保证后面的其它进程继续能执行
            PrcessRequest(sock, client);
        }
        exit(0);
    }
    else
    {
        // 父进程
        close(sock);
        waitpid(pid, NULL, 0); // 等待是为了让子进程退出，孙子进程为孤儿进程。
    }
}

// ./server ip port
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("usage:./server [ip] [port]");
        return 1;
    }

    // 打开网卡
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket error\n");
        return 2;
    }

    // 绑定端口号
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));
    int b = bind(sock, (struct sockaddr*)&server, sizeof(server));
    if(b < 0)
    {
        perror("bind error\n");
        return 3;
    }

    // 监听需要建立连接的请求，来进行排队
    int l = listen(sock, 10);// 等待队列的最大数量
    if (l < 0)
    {
        perror("listen error\n");
        return 4;
    }

    while (1)
    {
        // 建立连接
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int new_sock = accept(sock, (struct sockaddr*)&client, &len);
        if (new_sock < 0)
        {
            perror("accept error\n");
            continue;
        }

        // 多进程来进行处理请求
        CreatWorker(new_sock, &client);
    }
    return 0;
}
