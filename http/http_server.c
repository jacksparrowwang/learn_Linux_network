#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <signal.h>

#define PATH "index.html"

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
    
    // 用来进行把文件描述符为 TIME_WAIT 状态抢占
    int opt = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

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

    signal(SIGPIPE, SIG_IGN);
    while (1)
    {
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

        int fd = open(PATH, O_RDONLY);
        if (fd < 0)
        {
            /* return ; */
        }

        struct stat st;
        // 注意，这里是文件*****
        int s = stat("./index.html", &st);
        if (s < 0)
        {
            perror("stat");
            /* return; */
        }
        
        char header[1024] = "Content-Type:text/html;charset=utf-8";
        
        /* char body[1024] = "<h1>Hello 你 好 World<h1>"; */
        /* sprintf(buf, "HTTP/1.1 200 OK\n%s\nConten-Length: %lu\n\n%s", header,strlen(body),body); */
        sprintf(buf, "http/1.1 200 ok\r\n%s\r\n\r\n", header);

        write(new_sock, buf, strlen(buf));

        int se = sendfile(new_sock, fd, NULL, st.st_size);
        printf("%s\n",buf);
        if (se < 0)
        {
            perror("sendfile");
        }

        close(fd);
        close(new_sock);

    }
    
    return 0;
}

