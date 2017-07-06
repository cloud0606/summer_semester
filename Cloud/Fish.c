#include <windows.h>
#include "Fish.h"
#include "LinkList.h"
#include "Game.h"

/*************************/
/* ������Ⱥ */
PLIST CreateFishSchool();

/* ����һ���� */
Status CreateFish(PLIST list,
	int x, int y,
	int range_x, int range_y,
	FISH_LEVEL fish_lev,
	FISH_DIRECTION fish_dir);

/* �ƶ���Ⱥ��λ�� */
Status MoveFishSchool(PLIST list);

/* ������Ⱥ */
PLIST CreateFishSchool() {
	int i;
	int y;
	PLIST plist;
	int fishlevel;
	int sum = malloc(sizeof(int));//�洢��Ⱥ������
	sum = FISH_SUM;
	plist = ListCreate(&sum);
	for (i = 0; i < FISH_SUM; i++) {
		fishlevel = rand() % FIS_MAX_LEVEL + 1;
		if (i % 2 == 0) {//��Ⱥ�ֱ���������߽���
			y = rand() % FISH_BOUNDARY_Y;
			CreateFish(plist, 0, y, fishlevel, fishlevel, fishlevel, _RIGHT);
		}
		else {
			y = rand() % FISH_BOUNDARY_Y;
			CreateFish(plist, FISH_DOUNDARY_X, y, fishlevel, fishlevel, fishlevel, _LEFT);
		}

	}
	return plist;
}

/* ����һ���� */
Status CreateFish(PLIST list,
	int x, int y,
	int range_x, int range_y,
	FISH_LEVEL fish_lev,
	FISH_DIRECTION fish_dir)
{
	PFISH pfish;
	pfish = malloc(sizeof(FISH));
	if (pfish == NULL)
		return _ERROR;
	pfish->_coord._x = x;
	pfish->_coord._y = y;
	pfish->_rang._x = range_x;
	pfish->_rang._y = range_y;
	pfish->_fishlevel = fish_lev;
	pfish->_fishdir = fish_dir;
	ListPushFront(list, pfish);
	return _OK;
}

/* �ƶ���Ⱥ��λ�� */
Status MoveFishSchool(PLIST list) {
	PFISH pfish;
	int k,i,sum;
	sum = list->list_data;
	for (i = 0; i < sum; i++) {
		k = rand() % FIS_MAX_LEVEL;
		pfish = ListGetAt(list, i);
		pfish->_coord._x += FISH_MOVE_STEP;
		pfish->_coord._y += k* FISH_MOVE_STEP;
	}
	return _OK;
}