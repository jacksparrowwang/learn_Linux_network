#include "gobang_client.h"

#define ROW 15
#define COL 15

int arr[ROW][COL];

void InitArr()
{
    int i = 0;
    for (; i < ROW; i++)
    {
        int j = 0;
        for (; j < COL; ++j)
        {
            arr[i][j] = ' ';
        }
    }
}
// 游戏棋盘
void Chessboard()
{
    int w = 0;
    for (; w < COL; ++w)
    {
        printf("%4d", w);
    }
    printf("\n");

    int i = 0;
    for (; i < ROW; ++i)
    {
        printf("%2d",i);

        int j = 0;
        for (; j < COL; ++j)
        {
            printf(" %c |", arr[i][j]);
        }
        printf("\n");

        printf("  ");
        int q = 0;
        for (;q < COL; ++q)
        {
            printf("---|");
        }
        printf("\n");
    }
}

// 游戏菜单
void menu()
{
    while (1)
    {
        printf("**********************\n");
        printf("   1 开始    0 退出   \n");
        printf("**********************\n");
        char buf[512];
        ssize_t rd = read(0, &buf, sizeof(buf)-1);
        if (rd < 0)
        {
            perror("读取错误，请重新选择\n");
            continue;
        }
        buf[rd-1] = '\0';

        fflush(stdin);
        if (strlen(buf) != 1)
        {
            printf("输入错误\n");
            continue;
        }
        if (buf[0] == '1')
        {
            Chessboard();
            break;
        }
        else if (buf[0] == '0')
        {
            printf("游戏结束\n");
            exit(0);
        }
        else
        {
            printf("输入错误\n");
        }
    }
}

// 输入合法判断
Coordinate InputToDecide()
{
    Coordinate coor;
    printf("请输入要下的坐标~ 如：x y\n");
    while (1)
    {
        scanf("%d%d", &(coor.x), &(coor.y));
        if (coor.x < 0 || coor.x >= ROW || coor.y < 0 || coor.y >= COL)
        {
            printf("输入棋子坐标错误,请冲输入~\n");
            continue;
        }
        if (arr[coor.x][coor.y] == ' ')
        {
            break;
        }
        printf("输入的棋子坐标已经被占有,请重新输入~\n");
    }
    return coor;
}

int DecideWinLoss(Coordinate coor, char c)
{
    Coordinate _coor;
    _coor.x = coor.x;
    _coor.y = coor.y;
    // 上下方向
    int count = 0;
    while (coor.x >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.x;
    }
    ++coor.x;
    while (coor.x < ROW && arr[coor.x][coor.y] == c)
    {
        ++coor.x;
        ++count;
    }
    if (count >= 5)
    {
        // 0代表成功
        /* printf("shag\n"); */
        return 0;
    }

    // 上右方向
    count = 0;
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.x >= 0 && coor.y < COL && arr[coor.x][coor.y] == c)
    {
        --coor.x;
        ++coor.y;
    }
    ++coor.x;
    --coor.y;
    while (coor.x < ROW && coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
       ++coor.x;
       --coor.y;
       ++count;
       /* printf("1\n"); */
    }
    if (count >= 5)
    {

        /* printf("上you\n"); */
        return 0;
    }

    count = 0;
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.y < COL && arr[coor.x][coor.y] == c)
    {
        ++coor.y;
    }
    --coor.y;
    while (coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.y;
        ++count;
    }
    if (count >= 5)
    {
        /* printf("you\n"); */
        return 0;
    }

    count = 0;
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.y < COL && coor.x < ROW && arr[coor.x][coor.y] == c)
    {
        ++coor.x;
        ++coor.y;
    }
    --coor.x;
    --coor.y;
    while (coor.x >= 0 && coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.x;
        --coor.y;
        ++count;
    }
    if (count >= 5)
    {
        return 0;
    }
    return -1;
}

// 先建立连接的客户端
void FirstTalk(int sock)
{
    while (1)
    {
        // 判断输入是否合法
        Coordinate coor = InputToDecide();
        
        // 下棋子
        arr[coor.x][coor.y] = '$';
        Chessboard();

        int decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        printf("coor %d %d", coor.x, coor.y);
        coor.x = htonl(coor.x);
        coor.y = htonl(coor.y);
        write(sock, &coor, sizeof(coor));

        if (decide == 0)
        {
            printf("恭喜你~ 胜利了~\n");
            close(sock);
            return;
        }

        printf("对方下子中......\n");
        ssize_t rd = read(sock, &coor, sizeof(coor));
        if (rd < 0)
        {
            perror("read");
            return;
        }

        if (rd == 0)
        {
            printf("read done!");
            return;
        }
        coor.x = ntohl(coor.x);
        coor.y = ntohl(coor.y);

        arr[coor.x][coor.y] = '@';
        Chessboard();
        printf("对手: x = %d, y = %d", coor.x, coor.y);

        // 这里进行判断输赢
        decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        if (decide == 0)
        {
            printf("很遗憾~ 你输了~\n");
            close(sock);
            return;
        }
    }
}

// 后建立连接的
void SecondTalk(int sock)
{
    Coordinate coor;
    while (1)
    {
        printf("对方下子中......\n");
        ssize_t rd = read(sock, &coor, sizeof(coor));
        if (rd < 0)
        {
            perror("read");
            return;
        }

        if (rd == 0)
        {
            printf("read done!");
            return;
        }
        coor.x = ntohl(coor.x);
        coor.y = ntohl(coor.y);
       
        arr[coor.x][coor.y] = '$';
        Chessboard();
        printf("对方: x = %d, y = %d", coor.x, coor.y);
        int decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        if (decide == 0)
        {
            printf("很遗憾~ 你输了~\n");
            close(sock);
            return;
        }

        coor = InputToDecide();
        arr[coor.x][coor.y] = '@';
        Chessboard();

        decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        coor.x = htonl(coor.x);
        coor.y = htonl(coor.y);
        write(sock, &coor, sizeof(coor));

        if (decide == 0)
        {
            printf("恭喜你~ 胜利了~\n");
            close(sock);
            return;
        }
    }
}
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("usage: ./client [ip] [port]");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return 2;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));
    int n = connect(sock, (struct sockaddr*)&server, sizeof(server));
    if(n < 0)
    {
        perror("connect");
        return 3;
    }

    int i = 0;
    read(sock, &i, sizeof(int));
    /* printf("%d", i); */
    if (i == 0)
    {
        InitArr();
        menu(); // 游戏开始
        FirstTalk(sock);
    }
    else if (i == 1)
    {
        InitArr();
        menu();
        SecondTalk(sock);
    }
    else
    {
        perror("先后顺序错误");
        return -1;
    }
    return 0;
}

