#include <stdio.h>
#include <unistd.h>
#include <string.h>
/* #include <sys/types.h> */
/* #include <sys/stat.h> */
#include <fcntl.h>

void TestiFWrite()
{
    FILE* fp = fopen("./file.txt", "w");
    if (fp == NULL)
    {
        perror("file open error\n");
        return;
    }
    char buf[] = "Hello World!\n";
    // buf是要写的内容， strlen(buf)是一次写的个数， 1是写的次数，fp是要写入的文件
    ssize_t ret = fwrite(buf , strlen(buf), 1, fp);
    // 返回值为写的次数
    printf("%lu\n", ret);
    fclose(fp);
}

void TestFRead()
{
    FILE* fp = fopen("./file.txt", "r");
    if (fp == NULL)
    {
        perror("open file error");
        return;
    }

    char buf[64] = {0};
    while (1)
    {
        // fread也叫加载, 是把fp中内容加载到buf中
        ssize_t ret = fread(buf, 1, sizeof(buf)-1, fp);
        // 当读到文件末尾时候返回0
        if (ret == 0)
        {
            printf("read complete");
            return;
        }
        if (ret < 0)
        {
            printf("read error\n");
            return;
        }
        buf[ret] = '\0';
        printf("%s", buf);
    }
    fclose(fp);
}

void TestOpenWrite()
{
    int fd = open("./file.txt", O_WRONLY);
    // 打开失败返回-1
    if (fd < 0)
    {
        perror("open error\n");
        return;
    }
    char buf[] = "wo e le\n";
    write(fd, buf, strlen(buf));
    close(fd);
}

void TestOpenRead()
{
    // open的返回值为文件描述符。
    int fd = open("./file.txt", O_RDONLY);
    // 打开失败返回-1
    if (fd < 0)
    {
        perror("open error\n");
        return;
    }
    char buf[64] = {0};
    size_t ret = read(fd, buf, sizeof(buf)-1);
    if (ret == 0)
    {
        printf("read complete");
        return;
    }

    printf("%s\n",buf);
    return;
    close(fd);
}

//////////////////////////////
//注意：
//文件描述符就是文件指针数组下标。
//////////////////////////////

void TestDingxiang()
{
    int fd = open("./file.txt", O_WRONLY);
    if (fd < 0)
    {
        perror("open error\n");
        return;
    }
    char buf[] = "ni hao a !";
    // 重定向，要在1标准输出写，定向到fd文件中
    dup2(fd, 1); 
    write(1, buf, strlen(buf));
    close(fd);
}

int main()
{
    /* TestiFWrite(); */
    /* TestFRead(); */
    /* TestOpenWrite(); */
    /* TestOpenRead(); */
    TestDingxiang();
    return 0;
}
