#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "poto.h"

// .server 127.0.0.1 9999
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

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));

    Request req;
    Response resp;
    while (1)
    {
        printf("输入第一个数字\n");
        scanf("%d", &req.a);
        printf("输入第二个数字\n");
        scanf("%d", &req.b);
        req.a = htonl(req.a);
        req.b = htonl(req.b);
        sendto(sock, &req, sizeof(req), 0, (struct sockaddr*)&server, sizeof(server));

        ssize_t rd = recvfrom(sock, &resp, sizeof(resp), 0, NULL, NULL);
        if (rd < 0)
        {
            perror("recvfrom error\n");
            return 3;
        }
        if (rd == 0)
        {
            printf("read done!\n");
            return 0;
        }
        resp.sum = ntohl(resp.sum);
        printf("sum = %d\n", resp.sum);
    }
    return 0;
}
