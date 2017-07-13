#include <windows.h>
//#include <wingdi.h>
#include "Fish.h"
#include "ui.h"
#include "LinkLIst.h"

#pragma comment (lib, "Winmm.lib")
#pragma comment(lib,"Msimg32.lib")//transparentblt
#pragma  comment(lib,"WinMM.Lib")//��������

/****************** ȫ�ֱ��� *******************/

DWORD dwTimerElapse = 20;//ʱ����
HWND hwndBackground;
HWND hwndPlayer;
HINSTANCE hinst;
HBITMAP hbmpPlayer_2;
HBITMAP hbmpPlayer_3;
HBITMAP hbmpPlayer_4;
HBITMAP hbmpPlayer_MAX;
HBITMAP hbmpBackground;
HBITMAP hbmpFish_1;
HBITMAP hbmpFish_2;
HBITMAP hbmpFish_3;
HBITMAP hbmpFish_4;


DWORD WINAPI MUSIC_PLAY_BK(LPVOID lpParam) {
	switch (BK_MUSIC) {
	case 1:
		PlaySound(TEXT("���㹲��.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case 2:
		PlaySound(TEXT("�ɰ�����������.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_SYNC);
		break;
	case 3:
		PlaySound(TEXT("����.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	default:
		break;
	}
}

DWORD WINAPI MUSIC_PLAY_DEAD(LPVOID lpParam) {

	mciSendString("close fish", 0, 0, 0);
	mciSendStringA(TEXT("open dead.wav alias fish"), NULL, 0, NULL);
	mciSendString("play fish", 0, 0, 0);
	return 0;
}

DWORD WINAPI MUSIC_PLAY_FISH1(LPVOID lpParam) {

	mciSendString("close fish", 0, 0, 0);
	mciSendStringA(TEXT("open fish1.wav alias fish"), NULL, 0, NULL);
	mciSendString("play fish", 0, 0, 0);
	return 0;
}

DWORD WINAPI MUSIC_PLAY_FISH2(LPVOID lpParam) {

	mciSendString("close fish", 0, 0, 0);
	mciSendStringA(TEXT("open fish2.wav alias fish"), NULL, 0, NULL);
	mciSendString("play fish", 0, 0, 0);
	return 0;
}

DWORD WINAPI MUSIC_PLAY_FISH3(LPVOID lpParam) {

	mciSendString("close fish", 0, 0, 0);
	mciSendStringA(TEXT("open fish3.wav alias fish"), NULL, 0, NULL);
	mciSendString("play fish", 0, 0, 0);
	return 0;
}

DWORD WINAPI MUSIC_PLAY_FISH4(LPVOID lpParam) {

	mciSendString("close fish4", 0, 0, 0);
	mciSendStringA(TEXT("open fish4.wav alias fish"), NULL, 0, NULL);
	mciSendString("play fish", 0, 0, 0);
	return 0;
}

BOOL BackGroundWindwowRegister(HINSTANCE hinstance)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = BackGroundWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME_BK;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "��������classʧ��", "����", MB_ICONERROR | MB_OK);
		return FALSE;
	}
	return TRUE;
}

HWND BackgroundWindowCreate(HINSTANCE hinstance)
{

	HWND hwnd;
	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_LAYERED | WS_EX_TOPMOST,
		CLASS_NAME_BK,			// �����������������Ѿ�ע���˵Ĵ�����
		"",		// title-bar string 
		WS_POPUP,	// ���ڵ�style�������ʾΪtop-level window 
		CW_USEDEFAULT,			// ����ˮƽλ��default horizontal POINT 
		CW_USEDEFAULT,			// ���ڴ�ֱλ��default vertical POINT 
		CW_USEDEFAULT,			// ���ڿ�� default width 
		CW_USEDEFAULT,			// ���ڸ߶� default height 
		(HWND)NULL,				// �����ھ�� no owner window 
		(HMENU)NULL,			// ���ڲ˵��ľ�� use class menu 
		hinstance,				// Ӧ�ó���ʵ����� handle to application instance 
		(LPVOID)NULL);			// ָ�򸽼����ݵ�ָ�� no window-creation data 

	if (!hwnd) {
		MessageBox(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return NULL;
	}

	if (!SetLayeredWindowAttributes(
		hwnd, TRANS_BK_COLOR,
		0,//���ñ���͸����
		LWA_COLORKEY)) {//LWA_ALPHAʱ,LWA_COLORKEY,LWA_ALPHA | LWA_COLORKEY
		DWORD dwError = GetLastError();
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	return hwnd;

}

LONG BackgroundCreate(HWND hwnd)
{
	hbmpBackground = LoadImage(NULL, BITMAP_FILE_BK,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpBackground == NULL) {
		MessageBox(hwnd, "bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	MoveWindow(hwnd, 100, 20, FISH_BOUNDARY_X, FISH_BOUNDARY_Y, TRUE);

	ptPlayer.x = FISH_BOUNDARY_X / 2;
	ptPlayer.y = FISH_BOUNDARY_Y / 2;

	SetFocus(hwnd);

	return 0;
}

LONG BackgroundPaint(HWND hwnd)
{
	HDC hdc, hdcMem;
	RECT rect;

	BITMAP bmp;
	//���ߴ��ڵ�λ�ã����ڻ�����ͼ
	GetClientRect(hwnd, &rect);

	hdc = GetDC(hwnd);
	hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hbmpBackground);
	GetObject(hbmpBackground, sizeof(BITMAP), &bmp);

	StretchBlt(hdc,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top ,
		hdcMem,
		0, 0,
		bmp.bmWidth, bmp.bmHeight,
		SRCCOPY);

	DeleteObject(hdcMem);
	DeleteDC(hdcMem);
	ReleaseDC(hwnd, hdc);
	DeleteDC(hdc);
	return 0;
}

LONG BackgroundResizeAndMove(HWND hwnd, LPRECT lpRect)
{

	SetWindowPos(hwndPlayer,
		HWND_TOPMOST,/*�ô���λ���������д��ڵĶ��� HWND_BOTTOM�����۾����(�ײ�)*/
		lpRect->left , lpRect->top ,//���ô��ڵ�horizontal,verticalλ��
		lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, //���ô���width��height
		SWP_SHOWWINDOW);//��ʾ����

	//SetFocus(hwndBackground);

	return 0;
}

LONG OnKeydown(HWND hwnd, UINT vk)
{
	UINT key = vk;
	switch (key)
	{
	case VK_LEFT:
		ptPlayer.x -= FISH_MOVE_STEP_PLAYER;
		if (ptPlayer.x < 0)
			ptPlayer.x = 0;
		PlayerWindowPaint(hwndPlayer);
		break;
	case VK_RIGHT:
		ptPlayer.x += FISH_MOVE_STEP_PLAYER;
		if (ptPlayer.x + PLAYER_WIDTH > FISH_BOUNDARY_X)
			ptPlayer.x = FISH_BOUNDARY_X - PLAYER_WIDTH;
		PlayerWindowPaint(hwndPlayer);
		break;
	case VK_UP:
		ptPlayer.y -= FISH_MOVE_STEP_PLAYER;
		if (ptPlayer.y < 0)
			ptPlayer.y = 0;
		PlayerWindowPaint(hwndPlayer);
		break;
	case VK_DOWN:
		ptPlayer.y += FISH_MOVE_STEP_PLAYER;
		if (ptPlayer.y + PLAYER_HEIGHT > FISH_BOUNDARY_Y)
			ptPlayer.y = FISH_BOUNDARY_Y - PLAYER_HEIGHT;
		PlayerWindowPaint(hwndPlayer);
		break;
	default:
		break;
	}
	return 0;
}


LONG CALLBACK BackGroundWindowProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	RECT rectNew;
	switch (msg)
	{
	case WM_CREATE:
		BackgroundCreate(hwnd);
		SetFocus(hwnd);
		/*PostMessage(hwnd, WM_LBUTTONDOWN, 0, (DWORD)0x0050050);
		PostMessage(hwnd, WM_LBUTTONUP, 0, (DWORD)0x0050050);*/
		break;
	case WM_PAINT:
		BackgroundPaint(hwnd);
		break;

	case WM_MOVING:
		BackgroundResizeAndMove(hwnd, (LPRECT)lParam);
		break;

	case WM_KILLFOCUS:
		KillTimer(hwndPlayer, TIMER_ID);
		break;

	case WM_SETFOCUS:
		SetTimer(hwndPlayer, TIMER_ID, dwTimerElapse, NULL);
		break;

	case WM_SIZE:
		GetWindowRect(hwnd, &rectNew);
		BackgroundResizeAndMove(hwnd, &rectNew);
		break;

	case WM_KEYDOWN:
		OnKeydown(hwnd, (UINT)wParam);
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_TIMER:
		break;

	case WM_DESTROY:
		ExitProcess(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}

BOOL PlayerWindowRegister(HINSTANCE hinstance)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = PlayerWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(TRANS_BK_COLOR);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME_PLAYER;
	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "��������classʧ��", "����", MB_ICONERROR | MB_OK);
		return FALSE;
	}
	return TRUE;
}

HWND PlayerWindowCreate(HINSTANCE hinstance)
{
	HWND hwnd;
	hwnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE,
		CLASS_NAME_PLAYER,			// �����������������Ѿ�ע���˵Ĵ�����
		"",			// title-bar string 
		WS_POPUP,	// ���ڵ�style�������ʾΪtop-level window 
		CW_USEDEFAULT,			// ����ˮƽλ��default horizontal POINT 
		CW_USEDEFAULT,			// ���ڴ�ֱλ��default vertical POINT 
		CW_USEDEFAULT,			// ���ڿ�� default width 
		CW_USEDEFAULT,			// ���ڸ߶� default height 
		(HWND)NULL,				// �����ھ�� no owner window 
		(HMENU)NULL,			// ���ڲ˵��ľ�� use class menu 
		hinstance,				// Ӧ�ó���ʵ����� handle to application instance 
		(LPVOID)NULL);			// ָ�򸽼����ݵ�ָ�� no window-creation data 
	if (!hwnd) {
		GetLastError();
		MessageBox(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return NULL;
	}

	if (!SetLayeredWindowAttributes(
		hwnd, TRANS_BK_COLOR,
		255,
		LWA_COLORKEY))
	{
		DWORD dwError = GetLastError();
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	return hwnd;
}

LONG PlayerWindowPaint(HWND hwnd)
{
	int num, i;

	//�㣨��ң�
	HDC hdc;
	HDC hdcMem_BK;
	HDC hdcMem_FISH;
	HBITMAP hBitmap;
	BITMAP bmp;
	//�㣨������
	PFISH fish;
	//����
	RECT rect_BK;
	//����
	HFONT hFont;
	CHAR debug_info[100];

	//��ȡ���ڴ�С
    GetClientRect(hwnd, &rect_BK);
	//�㣨��ң�
	hdc = GetDC(hwnd);
	hdcMem_BK = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, // ������hdcMem��������ɺڰ�2ɫλͼ
	rect_BK.right - rect_BK.left, rect_BK.bottom - rect_BK.top);
	
	///****************  ������  ********************/
	SelectObject(hdcMem_BK, hBitmap);
	FillRect(hdcMem_BK, &rect_BK, (HBRUSH)GetStockObject(WHITE_BRUSH));//��������Ӱ

	/****************   �������   ********************/
	hdcMem_FISH = CreateCompatibleDC(hdc);
	//ѡ��hbmpPlayerλͼΪ����dc�Ŀɼ�����
	switch (getLevel()) {
	case FISH_LEV_2:
		SelectObject(hdcMem_FISH, hbmpPlayer_2);
		GetObject(hbmpPlayer_2, sizeof(BITMAP), &bmp);
		TransparentBlt(hdcMem_BK,
			ptPlayer.x, ptPlayer.y,
			PLAYER_WIDTH, PLAYER_HEIGHT,
			hdcMem_FISH,
			0, 0, bmp.bmWidth, bmp.bmHeight,
			RGB(255, 255, 255));
		break;

	case FISH_LEV_3:
		SelectObject(hdcMem_FISH, hbmpPlayer_3);
		GetObject(hbmpPlayer_3, sizeof(BITMAP), &bmp);
		TransparentBlt(hdcMem_BK,
			ptPlayer.x, ptPlayer.y,
			PLAYER_WIDTH + 20, PLAYER_HEIGHT + 20,
			hdcMem_FISH,
			0, 0, bmp.bmWidth, bmp.bmHeight,
			RGB(255, 255, 255));
		break;

	case FISH_LEV_4:
		SelectObject(hdcMem_FISH, hbmpPlayer_4);
		GetObject(hbmpPlayer_4, sizeof(BITMAP), &bmp);
		TransparentBlt(hdcMem_BK,
			ptPlayer.x, ptPlayer.y,
			PLAYER_WIDTH + 40, PLAYER_HEIGHT + 40,
			hdcMem_FISH,
			0, 0, bmp.bmWidth, bmp.bmHeight,
			RGB(255, 255, 255));
		break;


	case FISH_LEV_MAX:
		SelectObject(hdcMem_FISH, hbmpPlayer_MAX);
		GetObject(hbmpPlayer_MAX, sizeof(BITMAP), &bmp);
		TransparentBlt(hdcMem_BK,
			ptPlayer.x, ptPlayer.y,
			PLAYER_WIDTH + 80, PLAYER_HEIGHT + 80,
			hdcMem_FISH,
			0, 0, bmp.bmWidth, bmp.bmHeight,
			RGB(255, 255, 255));
		break;
	default:
		break;
	}
	;

	/****************   ����   ********************/
	num = getFishSize();

	for (i = 0; i < num; i++) {
		fish = getFishAt(i);
		switch (fish->_fishlevel) {
		case FISH_LEV_1:
			SelectObject(hdcMem_FISH, hbmpFish_1);
			GetObject(hbmpFish_1, sizeof(BITMAP), &bmp);
			//͸����ͼ
			TransparentBlt(hdcMem_BK,
				fish->_coord.x, fish->_coord.y,
				fish->_rang._x, fish->_rang._y,
				hdcMem_FISH,
				0, 0, bmp.bmWidth, bmp.bmHeight,
				RGB(255, 255, 255));
			break;

		case FISH_LEV_2:
			SelectObject(hdcMem_FISH, hbmpFish_2);
			GetObject(hbmpFish_2, sizeof(BITMAP), &bmp);
			TransparentBlt(hdcMem_BK,
				fish->_coord.x, fish->_coord.y,
				fish->_rang._x, fish->_rang._y,
				hdcMem_FISH,
				0, 0, bmp.bmWidth, bmp.bmHeight,
				RGB(255, 255, 255));
			break;

		case FISH_LEV_3:
			SelectObject(hdcMem_FISH, hbmpFish_3);
			GetObject(hbmpFish_3, sizeof(BITMAP), &bmp);
			TransparentBlt(hdcMem_BK,
				fish->_coord.x, fish->_coord.y,
				fish->_rang._x, fish->_rang._y,
				hdcMem_FISH,
				0, 0, bmp.bmWidth, bmp.bmHeight,
				RGB(255, 255, 255));
			break;

		case FISH_LEV_4:
			SelectObject(hdcMem_FISH, hbmpFish_4);
			GetObject(hbmpFish_4, sizeof(BITMAP), &bmp);
			TransparentBlt(hdcMem_BK,
				fish->_coord.x, fish->_coord.y,
				fish->_rang._x, fish->_rang._y,
				hdcMem_FISH,
				0, 0, bmp.bmWidth, bmp.bmHeight,
				RGB(255, 255, 255));
			break;
			default:
				break;
		}
	}

	// ������һ���������
	hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, 0, FALSE, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("��Բ"));


	wsprintf(debug_info, "��Ұ��һ����%d\n���� �÷֣�0x%p ����:%d", num, getScore(),getLevel());
	SelectObject(hdcMem_BK, hFont);
	SetTextColor(hdcMem_BK, RGB(205,92,92));
	TextOut(hdcMem_BK, 10, 10, debug_info, strlen(debug_info));


	// ������DC
	BitBlt(hdc,
		rect_BK.left, rect_BK.top,
		rect_BK.right - rect_BK.left, rect_BK.bottom - rect_BK.top,
		hdcMem_BK,
		0, 0,
		SRCCOPY);

	DeleteObject(hFont);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem_FISH);
	DeleteDC(hdcMem_BK);
	ReleaseDC(hwnd, hdc);
	DeleteDC(hdc);
	return 0;
}


LONG PlayerCreate(HWND hwnd)
{
	//���ű�������
	CreateThread(NULL, 0, MUSIC_PLAY_BK, NULL, 0, NULL);

   hbmpPlayer_2 = LoadImage(NULL, BITMAP_FILE_PLAYER2,
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpPlayer_2 == NULL)
	{
		MessageBox(hwnd, "player bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	hbmpPlayer_3 = LoadImage(NULL, BITMAP_FILE_PLAYER3,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpPlayer_3 == NULL)
	{
		MessageBox(hwnd, "player bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	hbmpPlayer_4 = LoadImage(NULL, BITMAP_FILE_PLAYER4,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpPlayer_4 == NULL)
	{
		MessageBox(hwnd, "player bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	hbmpPlayer_MAX = LoadImage(NULL, BITMAP_FILE_PLAYER5,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpPlayer_MAX == NULL)
	{
		MessageBox(hwnd, "player bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	hbmpFish_1 = LoadImage(NULL, BITMAP_FILE_FISH_1,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpFish_1 == NULL)
	{
		MessageBox(hwnd, "f1 bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	hbmpFish_2 = LoadImage(NULL, BITMAP_FILE_FISH_2,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpFish_2 == NULL)
	{
		MessageBox(hwnd, "f2 bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	hbmpFish_3 = LoadImage(NULL, BITMAP_FILE_FISH_3,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpFish_3 == NULL)
	{
		MessageBox(hwnd, "f3 bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	hbmpFish_4 = LoadImage(NULL, BITMAP_FILE_FISH_4,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpFish_4 == NULL)
	{
		MessageBox(hwnd, "f4 bmp file not find", "ERROR!",
			MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	FishInit();

	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);

	return 0;

}

LONG PlayerTimer(HWND hwnd)
{
	MoveFishSchool();

	IsFishOut();

	if (IsFishDead()) {
		KillTimer(hwnd, TIMER_ID);
		CreateThread(NULL, 0, MUSIC_PLAY_DEAD, NULL, 0, NULL);
		MessageBox(hwnd, "���ģ�С�㱻�Ե���~~~", "ooooo..", MB_OK);
		ExitProcess(0);
		DestroyFishByState();
		return 0;
	}

	FishUpgrade();

	DestroyFishByState();
	// 1.5%�ĸ��ʣ����������
	
	if (rand() % 100 < FISH_CREAT_RATE) {
		CreateFish();
	}
	return 0;
}

LONG CALLBACK PlayerWindowProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		PlayerCreate(hwnd);

	case WM_PAINT:
		//BackgroundPaint(hwnd);
		PlayerWindowPaint(hwnd);
		break;

	case WM_KEYDOWN:
		//����
		OnKeydown(hwnd, (UINT)wParam);
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_TIMER:
		PlayerTimer(hwnd);
		PlayerWindowPaint(hwnd);
		break;

	case WM_DESTROY:
		ExitProcess(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}



int WINAPI WinMain(
	HINSTANCE hinstance, // ����ʵ��������ڳ������У����̴������ɲ���ϵͳ��Ӧ�ó�����
	HINSTANCE hPrevInstance, // �����̵ĳ���ʵ�����
	LPSTR lpCmdLine,  // �����в�������λ����������C����main��������argc��argv,����û�а��ո�����з�
	int nCmdShow)   // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ�����
{
	BOOL fGotMessage;
	MSG msg;

	if (PlayerWindowRegister(hinst))
		hwndPlayer = PlayerWindowCreate(hinst);

	if (BackGroundWindwowRegister(hinstance))
		hwndBackground = BackgroundWindowCreate(hinstance);
	else
		return 0;


	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
		&& fGotMessage != -1) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}