#pragma once
#ifndef  GAME_H
#define GAME_H
#include <Windows.h>

//鱼群的参数
#define FISH_SUM 10
#define FISH_MOVE_STEP 3
#define FISH_MOVE_STEP_PLAYER 20
#define FISH_SCORE_ADD 100

//游戏边界
#define FISH_BOUNDARY_X  800
#define FISH_BOUNDARY_Y  500

#define PLAYER_WIDTH 60
#define PLAYER_HEIGHT 40

//玩家坐标
POINT ptPlayer;

/*********** 函数声明 ************/
BOOL BackGroundWindwowRegister(HINSTANCE hinstance);

HWND BackgroundWindowCreate(HINSTANCE hinstance);

LONG BackgroundCreate(HWND hwnd);

LONG BackgroundPaint(HWND hwnd);

LONG CALLBACK BackGroundWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL PlayerWindowRegister(HINSTANCE hinstance);

HWND PlayerWindowCreate(HINSTANCE hinstance);

LONG PlayerCreate(HWND hwnd);

LONG CALLBACK PlayerWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);




#endif // ! GAME_H
#pragma once
