#include <windows.h>
#include "Fish.h"
#include "Game.h"
#include "LinkLIst.h"

#pragma comment (lib, "Winmm.lib")

#define CLASS_NAME_BK  "bk_class"
#define CLASS_NAME_PLAYER  "player_class"

#define TRANS_BK_COLOR   RGB(135,206,250)
#define BITMAP_FILE_BK   "BK.bmp"
#define BITMAP_FILE_PLAYER  "PLAYER.bmp"
#define BITMAP_FILE_FISH_1  "FISH_1.bmp"
#define BITMAP_FILE_FISH_2  "FISH_2.bmp"
#define BITMAP_FILE_FISH_3  "FISH_3.bmp"
#define BITMAP_FILE_FISH_4  "FISH_4.bmp"

#define TIMER_ID	12340 

#define COLOR_FISH  RGB(135,206,250)
#define COLOR_FISH_DEAD  RGB(255,48,48)



//全局变量

DWORD dwTimerElapse = 40;

HWND hwndBackground;
HWND hwndPlayer;
HINSTANCE hinst;


HBITMAP hbmpPlayer;
HBITMAP hbmpBackground;
HBITMAP hbmpFish_1;
HBITMAP hbmpFish_2;
HBITMAP hbmpFish_3;
HBITMAP hbmpFish_4;

BOOL BackGroundWindwowRegister(HINSTANCE hinstance)
{
	WNDCLASS wc;
	// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	// 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	wc.lpfnWndProc = BackGroundWindowProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon成员用来指定窗口的图标
	// 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
	wc.hIcon = NULL;
	// Cursor是鼠标光标，这里是设定了鼠标光标的样式。
	// 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);
	// GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
	// 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// 窗口的菜单的资源名。
	wc.lpszMenuName = NULL;
	// 给窗口类起一个名字，在创建窗口时需要这个名字。
	wc.lpszClassName = CLASS_NAME_BK;

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return FALSE;
	}
	// 窗口注册成功，继续运行。
	return TRUE;
}

HWND BackgroundWindowCreate(HINSTANCE hinstance)
{

	// Create the main window. 
	HWND hwnd;
	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_LAYERED | WS_EX_TOPMOST,
		CLASS_NAME_BK,			// 窗口类名，必须是已经注册了的窗口类
		"",		// title-bar string 
		WS_POPUP,	// 窗口的style，这个表示为top-level window 
		CW_USEDEFAULT,			// 窗口水平位置default horizontal POINT 
		CW_USEDEFAULT,			// 窗口垂直位置default vertical POINT 
		CW_USEDEFAULT,			// 窗口宽度 default width 
		CW_USEDEFAULT,			// 窗口高度 default height 
		(HWND)NULL,				// 父窗口句柄 no owner window 
		(HMENU)NULL,			// 窗口菜单的句柄 use class menu 
		hinstance,				// 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);			// 指向附加数据的指针 no window-creation data 

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return NULL;
	}

	if (!SetLayeredWindowAttributes(
		hwnd, TRANS_BK_COLOR,
		150, LWA_ALPHA))//设置背景透明度
	{
		DWORD dwError = GetLastError();
	}

	// 窗口创建成功，继续运行。

	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwnd, SW_SHOW);

	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwnd);
	return hwnd;

}

LONG BackgroundCreate(HWND hwnd)
{

	if (PlayerWindowRegister(hinst))
		hwndPlayer = PlayerWindowCreate(hinst);

	//if (AutoflysWindowRegister(hinst))
	//	hwndAutoflys = AutoflyWindowCreate(hinst);

	//BITMAP bmp;背景图
	hbmpBackground = LoadImage(NULL, BITMAP_FILE_BK,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpBackground == NULL)
	{
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
	//PAINTSTRUCT ps;
	RECT rect;

	BITMAP bmp;

	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);
	// Create a memory device compatible with the above DC variable
	hdcMem = CreateCompatibleDC(hdc);

	SelectObject(hdcMem, hbmpBackground);

	GetObject(hbmpBackground, sizeof(BITMAP), &bmp);

	StretchBlt(hdc,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
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

	SetWindowPos(hwndPlayer, HWND_TOPMOST, lpRect->left, lpRect->top,
		lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, SWP_SHOWWINDOW);

	//SetWindowPos(hwndFighter, HWND_TOPMOST,
	//	(lpRect->left + lpRect->right - FIGHTER_WIDTH) / 2,
	//	lpRect->bottom - FIGHTER_HEIGHT,
	//	FIGHTER_WIDTH,
	//	FIGHTER_HEIGHT,
	//	SWP_SHOWWINDOW);

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
		// 其他键不响应
		break;
	}
	return 0;
}


LONG CALLBACK BackGroundWindowProc(
	HWND hwnd, //
	UINT msg, // 消息
	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam) // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
{
	RECT rectNew;
	// 注意，是switch-case, 每次这个函数被调用，只会落入到一个case中。
	switch (msg)
	{
		// 当窗口被创建时，收到的第一个消息就是WM_CREATE，
		// 一般收到这个消息处理过程中，可以用来进行一些初始化的工作
	case WM_CREATE:
		BackgroundCreate(hwnd);
		SetFocus(hwnd);
		PostMessage(hwnd, WM_LBUTTONDOWN, 0, (DWORD)0x0050050);
		PostMessage(hwnd, WM_LBUTTONUP, 0, (DWORD)0x0050050);

		break;

		// 当系统认为窗口上的GDI对象应该被重绘时，会向窗口发送一个WM_PAINT消息。
		// 当然应用程序也可以通过调用 UpateWindow来主动向窗口发送一个WM_PAINT消息。
		// 所有使用GDI在窗口上绘制图形的程序都 “必须” 写在这里。
		// 如果不是在WM_PAINT消息的处理过程中绘制GDI图形，那么在窗口刷新时就会被新被抹除和覆盖
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
	// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。

	// Fill in the window class structure with parameters 
	// that describe the main window. 
	// 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	wc.lpfnWndProc = PlayerWindowProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon成员用来指定窗口的图标
	// 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
	wc.hIcon = NULL;
	// Cursor是鼠标光标，这里是设定了鼠标光标的样式。
	// 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	// GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
	// 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(TRANS_BK_COLOR);
	// 窗口的菜单的资源名。
	wc.lpszMenuName = NULL;
	// 给窗口类起一个名字，在创建窗口时需要这个名字。
	wc.lpszClassName = CLASS_NAME_PLAYER;
	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return FALSE;
	}
	return TRUE;
}

HWND PlayerWindowCreate(HINSTANCE hinstance)
{
	HWND hwnd;
	hwnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE,
		CLASS_NAME_PLAYER,			// 窗口类名，必须是已经注册了的窗口类
		"",			// title-bar string 
		WS_POPUP,	// 窗口的style，这个表示为top-level window 
		CW_USEDEFAULT,			// 窗口水平位置default horizontal POINT 
		CW_USEDEFAULT,			// 窗口垂直位置default vertical POINT 
		CW_USEDEFAULT,			// 窗口宽度 default width 
		CW_USEDEFAULT,			// 窗口高度 default height 
		(HWND)NULL,				// 父窗口句柄 no owner window 
		(HMENU)NULL,			// 窗口菜单的句柄 use class menu 
		hinstance,				// 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);			// 指向附加数据的指针 no window-creation data 
	if (!hwnd)
	{
		GetLastError();
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return NULL;
	}


	if (!SetLayeredWindowAttributes(
		hwnd, TRANS_BK_COLOR,
		255, LWA_COLORKEY | LWA_ALPHA))
	{
		DWORD dwError = GetLastError();
	}

	// 窗口创建成功，继续运行。
	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwnd, SW_SHOW);
	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwnd);
	return hwnd;
}

LONG PlayerWindowPaint(HWND hwnd)
{
	int num, i;

	//鱼（玩家）
	HDC hdc, hdcMem, hdcBitmapSrc;
	HBITMAP hBitmap;
	BITMAP bmp;
	//鱼（其他）
	PFISH fish;

	//背景
	RECT rect;
	//字体
	HFONT hFont;


	CHAR debug_info[100];

	GetClientRect(hwnd, &rect);
	//鱼（玩家）
	hdc = GetDC(hwnd);
	hdcMem = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, // 不能是hdcMem，否则会变成黑白2色位图
		rect.right - rect.left, rect.bottom - rect.top);
	SelectObject(hdcMem, hBitmap);
	hdcBitmapSrc = CreateCompatibleDC(hdc);

	/*******************************************************************************
	* #############  画背景  ################
	*
	*******************************************************************************/
	FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	// 鱼（玩家）

	SelectObject(hdcBitmapSrc, hbmpPlayer);

	GetObject(hbmpPlayer, sizeof(BITMAP), &bmp);

	StretchBlt(hdcMem,
		ptPlayer.x, ptPlayer.y,
		PLAYER_WIDTH, PLAYER_HEIGHT,
		hdcBitmapSrc,
		0, 0, bmp.bmWidth, bmp.bmHeight,
		SRCCOPY);

	// 鱼（其他）
	num = getFishSize();

	for (i = 0; i < num; i++) {
		fish = getFishAt(i);
		switch (fish->_fishlevel) {
		case FISH_LEV_1:
			SelectObject(hdcBitmapSrc, hbmpFish_1);

			GetObject(hbmpFish_1, sizeof(BITMAP), &bmp);

			StretchBlt(hdcMem,
				fish->_coord.x, fish->_coord.y,
				fish->_rang._x, fish->_rang._y,
				hdcBitmapSrc,
				0, 0, bmp.bmWidth, bmp.bmHeight,
				SRCCOPY);
			break;

		case FISH_LEV_2:
			SelectObject(hdcBitmapSrc, hbmpFish_2);

			GetObject(hbmpFish_2, sizeof(BITMAP), &bmp);

			StretchBlt(hdcMem,
				fish->_coord.x, fish->_coord.y,
				fish->_rang._x, fish->_rang._y,
				hdcBitmapSrc,
				0, 0, bmp.bmWidth, bmp.bmHeight,
				SRCCOPY);
			break;

		case FISH_LEV_3:
			SelectObject(hdcBitmapSrc, hbmpFish_3);

			GetObject(hbmpFish_3, sizeof(BITMAP), &bmp);

			StretchBlt(hdcMem,
				fish->_coord.x, fish->_coord.y,
				fish->_rang._x, fish->_rang._y,
				hdcBitmapSrc,
				0, 0, bmp.bmWidth, bmp.bmHeight,
				SRCCOPY);
			break;

		case FISH_LEV_4:
			SelectObject(hdcBitmapSrc, hbmpFish_4);

			GetObject(hbmpFish_4, sizeof(BITMAP), &bmp);

			StretchBlt(hdcMem,
				fish->_coord.x, fish->_coord.y,
				fish->_rang._x, fish->_rang._y,
				hdcBitmapSrc,
				0, 0, bmp.bmWidth, bmp.bmHeight,
				SRCCOPY);
			break;
		}
	}

	// 创建了一个字体对象
	hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, 0, FALSE, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));


	wsprintf(debug_info, "FISH COUNT: %d\n SCORE：0x%p", num, getScore());
	SelectObject(hdcMem, hFont);
	SetTextColor(hdcMem, RGB(0, 0, 0));
	TextOut(hdcMem, 10, 10, debug_info, strlen(debug_info));


	// 拷贝到DC
	BitBlt(hdc,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		hdcMem,
		0, 0,
		SRCCOPY);

	DeleteObject(hFont);
	DeleteObject(hBitmap);
	DeleteDC(hdcBitmapSrc);
	DeleteDC(hdcMem);
	ReleaseDC(hwnd, hdc);
	//DeleteDC(hdc);
	return 0;
}

LONG PlayerCreate(HWND hwnd)
{
	//BITMAP bmp;
	hbmpPlayer = LoadImage(NULL, BITMAP_FILE_PLAYER,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmpPlayer == NULL)
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

	//SetFocus(hwndBackground);

	//MoveWindow(hwnd, 100, 100, MAX_X, MAX_Y, TRUE);
}

LONG PlayerTimer(HWND hwnd)
{
	MoveFishSchool();

	IsFishOut();

	if (IsFishDead()) {
		KillTimer(hwnd, TIMER_ID);
		MessageBox(hwnd, "天哪！小鱼被吃掉惹~~~", "ooooo..", 0);
		ExitProcess(0);
		DestroyFishByState();
		return 0;
	}

	DestroyFishByState();

	if (rand() % 1000 < 150) // 2%的概率，随机产生敌机。
	{
		CreateFish();
	}

	return 0;
}


LONG CALLBACK PlayerWindowProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{

	// 注意，是switch-case, 每次这个函数被调用，只会落入到一个case中。
	switch (msg)
	{
		// 当窗口被创建时，收到的第一个消息就是WM_CREATE，
		// 一般收到这个消息处理过程中，可以用来进行一些初始化的工作
	case WM_CREATE:
		PlayerCreate(hwnd);

	case WM_PAINT:
		PlayerWindowPaint(hwnd);
		break;

	case WM_KEYDOWN:
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
	HINSTANCE hinstance, // 程序实例句柄，在程序运行，进程创建后，由操作系统向应用程序传入
	HINSTANCE hPrevInstance, // 父进程的程序实例句柄
	LPSTR lpCmdLine,  // 命令行参数，地位和作用类似C语言main函数参数argc和argv,但是没有按空格进行切分
	int nCmdShow)   // 用于指明窗口是否需要显示的参数。
{
	BOOL fGotMessage;
	MSG msg;

	if (BackGroundWindwowRegister(hinstance))
		hwndBackground = BackgroundWindowCreate(hinstance);
	else
		return 0;

	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
		&& fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}