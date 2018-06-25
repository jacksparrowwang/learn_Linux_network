#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "poto.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("usage: ./server [ip] [port]\n");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
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
    if (b < 0)
    {
        perror("bind error\n");
        return 3;
    }

    // recvfrom
    Request req; // 自定义的协议来进行对请求和响应进行处理
    struct sockaddr_in client;
    socklen_t len = sizeof(client);

////////////////////////////////
// 自定义的协议根据实际情况来进行实际的接受的方式与大小，要注意！！！！
////////////////////////////////

    // sendto
    Response resp;
    
    while (1)
    {
        // 一般收的函数为输出型参数
        ssize_t recv = recvfrom(sock, &req, sizeof(req), 0, (struct sockaddr*)&client, &len);
        if (recv < 0)
        {
            perror("recvfrom error\n");
            continue;
        }
        if (recv == 0)
        {
            printf("read done!\n");
            return 0;
        }
        // 把网络序转成主机序列
        req.a = ntohl(req.a);
        req.b = ntohl(req.b);

        printf("client %s:%d say # a = %d, b = %d\n", \
               inet_ntoa(client.sin_addr), ntohs(client.sin_port), req.a, req.b);

        // 进行计算
        resp.sum = req.a + req.b;

        printf("server sun = %d\n", resp.sum);
        // 主机序转换成网路序，注意是long
        resp.sum = htonl(resp.sum);

        // 拿到了IP和端口号回发
        sendto(sock, &resp, sizeof(resp), 0, (struct sockaddr*)&client, len);
    }

    return 0;
}
