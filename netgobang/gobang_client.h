#pragma once

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

typedef struct Coordinate
{
    int x;
    int y;
}Coordinate;

// 游戏菜单
int menu();

// 初始化矩阵
void InitArr();

// 棋盘
void Chessboard();

// 输入合法判断
Coordinate InputToDecide();

// 判断输赢
int DecideWinLoss(Coordinate coor, char c);

// 先建立连接的
void FirstTalk(int sock);

// 后建立的
void SecondTalk(int sock);

// 建立连接
int Connect(char* ip, char* port);

// 人机对战
// 判断上下方向
Coordinate UpDown(Coordinate coor);

// 判断上右和下左方向
Coordinate UpRight(Coordinate coor);

// 判断左右方向
Coordinate LeftRight(Coordinate coor);

// 判断右下和左上方向
Coordinate RightDown(Coordinate coor);

// 找出对方下棋子最多的行或者列
Coordinate AroundHave(Coordinate coor, char c);

// 为机器计算要下的位置的函数，其中可以增加算法
Coordinate ProductPos(Coordinate coor);

// 人家对战入口
void PeopleFightMachine();
