#include <windows.h>
#include "Fish.h"
#include "LinkList.h"
#include "ui.h"

/****************** 全局变量 *******************/
DWORD score = 0;//玩家小鱼的分数
DWORD level = 2;//玩家小鱼的等级
PLIST fishes; //存储鱼群（不包括玩家小鱼）的链表

/****************** 函数定义 *******************/

/* 初始化鱼群 */
int FishInit()
{
	time_t t;
	fishes = ListCreate(0);
	srand((unsigned int)(time(&t)*time(&t)));
	return 0;
}

/* 创建一条鱼 */
Status CreateFish()
{
	int  y, lev;
	PFISH pfish;
	y = rand() % FISH_BOUNDARY_Y;
	lev = rand() % 4 + 1;
	pfish = malloc(sizeof(FISH));
	if (pfish == NULL)
		return _ERROR;
	if (y % 2 == 0) {//使左右两边都产生鱼
		pfish->_coord.x = FISH_BOUNDARY_X;
		pfish->_fishdir = _LEFT;
	}
	else {
		pfish->_coord.x = 0;
		pfish->_fishdir = _RIGHT;
	}
	pfish->_coord.y = y;
	pfish->_rang._x = lev * FISH_MAGNIFY;
	pfish->_rang._y = lev * FISH_MAGNIFY;
	pfish->_fishlevel = lev;
	ListPushFront(fishes, pfish);
	return _OK;
}

/* 移动鱼群的位置 */
Status MoveFishSchool() {
	PFISH pfish;
	int k, i, sum;
	sum = getFishSize();
	for (i = 0; i < sum; i++) {
		k = rand() % FISH_MAX_LEVEL - 2;
		pfish = ListGetAt(fishes, i);
		pfish->_coord.x += FISH_MOVE_STEP;
		if (i % 2 == 0)
			pfish->_coord.y += k* FISH_MOVE_STEP;
		else
			pfish->_coord.y -= k* FISH_MOVE_STEP;
	}
	return _OK;
}

/* 判断鱼是否游出界 */
void IsFishOut() {
	//鱼若游出界，则将鱼等级置为FISH_LEV_0
	int i;
	PFISH pos;
	for (i = 0; i < ListSize(fishes); i++) {
		pos = (PFISH)ListGetAt(fishes, i);

		if (pos->_coord.x > FISH_BOUNDARY_X ||
			pos->_coord.x < 0 ||
			pos->_coord.y > FISH_BOUNDARY_Y ||
			pos->_coord.y < 0) {
			pos->_fishlevel = FISH_LEV_0;
		}
	}
	return;
}

/* 鱼是否被吃 */
BOOL IsFishDead() {
	//判断玩家是否吃掉其他鱼 吃掉则加分
	//或玩家是否被其他鱼吃掉 
	int i, n;
	PFISH pos;
	n = ListSize(fishes);

	//for (i = 0; i < n; i++) {
	//	pos = (PFISH)getFishAt(i);
	//	if (pos->_coord.x < ptPlayer.x  &&
	//		pos->_coord.x + pos->_rang._x > ptPlayer.x  &&
	//		pos->_coord.y  < ptPlayer.y + PLAYER_HEIGHT / 2 &&
	//		pos->_coord.y + pos->_rang._y  > ptPlayer.y + PLAYER_HEIGHT / 2) {
	//		if (pos->_fishlevel < getLevel())
	//		{//玩家得分
	//			score += FISH_SCORE_ADD * pos->_fishlevel;
	//			pos->_fishlevel = FISH_LEV_0;
	//		}
	//		else if (pos->_fishlevel > getLevel()) {
	//			//玩家小鱼死亡
	//			setLevel(FISH_LEV_0);
	//			return 1;
	//		}
	//	}
	//}

	for (i = 0; i < n; i++) {
		pos = (PFISH)getFishAt(i);
		//判断位置位于玩家鱼嘴
		if (pos->_coord.x + FISH_DEAD_CONTROL_X < ptPlayer.x  &&
			pos->_coord.x + pos->_rang._x - FISH_DEAD_CONTROL_X > ptPlayer.x  &&
			pos->_coord.y + FISH_DEAD_CONTROL_Y < ptPlayer.y + PLAYER_HEIGHT / 2 &&
			pos->_coord.y + pos->_rang._y - FISH_DEAD_CONTROL_Y > ptPlayer.y + PLAYER_HEIGHT / 2) {
			if (pos->_fishlevel < getLevel())
			{//玩家得分
				score += FISH_SCORE_ADD * pos->_fishlevel;
				pos->_fishlevel = FISH_LEV_0;
			}
			else if (pos->_fishlevel > getLevel()) {
				//玩家小鱼死亡
				setLevel(FISH_LEV_0);
				return 1;
			}
		}
	}
		return 0;

    
}

/* 鱼是否可以升级 */
void FishUpgrade() {
	switch (getLevel()) {
	case FISH_LEV_1:
		if (score > FISH_SCORE_ADD * FISH_UPGRADE*FISH_LEV_1)
			setLevel(FISH_LEV_2);
		break;
	case FISH_LEV_2:
		if (score >  FISH_SCORE_ADD * FISH_UPGRADE*FISH_LEV_2)
			setLevel(FISH_LEV_3);
		break;
	case FISH_LEV_3:
		if (score > FISH_SCORE_ADD * FISH_UPGRADE*FISH_LEV_3)
			setLevel(FISH_LEV_4);
		break;

	case FISH_LEV_4:
		if (score > FISH_SCORE_ADD * FISH_UPGRADE*FISH_LEV_4)
			setLevel(FISH_LEV_MAX);
	default:
		break;
	}
}

/* 删除第i个小鱼节点 */
void DestroyFishAt(PLIST plist, int i) {
	PFISH pf;
	pf = ListDeleteAt(plist, i);
	free(pf);
}

/* 根据状态删除鱼 */
void DestroyFishByState() {
	int i, n;
	PFISH pf;
again:
	n = getFishSize();
	for (i = 0; i < n; i++) {
		pf = (PFISH)getFishAt(i);
		if (pf->_fishlevel == FISH_LEV_0) {
			DestroyFishAt(fishes, i);
			goto again;
		}
	}

}

/* 返回第i条鱼 */
PFISH getFishAt(int i) {
	return ListGetAt(fishes, i);
}

/* 返回玩家小鱼的等级 */
DWORD getLevel() {
	return level;
}

/* 设置玩家小鱼的等级 */
void setLevel(FISH_LEVEL i) {
	level = i;
}

/* 返回玩家小鱼的分数 */
DWORD getScore() {
	return score; 
}

/* 设置玩家小鱼的分数 */
void setScore(DWORD s) {
	score = s;
}

/* 返回鱼群的数量 */
int getFishSize() {
	return ListSize(fishes);
}