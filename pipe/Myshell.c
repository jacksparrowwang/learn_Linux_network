#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// 字符串切割
void Do_Split(char buf[], char* argv[])
{
    char* token = strtok(buf, " ");
    argv[0] = token;
    int i = 1;
    while (token != NULL)
    {
        token = strtok(NULL, " ");
        argv[i++] = token;
    }
    argv[i] = NULL;
}

// 创建子进程,进行进程的替换
void Do_Execute(char* argv[])
{
    pid_t id = fork();
    if (id > 0)
    {
        // father
        wait(NULL);
    }
    else if (id == 0)
    {
        // child
        execvp(argv[0], argv);
        perror(argv[0]);
        printf("替换错误 \n");
        exit(1);
    }
    else
    {
        perror("fork\n");
    }
}

void write_read_pipe(char* front[], char* back[])
{
    int fd[2];
    int f = pipe(fd);
    if (f != 0)
    {
        perror("pipe error\n");
        return;
    }
    pid_t id = fork();
    if (id == -1)
    {
        perror("fork1\n");
        exit(1);
    }
    if (id == 0)
    {
        close(fd[0]);
        // 把标准输入，重定向到管道的读
        dup2(fd[1], 1);
        execvp(front[0], front);
        perror("exec error\n");
        exit(0);
    }
    pid_t id2 = fork();
    if (id2 == -1)
    {
        perror("fork2\n");
        exit(1);
    }
    if (id2 == 0)
    {
        close(fd[1]);
        // 把从标准输入读定向到管道的读
        dup2(fd[0],0);
        execvp(back[0], back);
        perror("exec error\n");
        exit(0);
    }
    close(fd[1]);
    close(fd[0]);
}

// 处理有管道的情况
// pos是管道|是在argv指针数组中的那个位置
void Pipe(char* argv[], int pos)
{
    // 用两个指针数组来保存，管道前命令和管道后的命令
    char* front[32]; 
    char* back[32];
    int i = 0;
    // 用来给back赋值时候的循环
    int b_i = 0;
    while (argv[i] != NULL)
    {
       if (i < pos)
       {
           front[i] = argv[i];
       }
       else if (i > pos)
       {
           back[b_i] = argv[i];
           ++b_i;
       }
       else
       {
           // 用来结束第一个赋值
           front[i] = NULL;
       }
       ++i;
    }
    back[b_i] = NULL;
    write_read_pipe(front, back);
}

int main(int argc, char* argv[], char* env[])
{
    (void)argc;
    (void)env;
    char buf[1024] = {};
    while (1)
    {
        char arr[50] = {};
        gethostname(arr, 9);
        fprintf(stdout, "[myshell@%s]$ ",arr);
        gets(buf);
        /* scanf("%[^\n]%*c",buf); */
        Do_Split(buf,argv);
        int i = 0;
        while (argv[i] != NULL)
        {
            if (strcmp(argv[i],"|") == 0)
            {
                Pipe(argv, i);
                break;
            }
            ++i;
        }
        if (argv[i] == NULL)
        {
            Do_Execute(argv);
        }
    }
    return 0;
}
