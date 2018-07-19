#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
/////////////////////////
// select 函数参数 nfds 为需要监视最大文件描述符值+1
//
//        readfds  writefds  exceptfds 
//        分别对应可读文件描述符集
//        可写文件描述符集
//        异常文件描述符集
//        timeout 对应的是结构体 timeval 用来设置select等待时间
//        读 写 异常 对应的结构体fd_set 对应的是一个位图
//
//        返回值：执行成功返回文件描述符改变的个数
//        返回0代表超时
//        错误返回-1
/////////////////////////
// 对fd_set结构体，相关的位图操作接口
// void FD_CLR(int fd, fd_set* set); // 用来清除某一位上set的相关fd的位置
// int FD_ISSET(int fd, fd_set* set); // 用来测试set中相关的fd位是否为真
// void FD_SET(int fd, fd_set* set); // 用来设置描述词组中的fd的位
// void FD_ZERO(fd_set* set); // 用来清理描述词中set的全部位

int main()
{
    fd_set read_fd;
    FD_ZERO(&read_fd);
    FD_SET(0, &read_fd);

    struct timeval time;
    time.tv_sec = 5; // 5秒后超时 select返回不会再进行等待判断

    for (;;)
    {
        fd_set read_fd_tmp = read_fd;
        printf(">");
        fflush(stdout);
        int result = select(1, &read_fd_tmp, NULL,NULL,&time);
        if (result < 0)
        {
            perror("select error");
            continue;
        }
        if (FD_ISSET(0, &read_fd_tmp))
        {
            printf("nihao\n");
            char buf[512] = {0};
            read(0, buf, sizeof(buf)-1);
            printf("input:%s ", buf);
        }
        else if (result == 0)
        {
            printf("超时\n");
            continue;
        }
        else
        {
            printf("invaild fd\n");
            continue;
        }
    }
    return 0;
}

