#include <windows.h>
#include "Fish.h"
#include "LinkList.h"
#include "ui.h"

/****************** ȫ�ֱ��� *******************/
DWORD score = 0;//���С��ķ���
DWORD level = 2;//���С��ĵȼ�
PLIST fishes; //�洢��Ⱥ�����������С�㣩������

/****************** �������� *******************/

/* ��ʼ����Ⱥ */
int FishInit()
{
	time_t t;
	fishes = ListCreate(0);
	srand((unsigned int)(time(&t)*time(&t)));
	return 0;
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

/* �ƶ���Ⱥ��λ�� */
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

/* �ж����Ƿ��γ��� */
void IsFishOut() {
	//�����γ��磬����ȼ���ΪFISH_LEV_0
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

/* ���Ƿ񱻳� */
BOOL IsFishDead() {
	//�ж�����Ƿ�Ե������� �Ե���ӷ�
	//������Ƿ�������Ե� 
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
	//		{//��ҵ÷�
	//			score += FISH_SCORE_ADD * pos->_fishlevel;
	//			pos->_fishlevel = FISH_LEV_0;
	//		}
	//		else if (pos->_fishlevel > getLevel()) {
	//			//���С������
	//			setLevel(FISH_LEV_0);
	//			return 1;
	//		}
	//	}
	//}

	for (i = 0; i < n; i++) {
		pos = (PFISH)getFishAt(i);
		//�ж�λ��λ���������
		if (pos->_coord.x + FISH_DEAD_CONTROL_X < ptPlayer.x  &&
			pos->_coord.x + pos->_rang._x - FISH_DEAD_CONTROL_X > ptPlayer.x  &&
			pos->_coord.y + FISH_DEAD_CONTROL_Y < ptPlayer.y + PLAYER_HEIGHT / 2 &&
			pos->_coord.y + pos->_rang._y - FISH_DEAD_CONTROL_Y > ptPlayer.y + PLAYER_HEIGHT / 2) {
			if (pos->_fishlevel < getLevel())
			{//��ҵ÷�
				score += FISH_SCORE_ADD * pos->_fishlevel;
				pos->_fishlevel = FISH_LEV_0;
			}
			else if (pos->_fishlevel > getLevel()) {
				//���С������
				setLevel(FISH_LEV_0);
				return 1;
			}
		}
	}
		return 0;

    
}

/* ���Ƿ�������� */
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

/* ɾ����i��С��ڵ� */
void DestroyFishAt(PLIST plist, int i) {
	PFISH pf;
	pf = ListDeleteAt(plist, i);
	free(pf);
}

/* ����״̬ɾ���� */
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

/* ���ص�i���� */
PFISH getFishAt(int i) {
	return ListGetAt(fishes, i);
}

/* �������С��ĵȼ� */
DWORD getLevel() {
	return level;
}

/* �������С��ĵȼ� */
void setLevel(FISH_LEVEL i) {
	level = i;
}

/* �������С��ķ��� */
DWORD getScore() {
	return score; 
}

/* �������С��ķ��� */
void setScore(DWORD s) {
	score = s;
}

/* ������Ⱥ������ */
int getFishSize() {
	return ListSize(fishes);
}