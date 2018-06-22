#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

void Response(int new_sock, struct sockaddr_in * client)
{
    while (1)
    {
        char buf[512] = {0};
        // 接受请求
        ssize_t rd = read(new_sock, buf, sizeof(buf)-1);
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
        // 处理请求
        printf("client say [%s:%d] # %s\n",inet_ntoa(client->sin_addr), ntohs(client->sin_port), buf);

        // response 响应
        write(new_sock, buf, strlen(buf));
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("usage:./server_tcp [ip] [port]");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("sock error\n");
        return 2;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));
    int b = bind(sock, (struct sockaddr*)&server, sizeof(server));
    if (b < 0)
    {
        perror("bind error\n");
        return 3;
    }
    
    int l = listen(sock, 3); // 最大的连接数量
    if (l < 0)
    {
        perror("listen error\n");
        return 4;
    }

    printf("bind and listen .... \n");

    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int new_sock = accept(sock, (struct sockaddr*)&client, &len);
    Response(new_sock, &client);
    return 0;
}

