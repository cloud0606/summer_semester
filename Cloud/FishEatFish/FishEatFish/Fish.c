#include <windows.h>
#include "Fish.h"
#include "LinkList.h"
#include "Game.h"

/********** ȫ�ֱ��� **********/
DWORD score = 0;
DWORD level = 0;
PLIST fishes;

/*********** �������� **************/

/* ��ʼ����Ⱥ */
int FishInit()
{
	time_t t;
	fishes = ListCreate(0);
	srand((unsigned int)(time(&t)*time(&t)));
	return 0;
}

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
			CreateFish();
	}
	return plist;
}

/* ����һ���� */
Status CreateFish()
{
	int  y, lev;
	PFISH pfish;
	y = rand() % FISH_BOUNDARY_Y;
	lev = rand() % 4 + 1;
	pfish = malloc(sizeof(FISH));
	if (pfish == NULL)
		return _ERROR;
	if (y % 2 == 0) {//ʹ�������߶�������
		pfish->_coord._x = FISH_BOUNDARY_X;
		pfish->_fishdir = _LEFT;
	}
	else {
		pfish->_coord._x = 0;
		pfish->_fishdir = _RIGHT;
	}
	pfish->_coord._y = y;
	pfish->_rang._x = lev * FISH_MAGNIFY;
	pfish->_rang._y = lev * FISH_MAGNIFY;
	pfish->_fishlevel = lev;
	ListPushFront(fishes, pfish);
	return _OK;
}

/* �ƶ���Ⱥ��λ�� */
Status MoveFishSchool() {
	PFISH pfish;
	int k, i, sum;
	sum = fishes->list_data;
	for (i = 0; i < sum; i++) {
		k = rand() % FISH_MAX_LEVEL;
		pfish = ListGetAt(fishes, i);
		pfish->_coord._x += FISH_MOVE_STEP;
		pfish->_coord._y += k* FISH_MOVE_STEP;
	}
	return _OK;
}

/* �ж����Ƿ��γ��� */
void IsFishOut(PLIST plist) {
	//�����γ��磬����ȼ���ΪFISH_LEV_0
	int i;
	PFISH pos;
	for (i = 0; i < ListSize(plist); i++) {
		pos = (PFISH)ListGetAt(plist, i);
		if (pos->_coord._x >= FISH_BOUNDARY_X ||
			pos->_coord._x <= 0 ||
			pos->_coord._y >= FISH_BOUNDARY_Y ||
			pos->_coord._y <= 0) {
			pos->_fishlevel = FISH_LEV_0;
		}
	}
	return;
}

/* ���Ƿ񱻳� */
void IsFishDead(PLIST plist, PFISH player) {
	//�ж�����Ƿ�Ե�������
	//������Ƿ�������Ե�
	int i;
	PFISH pos;
	for (i = 0; i < ListSize(plist); i++) {
		pos = (PFISH)ListGetAt(plist, i);
		if (pos->_coord._x == player->_coord._x &&
			pos->_coord._y == player->_coord._y) {
			if (pos->_fishlevel < player->_fishlevel)
			{//��ҵ÷�
				pos->_fishlevel = FISH_LEV_0;

			}
			else if (pos->_fishlevel > player->_fishlevel) {
				//���С������
				player->_fishlevel = FISH_LEV_0;
			}
		}
	}
	return;


}

/* ���Ƿ�������� */
void FishUpgrade(PFISH player) {
	switch (player->_fishlevel) {
	case FISH_LEV_1:
		if (score > 30)
			player->_fishlevel = FISH_LEV_2;
		break;
	case FISH_LEV_2:
		if (score > 30)
			player->_fishlevel = FISH_LEV_3;
		break;
	case FISH_LEV_3:
		if (score > 30)
			player->_fishlevel = FISH_LEV_4;
		break;

	case FISH_LEV_4:
		if (score > 30)
			player->_fishlevel = FISH_LEV_MAX;
		break;
	default:
		break;
	}
}

/* ɾ����i��С��ڵ� */
void DestroyFishAt(PLIST plist, int i) {
	PFISH pf;
	pf = ListDeleteAt(plist, i);
	free(pf);
}

/* ����״̬ɾ���� */
void DestroyFishByStatus() {
	int i, n;
	PFISH pf;
again:
	n = ListSize(fishes);
	for (i = 0; i < n; i++) {
		pf = (PFISH)ListGetAt(fishes, i);
		if (pf->_fishlevel == FISH_LEV_0) {
			DestroyFishAt(fishes, i);
			goto again;
		}
	}

}

/* ���ص�i���� */
PFISH GetFishAt(int i) {
	return ListGetAt(fishes, i);
}


DWORD getScore() {
	return score;
}

DWORD getLevel() {
	return level;
}

int getFishX(PFISH f) {
	return f->_coord._x;
}

int getFishY(PFISH f) {
	return f->_coord._y;
}

int getFishSize() {
	return ListSize(fishes);
}