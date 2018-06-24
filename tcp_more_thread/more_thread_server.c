#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/wait.h>

typedef struct Arg
{
   int sock;
   struct sockaddr_in client;
}Arg;

void ProcessRequest(int sock, struct sockaddr_in client)
{
    char buf[512] = {0};
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
            perror("read done!\n");
            return;
        }
        buf[rd] = '\0';

        printf("client %s:%d say # %s\n",inet_ntoa(client.sin_addr), ntohs(client.sin_port),  buf);

        write(sock, buf, strlen(buf));
    }
}

void* CreateWorker(void* arg)
{
    Arg* ptr = (Arg*)arg;
    ProcessRequest(ptr->sock, ptr->client);
    return NULL;
}

// ./server 0 9999
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("usage:./server [ip] [port]");
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
    if (b < 0 )
    {
        perror("bind error\n");
        return 3;
    }

    int l = listen(sock, 10);
    if (l < 0)
    {
        perror("listen error\n");
        return 5;
    }

    while(1)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int new_sock = accept(sock, (struct sockaddr*)&client, &len);
        if (new_sock < 0)
        {
            perror("accept error\n");
            return 6;
        }
        
        Arg arg;
        arg.sock = new_sock;
        arg.client = client;
        pthread_t thread;
        pthread_create(&thread, NULL, CreateWorker, (void*)&arg);
        pthread_detach(thread);
    }
    return 0;
}


