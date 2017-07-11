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
	POINT _coord;
	_COORD _rang;
	FISH_LEVEL _fishlevel;
	FISH_DIRECTION _fishdir;
}FISH, *PFISH;

/*********** �������� **************/

/* ��ʼ����Ⱥ */
int FishInit();

/* ����һ���� */
Status CreateFish();

/* �ƶ���Ⱥ��λ�� */
Status MoveFishSchool();

/* �ж����Ƿ��γ��� */
void IsFishOut();

/* ���Ƿ񱻳� */
BOOL IsFishDead();

/* ���Ƿ�������� */
void FishUpgrade();

/* ����״̬ɾ���� */
void DestroyFishByState();

/* ���ص�i���� */
PFISH getFishAt(int i);

/* ������Ⱥ������ */
int getFishSize();

/* �������С��ķ��� */
DWORD getScore();

/* �������С��ķ��� */
void setScore(DWORD s);

/* �������С��ĵȼ� */
DWORD getLevel();

/* �������С��ĵȼ� */
void setLevel(FISH_LEVEL i);

#endif // !FISH_H