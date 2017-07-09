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

//���״̬
#define FISH_MOVED 0
#define FISH_EAT_FISH 1
#define FISH_BE_EATEN 2
#define FISH_GROWUP 3
#define FISH_SUCCEED 4
#define FISH_ERROR 5

//����
typedef struct _fish_coord {
	int _x;
	int _y;
}_COORD;

//����ƶ�����
typedef enum fish_move_dir {
	_UP,
	_DOWN,
	_LEFT,
	_RIGHT
}FISH_DIRECTION;

//��ĵȼ�
typedef enum fish_status {
	FISH_LEV_0,//0����ʾ����
	FISH_LEV_1,
	FISH_LEV_2,
	FISH_LEV_3,
	FISH_LEV_4,
	FISH_LEV_MAX
}FISH_LEVEL;

//��Ľṹ
typedef struct _fish {
	_COORD _coord;
	_COORD _rang;
	FISH_LEVEL _fishlevel;
	FISH_DIRECTION _fishdir;
}FISH, *PFISH;

/*********** �������� **************/

/* ��ʼ����Ⱥ */
int FishInit();

/* ������Ⱥ */
PLIST CreateFishSchool();

/* ����һ���� */
Status CreateFish();

/* �ƶ���Ⱥ��λ�� */
Status MoveFishSchool();

/* �ж����Ƿ��γ��� */
void IsFishOut(PLIST plist);

/* ���Ƿ񱻳� */
void IsFishDead(PLIST plist, PFISH player);

/* ���Ƿ�������� */
void FishUpgrade(PFISH player);

/* ����״̬ɾ���� */
void DestroyFishByStatus();

/* ���ص�i���� */
PFISH GetFishAt(int i);

int getFishX(PFISH f);

int getFishY(PFISH f);

int getFishSize();

DWORD getScore();

DWORD getLevel();
#endif // !FISH_H