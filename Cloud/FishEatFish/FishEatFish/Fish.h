#pragma once
#pragma once
#ifndef FISH_H
#define FISH_H
#include <windows.h>
#include "LinkLIst.h"

typedef enum status {
	_ERROR,
	_OK
}Status;

typedef enum _bool {
	_FALSE,
	_TRUE
}BOOL;
//
#define FISH_MAX_LEVEL 4
#define FISH_MAGNIFY 10

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
	FISH_LEV_0,//0级表示死亡
	FISH_LEV_1,
	FISH_LEV_2,
	FISH_LEV_3,
	FISH_LEV_4,
	FISH_LEV_MAX
}FISH_LEVEL;

//鱼的结构
typedef struct _fish {
	_COORD _coord;
	_COORD _rang;
	FISH_LEVEL _fishlevel;
	FISH_DIRECTION _fishdir;
}FISH, *PFISH;

/*********** 函数声明 **************/

/* 初始化鱼群 */
int FishInit();

/* 创建鱼群 */
PLIST CreateFishSchool();

/* 创建一条鱼 */
Status CreateFish();

/* 移动鱼群的位置 */
Status MoveFishSchool();

/* 判断鱼是否游出界 */
void IsFishOut(PLIST plist);

/* 鱼是否被吃 */
void IsFishDead(PLIST plist, PFISH player);

/* 鱼是否可以升级 */
void FishUpgrade(PFISH player);

/* 根据状态删除鱼 */
void DestroyFishByStatus();

/* 返回第i条鱼 */
PFISH GetFishAt(int i);

int getFishX(PFISH f);

int getFishY(PFISH f);

int getFishSize();

DWORD getScore();

DWORD getLevel();
#endif // !FISH_H