#include "gobang_client.h"

#define ROW 15
#define COL 15

int arr[ROW][COL];

// 初始化棋盘
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
int menu()
{
    while (1)
    {
        printf("**************************\n");
        printf("  1 人机对战  2 联机对战  \n");  
        printf("          0 退出          \n");  
        printf("**************************\n");
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
        if (buf[0] == '2')
        {
            return 2;
        }
        else if (buf[0] == '1')
        {
            return 1;
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
    return 0;
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

    // 左右
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

    // 右下
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

// 建立连接
int Connect(char* ip, char* port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return 2;
    }

    // 建立连接
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(atoi(port));
    int n = connect(sock, (struct sockaddr*)&server, sizeof(server));
    if(n < 0)
    {
        perror("connect");
        return 3;
    }
    return sock;
}

//////////////////////////
//注释为上右注释，其它情况路基相同
//////////////////////////
//进行下棋位置锁定
// 判断上下方向是否可以下棋
Coordinate UpDown(Coordinate coor)
{
    int x = coor.x;
    int y = coor.y;
    while (x >= 0 && arr[x][y] == '$')
    {
        --x;
    }
    if (x >= 0 && arr[x][y] == ' ')
    {
        coor.x = x;
        coor.y = y;
        return coor;
    }
    if (x < 0 || arr[x][y] == '@')
    {
        x = coor.x;
        y = coor.y;
        while (x < ROW && arr[x][y] == '$')
        {
            ++x;
        }
        coor.x = x;
        coor.y = y;
        return coor;
    }
    return coor;
}

Coordinate UpRight(Coordinate coor)
{
    int x = coor.x;
    int y = coor.y;
    while (x >= 0 && y < COL && arr[x][y] == '$')
    {
        --x;
        ++y;
    }
    if (x >= 0 && y < COL && arr[x][y] == ' ')
    {
       coor.x = x;
       coor.y = y;
       return coor;
    }
    if (x < 0 || y == COL || arr[x][y] == '@')
    {
        x = coor.x;
        y = coor.y;
        while (x < ROW && y >= 0 && arr[x][y] == '$')
        {
            ++x;
            --y;
        }
        coor.x = x;
        coor.y = y;
    }
    // 如果出错返回原来的位置
    return coor;
}

Coordinate LeftRight(Coordinate coor)
{
    int x = coor.x;
    int y = coor.y;
    while (y >= 0 && arr[x][y] == '$')
    {
        --y;
    }
    if (y >= 0 && arr[x][y] == ' ')
    {
        coor.x = x;
        coor.y = y;
        return coor;
    }
    if (y < 0 || arr[x][y] == '@')
    {
        x = coor.x;
        y = coor.y;
        while (y < COL && arr[x][y] == '$')
        {
            ++y;
        }
        coor.x = x;
        coor.y = y;
        return coor;
    }
    return coor;
}

Coordinate RightDown(Coordinate coor)
{
    int x = coor.x;
    int y = coor.y;
    // 先去判断左上进行判断
    while (x >= 0 && y >= 0 && arr[x][y] == '$')
    {
        --x;
        --y;
    }
    // 看左上的最后一个是不是‘ ’
    if (x >= 0 && y >= 0 && arr[x][y] == ' ')
    {
        coor.x = x;
        coor.y = y;
        return coor;
    }
    // 这里就是左上位置自己已经下了，或者到了边界
    if (x < 0 || y < 0 || arr[x][y] == '@')
    {
        // 回到对象下棋的位置
        x = coor.x;
        y = coor.y;
        // 进行右下方向的判断
        while (x < ROW && y < COL && arr[x][y] == '$')
        {
            ++x;
            ++y;
        }
        // 因为在最开始寻找最长路的时候，已经判断了两端都被堵死的情况，所以现在一定会有一个位置
        coor.x = x;
        coor.y = y;
        return coor;
    }
    // 就相当与异常
    // 如果没有进入其中的判断语句返回原来的左边，在函数入口进行判断。
    return coor;
}
// 第一次生成坐标
// 通过走一圈来比较出那条路线上的棋子最多
// c 是 $
Coordinate AroundHave(Coordinate coor, char c)
{
    Coordinate _coor;
    _coor.x = coor.x;
    _coor.y = coor.y;
    int count[4] = {0};
    // 上下方向
    // 先走到最上
    while (coor.x >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.x;
    }
    // 判断是不是最上的下一个是对方棋子，为后面的下棋排除一种两端都堵死的情况
    int fg0 = 0;
    if (coor.x < 0 || arr[coor.x][coor.y] == '@')
    {
        fg0 = 1;
    }
    // 返回最上位置
    ++coor.x;
    // 计算上下这条路中的对方棋子数
    while (coor.x < ROW && arr[coor.x][coor.y] == c)
    {
        ++coor.x;
        ++count[0];// 记录在数组中
    }
    // 如果上下对方的棋子已经被我们堵死，这时候就不用管这种情况了
    if ((coor.x ==  ROW || arr[coor.x][coor.y] == '@') && fg0 == 1) //判断这是为了解决这个路径上的棋子已经被堵死
    {
        count[0] = 0;
    }

    /////// 按照上下逻辑写出来右上、右左、右下
    // 上右方向
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.x >= 0 && coor.y < COL && arr[coor.x][coor.y] == c)
    {
        --coor.x;
        ++coor.y;
    }
    int fg1 = 0;
    if (coor.x == 0 || coor.y == COL || arr[coor.x][coor.y] == '@')
    {
        fg1 = 1;
    }
    ++coor.x;
    --coor.y;
    while (coor.x < ROW && coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
       ++coor.x;
       --coor.y;
       ++count[1];
    }
    if ((coor.x == ROW || coor.y == 0 || arr[coor.x][coor.y] == '@') && fg1 == 1)
    {
        count[1] = 0;
    }

    // 左右
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.y < COL && arr[coor.x][coor.y] == c)
    {
        ++coor.y;
    }
    int fg2 = 0;
    if (coor.y == COL || arr[coor.x][coor.y] == '@')
    {
        fg2 = 1;
    }
    --coor.y;
    while (coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.y;
        ++count[2];
    }
    if ((coor.y == 0 || arr[coor.x][coor.y] == '@') && fg2 == 1)
    {
        count[2] = 0;
    }

    // 右下
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.y < COL && coor.x < ROW && arr[coor.x][coor.y] == c)
    {
        ++coor.x;
        ++coor.y;
    }
    int fg3 = 0;
    if (coor.y == COL || coor.x == ROW || arr[coor.x][coor.y] == '@')
    {
        fg3 = 1;
    }
    --coor.x;
    --coor.y;
    while (coor.x >= 0 && coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.x;
        --coor.y;
        ++count[3];
    }
    if ((coor.x == 0 || coor.y == 0 || arr[coor.x][coor.y] == '@') && fg3 == 1)
    {
        count[3] = 0;
    }

    // 用来判断哪个路径上的棋子多
    int count_num = 0;
    int i = 0;
    for (; i < 4; ++i)
    {
        int j = 0;
        for (; j < 4; ++j) 
        {
            if (count[i] < count[j])
            {
                break;
            }
        }
        if (j == 4)
        {
            /* printf("来看比较是那个路上进行下棋%d", i); */
            count_num = i;
            break;
        }
    }
    
    // 进行分支判断, 需要在那条路上进行下棋
    if (count_num == 0)
    {
        // 上下路
        Coordinate ud = UpDown(_coor);// 来处理各各路上的在那边下棋;
        if (ud.x == _coor.x && ud.y == _coor.y)
        {
            perror("UpDown error\n");
            return coor;
        }
        // 这是处理当下的棋子为第0行的时候，棋子要下在下方
        // 防止数组越界
        if (ud.x < 0)
        {
            ++ud.x;
            while (arr[ud.x][ud.y] == '$')
            {
                ++ud.x;
            }
            return ud;
        }
        return ud;

    }
    else if (count_num == 1)
    {
        // 上右和下左进行判断
        Coordinate ur = UpRight(_coor);
        if (ur.x == _coor.x && ur.y == _coor.y)
        {
            perror("UpRight error\n");
            return coor;
        }
        return ur;
    }
    else if (count_num == 2)
    {
        // 左右 判断
        Coordinate lr = LeftRight(_coor);
        if (lr.x == _coor.x && lr.y == _coor.y)
        {
            perror("LeftRight error\n");
            return coor;
        }
        return lr;
    }
    else
    {
        // 右下和左上判断
        Coordinate rd = RightDown(_coor);
        if (rd.x == _coor.x && rd.y == _coor.y)
        {
            perror("RightDown error\n");
            return coor;
        }
        return rd;
    }

    // 坐标生成出错，自定义一个数字，回返后判断是自定义数字
    // 则返回错误信息
    Coordinate tmp;
    tmp.x = 100;
    tmp.y = 100;
    return tmp;
}


// 这里预留接口，为后续加入更复杂的程序，进行更有难度的计算坐标
Coordinate ProductPos(Coordinate coor)
{
   return AroundHave(coor, '$');
}

// 人机对战
void PeopleFightMachine()
{
    // 初始化棋盘
    InitArr();
    while (1)
    {
        Chessboard();
        printf("请落子~\n");
        // 判断合法输入
        Coordinate coor = InputToDecide();
        
        // 下棋子
        arr[coor.x][coor.y] = '$';
        Chessboard();
        // 判断输赢
        int decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        if (decide == 0)
        {
            printf("恭喜你~ 击败对手~\n");
            return;
        }

        // 电脑下
        // 电脑分析，要下位置
        coor = ProductPos(coor);
        //
        if (coor.x == 100)
        {
            perror("错误\n");
            return;
        }
        
        // 下棋子
        arr[coor.x][coor.y] = '@';
        Chessboard();

        decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        if (decide == 0)
        {
            printf("很遗憾~ 你输了~\n");
            return;
        }
    }
}

// 要输入./client IP port 命令行参数
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("usage: ./client [ip] [port]");
        return 1;
    }

    ///////////////////////////////////
    // 进行人机对战
    ///////////////////////////////////
    while (1)
    {
        // 菜单
        int me = menu(); // 游戏开始
        if (me == 1)
        {
            // 人机对战
            PeopleFightMachine();
        }
        else if (me == 2) // 选择2 说明要进行网络对战
        {
            // 跳出去说明是想网络对战
            break;
        }
    }

    ///////////////////////////////////
    // 网络对战,模块
    ///////////////////////////////////

    // 建立连接
    int sock = Connect(argv[1], argv[2]);

    // 用来接收服务器发送的ID 用来判断是黑方还是白方
    int i = 0;
    read(sock, &i, sizeof(int));
    /* printf("%d", i); */
    if (i == 0)
    {
        // 黑方棋手
        InitArr();
        Chessboard();
        FirstTalk(sock);
    }
    else if (i == 1)
    {
        // 白方棋手
        InitArr();
        Chessboard();
        SecondTalk(sock);
    }
    else
    {
        perror("先后顺序错误");
        return -1;
    }
    return 0;
}

