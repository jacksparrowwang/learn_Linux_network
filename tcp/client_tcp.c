#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("usage:./client_tcp [ip] [port]");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("scoket error\n");
        return 2;
    }
    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));
    socklen_t len = sizeof(server);
    int co = connect(sock, (struct sockaddr*)&server, len);
    if (co < 0)
    {
        perror("connect error\n");
        return 3;
    }
    
    while (1)
    {
        char buf[512] = {0};
        ssize_t rd = read(0, buf, sizeof(buf)-1);
        if (rd < 0)
        {
            perror("read error\n");
            return 4;
        }
        if (rd == 0)
        {
            printf("read done!\n");
            return 5;
        }
        buf[rd-1] = '\0';

        write(sock, buf, strlen(buf)); // 会阻塞的写与读

        ssize_t sockrd = read(sock, buf, sizeof(buf)-1);
        if (sockrd < 0)
        {
            perror("read error\n");
            return 4;
        }

        if (sockrd == 0)
        {
            printf("read done!\n");
            return 5;
        }

        buf[rd] = '\0';
        printf("server say # %s\n", buf);
    }
    return 0;
}

