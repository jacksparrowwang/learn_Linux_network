#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SIZE 512

void Server(int sock)
{
    while (1)
    {
        struct sockaddr_in addr; // 接受的远端socket信息,这是为了拿到发送信息的人的IP，client上的信息
        socklen_t len = sizeof(addr); // 既是输出也是输入
        char buf[SIZE];
        buf[0] = '\0';
        // 记住强转, 0 为阻塞方式读数据。后面两个为输出参数
        ssize_t ret = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&addr, &len);// addr 为对方的IP和port
        if (ret > 0)
        {
            buf[ret] = '\0';
            // inet_ntoa是把4字节数字大端网络字节序列，转成点分制
            printf("[%s:%d]$ %s\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buf);

            // 这里往客户端发送，因为前面的recvfrom拿到了，client的IP和port和buf
            // 所以回发回去
            sendto(sock, buf, sizeof(buf), 0 , (struct sockaddr*)&addr, len);
        }
    }
}


// ./server_udp 172.0.0.1 8888
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("argc error\n");
        return 1;
    }
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket error\n");
        return 2;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = inet_addr(argv[2]);

    int b =  bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (b < 0)
    {
        perror("bind error\n");
        return 3;
    }

    Server(sock);

    close(sock);
    return 0;
}
