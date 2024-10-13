#include "midi.h"
#include <time.h>
#include "game.h"
#include "resource.h"

HANDLE hProcess = 0;
char * pname = 0;
char * className = "UnityWndClass";
char * capName = "KingdomTwoCrowns";
char szText[20] = "By一梦箭心";

gameCheater game;

DWORD pid;
Midi midi;
#define IDB_ONE     53301  
#define IDB_TWO     53302  
#define IDB_TRE     53303  
#define IDB_FOR     53304  
#define IDB_UPD     53310

int checkbn = 4;
int startid = 53301;
int startx = 10;
int starty = 10;
int steph = 30;
int ww = 320;
int wh = 240;
HWND hwnd=0;//窗口句柄
char szClassName[20] = "CarrionCc";
char CheckButtonClassName[20] = "CheckButtonCc";
static HFONT hFont;//默认字体

char buttonName[][20] = {
	"锁定金币",
	"锁定钻石",
	"人物瞬移",
	"无限乞丐",
};
char labelName[][50] = {
	"锁定金币为5 ATL+F1",
	"锁定钻石为5 ATL+F2",
	"按住F加方向 ATL+F3",
	"我难民打钱  ATL+F5"
};
int HotKeyId1;
int HotKeyId2;
int HotKeyId3;
int HotKeyId4;

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup")

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);//窗口函数的函数原型
void initWindows();
void registerAllClass();

int main()
{
	HANDLE hMutex;
	hMutex = CreateMutex(NULL, FALSE, "r");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		exit(0);
	}

	registerAllClass();
	initWindows();
	if (hwnd == NULL)
	{
		::MessageBox(NULL, "创建窗口出错", "系统提示", MB_OK);
	}
	hFont = CreateFont(
		-15/*高度*/, -7.5/*宽度*/, 0/*不用管*/, 0/*不用管*/, 500 /*一般这个值设为400*/,
		FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
		GB2312_CHARSET,  //这里使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
		NONANTIALIASED_QUALITY,  //默认输出质量
		FF_DONTCARE,  //不指定字体族*/
		"微软雅黑"  //字体名
	);

	for (int i = startid; i < startid+ checkbn; i++)
	{
		HWND bthwnd = CreateWindow("Button", buttonName[i- startid], WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_BORDER | BS_FLAT,
			startx, starty, 100, steph, hwnd, (HMENU)i, NULL, NULL);

		SendMessage(bthwnd, WM_SETFONT, (WPARAM)hFont, 1);

		starty += steph + 10;
	}
	HWND bthwnd = CreateWindow("Button","更新", WS_VISIBLE | WS_CHILD  | BS_FLAT,
		130, wh - 58, 50, 18, hwnd, (HMENU)IDB_UPD, NULL, NULL);
	SendMessage(bthwnd, WM_SETFONT, (WPARAM)hFont, 1);

	starty = 10;
	for (int i = 1000; i < 1000+ checkbn; i++)
	{
		HWND hStatic = CreateWindow(
			"static", //静态文本框的类名
			labelName[i - 1000],  //控件的文本
			WS_CHILD /*子窗口*/ | WS_VISIBLE /*创建时显示*/ | SS_LEFT /*水平居中*/ | SS_CENTERIMAGE /*垂直居中*/,
			120 /*X坐标*/, starty /*Y坐标*/, 200 /*宽度*/, steph /*高度*/,
			hwnd,  //父窗口句柄
			(HMENU)i,  //为控件指定一个唯一标识符
			NULL,  //当前程序实例句柄
			NULL
		);
		SendMessage(
			hStatic,  //欲设置字体的控件句柄
			WM_SETFONT,  //消息名（消息类型）
			(WPARAM)hFont,  //字体句柄
			1  //传空值即可
		);
		starty += steph + 10;
	}

	HotKeyId1 = GlobalAddAtom("MyHotKey1");
	RegisterHotKey(hwnd, HotKeyId1, MOD_ALT| MOD_NOREPEAT, VK_F1);

	HotKeyId2 = GlobalAddAtom("MyHotKey2");
	RegisterHotKey(hwnd, HotKeyId2, MOD_ALT | MOD_NOREPEAT, VK_F2);

	HotKeyId3 = GlobalAddAtom("MyHotKey3");
	RegisterHotKey(hwnd, HotKeyId3, MOD_ALT | MOD_NOREPEAT, VK_F3);

	HotKeyId4 = GlobalAddAtom("MyHotKey4");
	RegisterHotKey(hwnd, HotKeyId3, MOD_ALT | MOD_NOREPEAT, VK_F5);

	SetTimer(hwnd, 12, 500, NULL);
	::ShowWindow(hwnd, SW_SHOW);//显示窗口
	::UpdateWindow(hwnd);//刷新窗口
	MSG msg;
	SendMessage(hwnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)LoadIcon(GetModuleHandle(NULL), (LPCTSTR)IDI_ICON1));
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);//转换为键盘消息
		::DispatchMessage(&msg);//将消息发送到相应的窗口函数
	}
	return msg.wParam;//当GetMessage返回FALSE时程序结束
}
void registerAllClass() {
	WNDCLASSEX wndclass;   //用来描述主窗口的结构
	wndclass.cbSize = sizeof(wndclass);  //结构的大小
	wndclass.style = CS_HREDRAW | CS_VREDRAW; //如果窗口大小，横向改变和竖向改变都会重画窗口
	wndclass.lpfnWndProc = MainWndProc;//窗口函数指针
	wndclass.cbClsExtra = 0;//没有额外的类内存
	wndclass.cbWndExtra = 0;//没有额外的窗口内存
	//wndclass.hInstance = hInstance;//实例句柄
	wndclass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);//使用预定义图标
	wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);//使用预定义光标
	wndclass.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);//使用白色背景画刷
	wndclass.lpszMenuName = NULL;//不指定菜单
	wndclass.lpszClassName = szClassName;//窗口类的名称
	wndclass.hIconSm = NULL;//没有类的小图标
	wndclass.hbrBackground = CreateSolidBrush(RGB(0xec, 0xed, 0xef));
	::RegisterClassEx(&wndclass);//注册这个窗口类
}
void initWindows() {
	//MessageBoxA(0, "第一个C窗体程序", "消息", 3);
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	hwnd = ::CreateWindowEx(
		0,//扩展样式
		szClassName,//类名
		"王国双王修改器",//窗口名。标题
		WS_EX_LAYERED | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,//窗口风格
		(nScreenWidth - ww) / 2,//初始X坐标
		(nScreenHeight - wh) / 2,//初始Y坐标
		ww,//宽度
		wh,//高度
		NULL,//父窗口句柄
		NULL,//菜单句柄
		NULL,//实例句柄
		//hInstance,//实例句柄
		NULL//用户数据
	);
}

bool run=true;
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (run != game.gameisRun) {
		run = game.gameisRun;
		InvalidateRect(hwnd, NULL, TRUE);
		//for (int i = startid; i < startid + checkbn; i++)
		//{
		//	CheckDlgButton(hwnd, i, BST_UNCHECKED);
		//}
	}
	//检查游戏是否运行，初始化状态
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDB_ONE:
			if (IsDlgButtonChecked(hwnd, IDB_ONE))
			{
				game.LockCoin(true);
			}
			else
			{
				game.LockCoin(false);
			}
			break;
		case IDB_TWO:
			if (IsDlgButtonChecked(hwnd, IDB_TWO))
			{
				game.LockDm(true);
			}
			else
			{
				game.LockDm(false);
			}
			break;
		case IDB_TRE:
			if (IsDlgButtonChecked(hwnd, IDB_TRE))
			{
				game.Fmove(true);
			}
			else
			{
				game.Fmove(false);
			}
			break;
		case IDB_FOR:
			if (IsDlgButtonChecked(hwnd, IDB_FOR))
			{
				game.LockBg(true);
			}
			else
			{
				game.LockBg(false);
			}
			break;
		case IDB_UPD:
			game.midi.setCh(0);
			game.midi.out(67);//5
			Sleep(208);
			game.midi.out(72);
			Sleep(208);
			game.midi.out(74);//.2
			Sleep(208);
			game.midi.out(76);//.3
			game.midi.out(48);//1.
			Sleep(208);
			game.midi.out(77);//.4
			game.midi.out(55);//5.
			Sleep(208);
			game.midi.out(79);
			game.midi.out(60);
			Sleep(208);
			game.midi.out(76);
			Sleep(208);
			game.midi.out(74);//.2
			game.midi.out(47);//7..
			Sleep(208);
			game.midi.out(55);//5.
			Sleep(208);
			game.midi.out(72);//.1
			game.midi.out(60);//1

			ShellExecute(NULL, "open", "https://www.bilibili.com/video/BV1WU4y1q7vS", NULL, NULL, SW_SHOWNORMAL);
			Sleep(1000);
			game.midi.out(0);//1
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	case WM_HOTKEY:
		if (HotKeyId1 == wParam) {
			if (IsDlgButtonChecked(hwnd, IDB_ONE))
			{
				CheckDlgButton(hwnd, IDB_ONE, BST_UNCHECKED);
				game.LockCoin(false);
			}
			else
			{
				CheckDlgButton(hwnd, IDB_ONE, BST_CHECKED);
				game.LockCoin(true);
			}
		}
		if (HotKeyId2 == wParam) {
			if (IsDlgButtonChecked(hwnd, IDB_TWO))
			{
				CheckDlgButton(hwnd, IDB_TWO, BST_UNCHECKED);
				game.LockDm(false);
			}
			else
			{
				CheckDlgButton(hwnd, IDB_TWO, BST_CHECKED);
				game.LockDm(true);
			}
		}
		if (HotKeyId3 == wParam) {
			if (IsDlgButtonChecked(hwnd, IDB_TRE))
			{
				CheckDlgButton(hwnd, IDB_TRE, BST_UNCHECKED);
				game.Fmove(false);
			}
			else
			{
				CheckDlgButton(hwnd, IDB_TRE, BST_CHECKED);
				game.Fmove(true);
			}
		}
		if (HotKeyId4 == wParam) {
			if (IsDlgButtonChecked(hwnd, IDB_TRE))
			{
				CheckDlgButton(hwnd, IDB_TRE, BST_UNCHECKED);
				game.LockBg(false);
			}
			else
			{
				CheckDlgButton(hwnd, IDB_TRE, BST_CHECKED);
				game.LockBg(true);
			}
		}
		break;
	case WM_CREATE:
		break;
	case WM_PAINT:
	{
		HDC hdc;
		PAINTSTRUCT ps;
		hdc = ::BeginPaint(hwnd, &ps);//使无效的客户区变的有效，并取得设备环境句柄
		SelectObject(hdc, hFont);
		//绘制作者信息
		::SetTextColor(hdc, RGB(168, 125, 115));
		::SetBkMode(hdc, TRANSPARENT);
		::TextOut(hdc, ww - 95, wh - 57, szText, strlen(szText));//输出文本
		
		//绘制游戏状态
		if (game.gameisRun)
		{
			::SetTextColor(hdc, RGB(250, 128, 164));
			::TextOut(hdc, 0, wh - 57, "游戏运行中", strlen(szText));//输出文本
		}
		else
		{
			::SetTextColor(hdc, RGB(50, 50, 50));
			::TextOut(hdc, 0, wh - 57, "游戏未运行", strlen(szText));//输出文本
		}
		
		::EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:
	{
		::PostQuitMessage(0);//向消息队列投递一个WM_QUIT消息，促使GET MESSAGE函数返回0，结束消息循环
		return 0;
	}
	case WM_SYSCOLORCHANGE:
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_CTLCOLORSTATIC:
		//HBRUSH hbr = (HBRUSH)CreateSolidBrush(RGB(0, 0x7d, 0x65));
		//return (LRESULT)hbr;
		
		//修改按钮背景颜色
		for (int i = startid; i < startid+checkbn; i++)
		{
			if ((HWND)lParam == GetDlgItem(hwnd, i)){
				HWND hbn = (HWND)lParam;
				HDC hdc = (HDC)wParam;
				//RECT rc;
				//RECT rc1;
				//TCHAR text[64];

				//GetWindowText(hbn, text, 63);
				//GetClientRect(hbn, &rc);
				//SetTextColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, TRANSPARENT);
				//rc1.left -= 5;
				//rc1.right = rc1.left+5;
				//rc1.bottom = rc.bottom;
				//rc1.top = rc.top;
				//FillRect(hdc, &rc1,CreateSolidBrush(RGB(65, 166, 76)));
				//DrawText(hdc, text, _tcslen(text), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				return (INT_PTR)CreateSolidBrush(RGB(65, 166, 76));
			}
		}
		HDC hdc = (HDC)wParam;
		SetBkMode(hdc, TRANSPARENT);
		return (INT_PTR)(HBRUSH)::GetStockObject(NULL_BRUSH);
		break;
	}
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}