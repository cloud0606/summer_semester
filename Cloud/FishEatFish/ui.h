#pragma once
#ifndef  GAME_H
#define GAME_H
#include <Windows.h>

/****************** �궨�� *******************/

#define TRANS_BK_COLOR   RGB(135,206,250)
#define BITMAP_FILE_BK   "BK.bmp" //����ͼ
#define BITMAP_FILE_PLAYER2  "PLAYER2.bmp" //���С��2��ͼ�ļ���
#define BITMAP_FILE_PLAYER3  "PLAYER3.bmp" //���С��3��ͼ�ļ���
#define BITMAP_FILE_PLAYER4  "PLAYER4.bmp" //���С��4��ͼ�ļ���
#define BITMAP_FILE_PLAYER5  "PLAYER5.bmp" //���С���޵�ͼ�ļ���
#define BITMAP_FILE_FISH_1  "FISH_1.bmp" //�ȼ�1С���ļ���
#define BITMAP_FILE_FISH_2  "FISH_2.bmp" //�ȼ�2С���ļ���
#define BITMAP_FILE_FISH_3  "FISH_3.bmp" //�ȼ�3С���ļ���
#define BITMAP_FILE_FISH_4  "FISH_4.bmp" //�ȼ�4С���ļ���
#define TIMER_ID	12340

#define CLASS_NAME_BK  "bk_class"
#define CLASS_NAME_PLAYER  "player_class"

//��Ⱥ�Ĳ���
#define FISH_MOVE_STEP  3  //��Ⱥ����������ң����ƶ�����
#define FISH_MAGNIFY  25   //������Ⱥ����������ң��ķŴ���
#define FISH_MAX_LEVEL 4  //��Ⱥ�����ȼ�
#define FISH_MOVE_STEP_PLAYER 20 //���С����ƶ�����
#define FISH_SCORE_ADD 100 //���óԵ�С���÷ֵķ���
#define FISH_DEAD_CONTROL_X 0//�������С�������Ѷ�
#define FISH_DEAD_CONTROL_Y 0//�������С�������Ѷ�
#define FISH_UPGRADE 1 //�������С���������Ѷ�
//��Ϸ�߽�
#define FISH_BOUNDARY_X  800
#define FISH_BOUNDARY_Y  500

//���С��Ŀ�͸�
#define PLAYER_WIDTH 60
#define PLAYER_HEIGHT 40

/****************** ȫ�ֱ��� *******************/
//���С�������
POINT ptPlayer;

/***************** �������� ********************/

/* ����������غ��� */
LONG CALLBACK BackGroundWindowProc(
	HWND hwnd, //
	UINT msg, // ��Ϣ
	WPARAM wParam, // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
	LPARAM lParam);

BOOL BackGroundWindwowRegister(HINSTANCE hinstance);

HWND BackgroundWindowCreate(HINSTANCE hinstance);

LONG BackgroundCreate(HWND hwnd);

LONG BackgroundPaint(HWND hwnd);

LONG BackgroundResizeAndMove(HWND hwnd, LPRECT lpRect);

LONG OnKeydown(HWND hwnd, UINT vk);

/* ��ң���Ⱥ����ң�������غ��� */
LONG CALLBACK PlayerWindowProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam);

HWND PlayerWindowCreate(HINSTANCE hinstance);

BOOL PlayerWindowRegister(HINSTANCE hinstance);

LONG PlayerCreate(HWND hwnd);

LONG PlayerWindowPaint(HWND hwnd);

LONG PlayerTimer(HWND hwnd);



#endif // ! GAME_H
#pragma once
