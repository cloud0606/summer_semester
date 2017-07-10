#pragma once
#ifndef  GAME_H
#define GAME_H
#include <Windows.h>
/****************** 宏定义 *******************/
//鱼群的参数
#define FISH_MOVE_STEP  3  //鱼群（不包括玩家）的移动步长
#define FISH_MAGNIFY 25   //设置鱼群（不包括玩家）的放大倍数
#define FISH_MAX_LEVEL 4  //鱼群的最大等级
#define FISH_MOVE_STEP_PLAYER 20 //玩家小鱼的移动步长
#define FISH_SCORE_ADD 100 //设置吃掉小鱼后得分的幅度

//游戏边界
#define FISH_BOUNDARY_X  800
#define FISH_BOUNDARY_Y  500

//玩家小鱼的宽和高
#define PLAYER_WIDTH 60
#define PLAYER_HEIGHT 40

/***************** 全局变量 ********************/
//玩家小鱼的坐标
POINT ptPlayer;

/***************** 函数声明 ********************/

/* 背景窗口相关函数 */
LONG CALLBACK BackGroundWindowProc(
	HWND hwnd, //
	UINT msg, // 消息
	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam);

BOOL BackGroundWindwowRegister(HINSTANCE hinstance);

HWND BackgroundWindowCreate(HINSTANCE hinstance);

LONG BackgroundCreate(HWND hwnd);

LONG BackgroundPaint(HWND hwnd);

LONG BackgroundResizeAndMove(HWND hwnd, LPRECT lpRect);

LONG OnKeydown(HWND hwnd, UINT vk);

/* 玩家（鱼群和玩家）窗口相关函数 */
LONG CALLBACK PlayerWindowProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam);

HWND PlayerWindowCreate(HINSTANCE hinstance);

BOOL PlayerWindowRegister(HINSTANCE hinstance);

LONG PlayerCreate(HWND hwnd);

LONG PlayerWindowPaint(HWND hwnd);

LONG PlayerTimer(HWND hwnd);



#endif // ! GAME_H
#pragma once
