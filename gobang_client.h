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
void menu();

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


