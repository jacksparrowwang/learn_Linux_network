#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
    // 如果添加了O_CREAT是如果文件不存在就创建一个，后面参数就会多加一个权限
    int fd = open("myfifo", O_RDONLY);
    if (fd < 0)
    {
        perror("open\n");
        return 1;
    }

    char buf[1024] = {0};
    while (1)
    {
        ssize_t rd = read(fd, buf, sizeof(buf)-1);

        if (rd > 0)
        {
            // 注意\0
            buf[rd] = '\0';
            printf("%s", buf);
        }
        else if (rd == 0)
        {
            printf("read done!\n");
            return 0;
        }
        else 
        {
            perror("read error\n");
            return 1;
        }
        if (strcmp(buf,"quit!") == 0)
        {
            break;
        }
    }

    close(fd);
    return 0;
}
