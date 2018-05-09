#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char* argv[], char* env[])
{
    (void)argc;
    (void)env;
    char buf[1024] = {};
    while (1)
    {
        char arr[50] = {};
        gethostname(arr, 9);
        printf("[myshell@%s]$ ",arr);
        /* gets(buf); */
        scanf("%[^\n]%*c",buf);
        Do_Split(buf,argv);
        Do_Execute(argv);
    }
    return 0;
}
