#include <windows.h>
#include "Fish.h"
#include "LinkList.h"
#include "ui.h"

/****************** ȫ�ֱ��� *******************/
DWORD score = 0;//���С��ķ���
DWORD level = 2;//���С��ĵȼ�
PLIST fishes; //�洢��Ⱥ�����������С�㣩������
DWORD Score_Update_2 = FISH_SCORE_ADD * FISH_UPGRADE;
DWORD Score_Update_3 = 6* FISH_SCORE_ADD * FISH_UPGRADE;
DWORD Score_Update_4 = 20* FISH_SCORE_ADD * FISH_UPGRADE;
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
	pfish->_fishdir = _RIGHT;
	pfish->_coord.x = 0;
	pfish->_coord.y = y;
	pfish->_fishlevel = lev;
	switch (lev) {
	case FISH_LEV_1:
		pfish->_rang._x = 2 *lev * FISH_MAGNIFY;
		pfish->_rang._y = lev * FISH_MAGNIFY;
		break;
	case FISH_LEV_2:
		pfish->_rang._x =  lev * FISH_MAGNIFY;
		pfish->_rang._y = lev * FISH_MAGNIFY;
		break;
	case FISH_LEV_3:
		pfish->_rang._x =   lev * FISH_MAGNIFY - 10;
		pfish->_rang._y = lev * FISH_MAGNIFY - 10;
		break;
	case FISH_LEV_4:
		pfish->_rang._x = lev * FISH_MAGNIFY;
		pfish->_rang._y = lev * FISH_MAGNIFY;
		break;
	
	}


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
	int x = 0;
	int y = 0;
	switch (getLevel()) {
	case FISH_LEV_2:
	case FISH_LEV_3:
		x = ptPlayer.x;
		y = ptPlayer.y;
		break;
	case FISH_LEV_4:
		x = ptPlayer.x +15 ;
		y = ptPlayer.y + 60;
		break;
	case FISH_LEV_MAX:
		x = ptPlayer.x + 10;
		y = ptPlayer.y + 50;
		break;
	default:
		break;
	}

	
	for (i = 0; i < n; i++) {
		pos = (PFISH)getFishAt(i);
		//�ж�λ��λ��������� 
		switch (pos->_fishlevel) {//���²������ģ�����������
		case FISH_LEV_1:
			if (pos->_coord.x + pos->_rang._x / 4 < x &&
				pos->_coord.x + pos->_rang._x > x  &&
				pos->_coord.y  < y + PLAYER_HEIGHT / 2 &&
				pos->_coord.y + pos->_rang._y > y + PLAYER_HEIGHT / 2) {
				if (pos->_fishlevel < getLevel())
				{//��ҵ÷�
				 //���ű�������
					CreateThread(NULL, 0, MUSIC_PLAY_FISH1, NULL, 0, NULL);
					score += FISH_SCORE_ADD * pos->_fishlevel;
					pos->_fishlevel = FISH_LEV_0;
				}
				else if (pos->_fishlevel > getLevel()) {
					//���С������
					setLevel(FISH_LEV_0);
					return 1;
				}
			}
			break;

		case FISH_LEV_2:
			if (pos->_coord.x + pos->_rang._x / 3 < x  &&
				pos->_coord.x + pos->_rang._x > x  &&
				pos->_coord.y  < y + PLAYER_HEIGHT / 2 &&
				pos->_coord.y + pos->_rang._y > y + PLAYER_HEIGHT / 2) {
				if (pos->_fishlevel < getLevel())
				{//��ҵ÷�
					CreateThread(NULL, 0, MUSIC_PLAY_FISH2, NULL, 0, NULL);
					score += FISH_SCORE_ADD * pos->_fishlevel;
					pos->_fishlevel = FISH_LEV_0;
				}
				else if (pos->_fishlevel > getLevel()) {
					//���С������
					setLevel(FISH_LEV_0);
					return 1;
				}
			}
			break;

		case FISH_LEV_3:
			if (pos->_coord.x + pos->_rang._x / 6 < x  &&
				pos->_coord.x + pos->_rang._x - pos->_rang._x / 6 > x  &&
				pos->_coord.y + pos->_rang._y / 4 < y + PLAYER_HEIGHT / 2 &&
				pos->_coord.y + pos->_rang._y - pos->_rang._y / 4 > y + PLAYER_HEIGHT / 2) {
				if (pos->_fishlevel < getLevel())
				{//��ҵ÷�
					CreateThread(NULL, 0, MUSIC_PLAY_FISH3, NULL, 0, NULL);
					score += FISH_SCORE_ADD * pos->_fishlevel;
					pos->_fishlevel = FISH_LEV_0;
				}
				else if (pos->_fishlevel > getLevel()) {
					//���С������
					setLevel(FISH_LEV_0);
					return 1;
				}
			}
			break;

		case FISH_LEV_4:
			if (pos->_coord.x + pos->_rang._x / 8 < x  &&
				pos->_coord.x + pos->_rang._x - pos->_rang._x / 8 > x  &&
				pos->_coord.y + pos->_rang._y / 6 < y + PLAYER_HEIGHT / 2 &&
				pos->_coord.y + pos->_rang._y - pos->_rang._y / 6 > y + PLAYER_HEIGHT / 2) {
				if (pos->_fishlevel < getLevel())
				{//��ҵ÷�
					CreateThread(NULL, 0, MUSIC_PLAY_FISH4, NULL, 0, NULL);
					score += FISH_SCORE_ADD * pos->_fishlevel;
					pos->_fishlevel = FISH_LEV_0;
				}
				else if (pos->_fishlevel > getLevel()) {
					//���С������
					setLevel(FISH_LEV_0);
					return 1;
				}
			}
			break;
		}//endswitch

	/*	if (getLevel() == FISH_MAX_LEVEL) {
			if (pos->_coord.x + pos->_coord.x > ptPlayer.x &&
				pos->_coord.x + pos->_coord.x < ptPlayer.x + PLAYER_WIDTH &&
				pos->_coord.y  < ptPlayer.y + PLAYER_HEIGHT  &&
				pos->_coord.y + pos->_rang._y >ptPlayer.y) {
				CreateThread(NULL, 0, MUSIC_PLAY_FISH2, NULL, 0, NULL);
				score += FISH_SCORE_ADD * pos->_fishlevel;
				pos->_fishlevel = FISH_LEV_0;
			}
		}*/

	}//endfor

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
		if (score >  Score_Update_2)
			setLevel(FISH_LEV_3);
		break;
	case FISH_LEV_3:
		if (score > Score_Update_3)
			setLevel(FISH_LEV_4);
		break;

	case FISH_LEV_4:
		if (score > Score_Update_4)
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