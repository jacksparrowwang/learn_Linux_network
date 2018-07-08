#include "gobang_server.h"

int InitSocket(char *ip, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return -1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = (htons(port));
    int b = bind(sock, (struct sockaddr*)&server, sizeof(server));
    if (b < 0)
    {
        perror("bind");
        return -2;
    }
    return sock;
}

void *Fighting1(void* arg)
{
    int sock1 = ((int*)arg)[0];
    int sock2 = ((int*)arg)[1];
    Coordinate requese;
    while (1)
    {
        ssize_t rd = read(sock1, &requese, sizeof(requese));
        if (rd < 0)
        {
            perror("read");
            return NULL; 
        }

        if (rd == 0)
        {
            perror("read done!");
            close(sock1);
            return NULL;
        }


        write(sock2, &requese, sizeof(requese));
    }
}

void *Fighting2(void* arg)
{
    int sock1 = ((int*)arg)[0];
    int sock2 = ((int*)arg)[1];
    Coordinate requese;
    while (1)
    {
        ssize_t rd = read(sock2, &requese, sizeof(requese));
        if (rd < 0)
        {
            perror("read");
            return NULL; 
        }


        if (rd == 0)
        {
            perror("read done!");
            close(sock2);
            return NULL;
        }

        write(sock1, &requese, sizeof(requese));
    }
}

    int arr[2] = {0};
    int i = 0;
void CreateWorker(int sock)
{
    while (1)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int accept_sock = accept(sock, (struct sockaddr*)&client, &len);
        if (accept_sock < 0)
        {
            return;
        }
        arr[i] = accept_sock;
        /* printf("%d %d\n",i, accept_sock); */
        write(accept_sock, &i, sizeof(int));
        ++i;
        if (i == 2)
        {
            /* printf("xiancheng\n"); */
            pthread_t th1, th2;
            pthread_create(&th1, NULL, Fighting1, arr);
            pthread_create(&th2, NULL, Fighting2, arr);
            pthread_detach(th1);
            pthread_detach(th2);
            i = 0;
        }

    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("usage : ./server [ip] [port]");
        return 1;
    }

    int sock = InitSocket(argv[1], atoi(argv[2]));
    
    int l = listen(sock, 5);
    if (l < 0)
    {
        perror("listen");
        return 2;
    }


    CreateWorker(sock);
    return 0;
}

