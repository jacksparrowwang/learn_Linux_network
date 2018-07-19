#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// 非阻塞IO，往往要搭配轮询式的去判断是否有数据，
// 缺点就是浪费CPU资源
void SetNoBlock(int fd)
{
    // 先是将文件描述符的属性取出来
    int f1 = fcntl(fd, F_GETFL);
    if (f1 < 0)
    {
        perror("fcntl error\n");
        return;
    }
    // O_NONBLOCK 就是 04000
    // 对文件描述符的属性进行设置，然后设置回去
    fcntl(fd, F_SETFL, f1|O_NONBLOCK);
}
int main()
{
    // 将输入设置为非阻塞读
    SetNoBlock(0);
    while (1)
    {
        char buf[512] = {0};
        // 这里的读就会非阻塞的进行去读,读失败就会重新去读
        ssize_t read_size = read(0, buf, sizeof(buf)-1);
        if (read_size < 0)
        {
            perror("read error\n");
            sleep(1);
            continue;
        }
        if (read_size == 0)
        {
            printf("read down\n");
            return 0;
        }
        buf[read_size] = '\0';
        printf("%s", buf);
    }
    return 0;
}
