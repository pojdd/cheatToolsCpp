#include "midi.h"
#include <time.h>
#include "game.h"
#include "resource.h"

HANDLE hProcess = 0;
char * pname = 0;
char * className = "UnityWndClass";
char * capName = "KingdomTwoCrowns";
char szText[20] = "Byһ�μ���";

gameCheater game;

DWORD pid;
Midi midi;
#define IDB_ONE     53301
#define IDB_UPD     53310

const int checkbn = 8;
const int startid = 53301;
int startx = 10;
int starty = 10;
int steph = 30;
int stepw = 150;
int ww = 320;
int wh = 180+ checkbn * steph;
HWND hwnd=0;//���ھ��
char szClassName[20] = "CarrionCc";
char CheckButtonClassName[20] = "CheckButtonCc";
static HFONT hFont;//Ĭ������

char buttonName[][20] = {
	"��������",
	"���һ������",
	"���һ���;�",
	"���һ�������ٶ�",
	"���һ��������",
	"���һ���Զ����",
	"���һ�������ٶ�",
	"���һ��������Χ",
};
char labelName[][50] = {
	"CTRL+ATL+F1",
	"CTRL+ATL+F2",
	"CTRL+ATL+F3",
	"CTRL+ATL+F4",
	"CTRL+ATL+F5",
	"CTRL+ATL+F6",
	"CTRL+ATL+F7",
	"CTRL+ATL+F8",
};
int HotKeyId[checkbn];

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup")

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);//���ں����ĺ���ԭ��
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
		::MessageBox(NULL, "�������ڳ���", "ϵͳ��ʾ", MB_OK);
	}
	hFont = CreateFont(
		-15/*�߶�*/, -7.5/*���*/, 0/*���ù�*/, 0/*���ù�*/, 500 /*һ�����ֵ��Ϊ400*/,
		FALSE/*����б��*/, FALSE/*�����»���*/, FALSE/*����ɾ����*/,
		GB2312_CHARSET,  //����ʹ��Ĭ���ַ��������������� _CHARSET ��β�ĳ�������
		OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //���в������ù�
		NONANTIALIASED_QUALITY,  //Ĭ���������
		FF_DONTCARE,  //��ָ��������*/
		"΢���ź�"  //������
	);

	for (int i = startid; i < startid+ checkbn; i++)
	{
		HWND bthwnd = CreateWindow("Button", buttonName[i- startid], WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_BORDER | BS_FLAT,
			startx, starty, stepw, steph, hwnd, (HMENU)i, NULL, NULL);

		SendMessage(bthwnd, WM_SETFONT, (WPARAM)hFont, 1);

		starty += steph + 10;
	}
	HWND bthwnd = CreateWindow("Button","����", WS_VISIBLE | WS_CHILD  | BS_FLAT,
		130, wh - 58, 50, 18, hwnd, (HMENU)IDB_UPD, NULL, NULL);
	SendMessage(bthwnd, WM_SETFONT, (WPARAM)hFont, 1);

	starty = 10;
	for (int i = 1000; i < 1000+ checkbn; i++)
	{
		HWND hStatic = CreateWindow(
			"static", //��̬�ı��������
			labelName[i - 1000],  //�ؼ����ı�
			WS_CHILD /*�Ӵ���*/ | WS_VISIBLE /*����ʱ��ʾ*/ | SS_LEFT /*ˮƽ����*/ | SS_CENTERIMAGE /*��ֱ����*/,
			stepw+20 /*X����*/, starty /*Y����*/, 200 /*���*/, steph /*�߶�*/,
			hwnd,  //�����ھ��
			(HMENU)i,  //Ϊ�ؼ�ָ��һ��Ψһ��ʶ��
			NULL,  //��ǰ����ʵ�����
			NULL
		);
		SendMessage(
			hStatic,  //����������Ŀؼ����
			WM_SETFONT,  //��Ϣ������Ϣ���ͣ�
			(WPARAM)hFont,  //������
			1  //����ֵ����
		);
		starty += steph + 10;
	}

	for (int i = 0; i < checkbn; i++)
	{
		HotKeyId[i] = GlobalAddAtom("MyHotKey1");
		RegisterHotKey(hwnd, HotKeyId[i], MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, VK_F1+i);
	}

	SetTimer(hwnd, 12, 500, NULL);
	::ShowWindow(hwnd, SW_SHOW);//��ʾ����
	::UpdateWindow(hwnd);//ˢ�´���
	MSG msg;
	SendMessage(hwnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)LoadIcon(GetModuleHandle(NULL), (LPCTSTR)IDI_ICON1));
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);//ת��Ϊ������Ϣ
		::DispatchMessage(&msg);//����Ϣ���͵���Ӧ�Ĵ��ں���
	}
	return msg.wParam;//��GetMessage����FALSEʱ�������
}
void registerAllClass() {
	WNDCLASSEX wndclass;   //�������������ڵĽṹ
	wndclass.cbSize = sizeof(wndclass);  //�ṹ�Ĵ�С
	wndclass.style = CS_HREDRAW | CS_VREDRAW; //������ڴ�С������ı������ı䶼���ػ�����
	wndclass.lpfnWndProc = MainWndProc;//���ں���ָ��
	wndclass.cbClsExtra = 0;//û�ж�������ڴ�
	wndclass.cbWndExtra = 0;//û�ж���Ĵ����ڴ�
	//wndclass.hInstance = hInstance;//ʵ�����
	wndclass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);//ʹ��Ԥ����ͼ��
	wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);//ʹ��Ԥ������
	wndclass.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);//ʹ�ð�ɫ������ˢ
	wndclass.lpszMenuName = NULL;//��ָ���˵�
	wndclass.lpszClassName = szClassName;//�����������
	wndclass.hIconSm = NULL;//û�����Сͼ��
	wndclass.hbrBackground = CreateSolidBrush(RGB(0xec, 0xed, 0xef));
	::RegisterClassEx(&wndclass);//ע�����������
}
void initWindows() {
	//MessageBoxA(0, "��һ��C�������", "��Ϣ", 3);
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	hwnd = ::CreateWindowEx(
		0,//��չ��ʽ
		szClassName,//����
		"�ڻ����޸���",//������������
		WS_EX_LAYERED | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,//���ڷ��
		(nScreenWidth - ww) / 2,//��ʼX����
		(nScreenHeight - wh) / 2,//��ʼY����
		ww,//���
		wh,//�߶�
		NULL,//�����ھ��
		NULL,//�˵����
		NULL,//ʵ�����
		//hInstance,//ʵ�����
		NULL//�û�����
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
	//�����Ϸ�Ƿ����У���ʼ��״̬
	switch (message)
	{
	case WM_COMMAND:
		for (int i = 0; i < checkbn; i++)
		{
			if (wParam == IDB_ONE + i) {
				if (IsDlgButtonChecked(hwnd, IDB_ONE + i))
				{
					game.LockSet(i, true);
				}
				else
				{
					game.LockSet(i, false);
				}
			}
		}
		switch (LOWORD(wParam))
		{
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

			ShellExecute(NULL, "open", "https://www.bilibili.com/video/BV1ud4y1E7L2", NULL, NULL, SW_SHOWNORMAL);
			Sleep(1000);
			game.midi.out(0);//1
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	case WM_HOTKEY:
		for (int i = 0; i < checkbn; i++)
		{
			if (HotKeyId[i] == wParam) {
				if (IsDlgButtonChecked(hwnd, IDB_ONE + i))
				{
					CheckDlgButton(hwnd, IDB_ONE + i, BST_UNCHECKED);
					game.LockSet(i,false);
				}
				else
				{
					CheckDlgButton(hwnd, IDB_ONE + i, BST_CHECKED);
					game.LockSet(i,true);
				}
			}
		}
	/*	if (HotKeyId1 == wParam) {
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
		}*/
		break;
	case WM_CREATE:
		break;
	case WM_PAINT:
	{
		HDC hdc;
		PAINTSTRUCT ps;
		hdc = ::BeginPaint(hwnd, &ps);//ʹ��Ч�Ŀͻ��������Ч����ȡ���豸�������
		SelectObject(hdc, hFont);
		//����������Ϣ
		::SetTextColor(hdc, RGB(168, 125, 115));
		::SetBkMode(hdc, TRANSPARENT);
		::TextOut(hdc, ww - 95, wh - 57, szText, strlen(szText));//����ı�
		
		//������Ϸ״̬
		if (game.gameisRun)
		{
			::SetTextColor(hdc, RGB(250, 128, 164));
			::TextOut(hdc, 0, wh - 57, "��Ϸ������", strlen(szText));//����ı�
		}
		else
		{
			::SetTextColor(hdc, RGB(50, 50, 50));
			::TextOut(hdc, 0, wh - 57, "��Ϸδ����", strlen(szText));//����ı�
		}
		
		::EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:
	{
		::PostQuitMessage(0);//����Ϣ����Ͷ��һ��WM_QUIT��Ϣ����ʹGET MESSAGE��������0��������Ϣѭ��
		return 0;
	}
	case WM_SYSCOLORCHANGE:
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_CTLCOLORSTATIC:
		//HBRUSH hbr = (HBRUSH)CreateSolidBrush(RGB(0, 0x7d, 0x65));
		//return (LRESULT)hbr;
		
		//�޸İ�ť������ɫ
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