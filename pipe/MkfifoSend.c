#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
    // 程序执行完成才会生成一个文件
    mkfifo("./myfifo", 0666);
    /* if (mf == -1) */
    /* { */
    /*     perror("mkfifr\n"); */
    /*     return 1; */
    /* } */
    // 如果添加了O_CREAT是如果文件不存在就创建一个，后面参数就会多加一个权限
    int fd = open("myfifo", O_WRONLY);
    if (fd < 0)
    {
        perror("open\n");
        return 1;
    }

    char buf[1024];
    while (1)
    {
        // 从标准输入的时候要刷新缓冲区
        fflush(stdout);
        ssize_t rd = read(0, buf, sizeof(buf)-1);
        if (rd > 0)
        {
            // 只要是往缓冲区中读，就要加\0
            buf[rd] = '\0';
            // 这里没有\0的话，在strlen中会多遍历出字节，直到出现\0
            write(fd, buf, strlen(buf));
            const char* _exit = "quit";
            if (strcmp(buf,_exit) == 0)
            {
                break;
            }
        }
        else if (rd == 0)
        {
            printf("read done!");
            return 0;
        }
        else 
        {
            perror("read error\n");
            return 1;
        }
    }

    close(fd);
    return 0;
}
