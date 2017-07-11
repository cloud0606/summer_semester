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
	POINT _coord;
	_COORD _rang;
	FISH_LEVEL _fishlevel;
	FISH_DIRECTION _fishdir;
}FISH, *PFISH;

/*********** 函数声明 **************/

/* 初始化鱼群 */
int FishInit();

/* 创建一条鱼 */
Status CreateFish();

/* 移动鱼群的位置 */
Status MoveFishSchool();

/* 判断鱼是否游出界 */
void IsFishOut();

/* 鱼是否被吃 */
BOOL IsFishDead();

/* 鱼是否可以升级 */
void FishUpgrade();

/* 根据状态删除鱼 */
void DestroyFishByState();

/* 返回第i条鱼 */
PFISH getFishAt(int i);

/* 返回鱼群的数量 */
int getFishSize();

/* 返回玩家小鱼的分数 */
DWORD getScore();

/* 设置玩家小鱼的分数 */
void setScore(DWORD s);

/* 返回玩家小鱼的等级 */
DWORD getLevel();

/* 设置玩家小鱼的等级 */
void setLevel(FISH_LEVEL i);

#endif // !FISH_H