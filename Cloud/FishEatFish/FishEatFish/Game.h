#pragma once
#ifndef  GAME_H
#define GAME_H
#include <Windows.h>

//��Ⱥ�Ĳ���
#define FISH_SUM 10
#define FISH_MOVE_STEP 2

//��Ϸ�߽�
#define FISH_BOUNDARY_X  800
#define FISH_BOUNDARY_Y  500

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 40

/*********** �������� ************/
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
