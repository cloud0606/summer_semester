#pragma once
#ifndef FISH_H
#define FISH_H
#include <windows.h>

typedef enum status {
	_ERROR,
	_OK,
}Status;

//鱼的状态
#define FISH_MOVED 0
#define FISH_EAT_FISH 1
#define FISH_BE_EATEN 2
#define FISH_GROWUP 3
#define FISH_SUCCEED 4
#define FISH_ERROR 5

//坐标
typedef struct _fish_coord {
	int _x;
	int _y;
}_COORD;

//鱼的移动方向
typedef enum fish_move_dir {
	_UP,
	_DOWN,
	_LEFT,
	_RIGHT
}FISH_DIRECTION;

//鱼的等级
typedef enum fish_status {
	FISH_LEV_0,
	FISH_LEV_1,
	FISH_LEV_2,
	FISH_LEV_3,
	FISH_LEV_4
}FISH_LEVEL;

//鱼的结构
typedef struct _fish {
	_COORD _coord;
	_COORD _rang;
	FISH_LEVEL _fishlevel;
	FISH_DIRECTION _fishdir;
}FISH,*PFISH;


#endif // !FISH_H