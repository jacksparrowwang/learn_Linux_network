#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("usage: ./http [ip] [port]\n");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket error\n");
        return 2;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));
    int b = bind(sock, (struct sockaddr*)&server, sizeof(server));
    if (b < 0)
    {
        perror("bind");
        return 5;
    }

    int lis = listen(sock, 5);
    if (lis < 0)
    {
        perror("listen error\n");
        return 3;
    }

    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int new_sock = accept(sock, (struct sockaddr*)&server, &len);
    if (new_sock < 0)
    {
        perror("accept error\n");
        return 4;
    }

    char buf[1024*10] = {0};
    ssize_t rd = read(new_sock, buf, sizeof(buf)-1);
    if (rd < 0)
    {
        perror("read error\n");
        return 5;
    }
    buf[rd] = '\0';
    printf("response %s\n", buf);

    char header[1024] = "Content-Type: text/html";
    char body[1024] = "<h1>Hello World<h1>";
    sprintf(buf, "HTTP/1.1 200 OK\n%s\nConten-Length: %lu\n\n%s", header,strlen(body),body);

    write(new_sock, buf, strlen(buf));
    
    return 0;
}

