#pragma once
#include <Windows.h>
#include "AddrHelper.h"
#include "midi.h"
#include <stdio.h>


char buttonName[][20] = {
	"食物消耗",
	"木头消耗",
	"石头消耗",
	"铁块消耗",
	"宝石消耗",
	"金币消耗",
	"建筑生命",
};
char labelName[][50] = {
	"CTRL+ATL+F1",
	"CTRL+ATL+F2",
	"CTRL+ATL+F3",
	"CTRL+ATL+F4",
	"CTRL+ATL+F5",
	"CTRL+ATL+F6",
	"CTRL+ATL+F7",
};
const int checkbn = 7;
char wintitle[] = "要战便战修改器";
const char upurl[] = "https://www.bilibili.com/video/BV1um4y1i7bm/";

class gameCheater
{
	char* className = "UnityWndClass";
	char* capName = "Diplomacy is Not an Option";
	HWND gameHwnd = 0;//窗口句柄
	HANDLE hProcess = 0;
	DWORD pid;
	bool run;//对象是否析构
public:
	Midi midi;
	bool gameisRun = false;
	int LockState[9] = { 0 };
	gameCheater() {
		run = true;
		findGame();
		CloseHandle(CreateThread(NULL, 0, Update, this, 0, NULL));
		//CloseHandle(CreateThread(NULL, 0, UpdateAddr, this, 0, NULL));
	}
	void LockSet(int idx, bool isLock) {
		if (isLock) {
			midi.setCh(0);
			midi.out(58+idx);
			LockState[idx] = true;
		}
		else {
			midi.setCh(9);
			midi.out(50);
			LockState[idx] = false;
		}
	}
	~gameCheater() {
		run = false;
	}

	void SetLockState(DWORD64 addr,int enable) {
		SIZE_T dwSize;
		char rdata[4] = {0x00};
		ReadProcessMemory(hProcess, (LPVOID)(addr), rdata, 4, &dwSize);
		if (enable) {
			if (rdata[0] != 0x4C) {
				const char data[4] = { 0x4C,0x89,0x5C,0x10 };
				WriteProcessMemory(hProcess, (LPVOID)(addr), data, 4, &dwSize);
			}
		}
		else {
			if (rdata[0] != 0x44) {
				const char data[4] = { 0x44,0x89,0x44,0x02 };
				WriteProcessMemory(hProcess, (LPVOID)(addr), data, 4, &dwSize);
			}
		}
	}
	void SetBuildLockState(DWORD64 addr, int enable) {
		SIZE_T dwSize;
		char rdata[7] = { 0x00 };
		ReadProcessMemory(hProcess, (LPVOID)(addr), rdata, 2, &dwSize);
		if (enable) {
			if (rdata[0] != 0x42) {
				const char data[7] = {0x42,0x0F,0x11,0x54,0x11};
				WriteProcessMemory(hProcess, (LPVOID)(addr+1), data, 5, &dwSize);
			}
		}
		else {
			if (rdata[0] != 0x41) {
				const char data[7] = {0x41,0x0F,0x11,0x7C,0x0A};
				WriteProcessMemory(hProcess, (LPVOID)(addr+1), data, 5, &dwSize);
			}
		}
	}
private:
	//static DWORD WINAPI UpdateAddr(LPVOID lpParamter) {
	//	gameCheater* obj = (gameCheater*)lpParamter;
	//	while (obj->run)
	//	{
	//		obj->findGame();
	//		Sleep(5000);
	//	}
	//	return 0;
	//}
	static DWORD WINAPI Update(LPVOID lpParamter) {
		gameCheater* obj = (gameCheater*)lpParamter;
		while (obj->run)
		{
			obj->findGame();
			if (obj->gameisRun) {
				DWORD64 address = 0;
				GetModuleBaseAddress64(obj->hProcess, "lib_burst_generated.dll", address);
				obj->SetLockState(address + 0x45AA48, obj->LockState[0]);
				obj->SetLockState(address + 0x45B308, obj->LockState[1]);
				obj->SetLockState(address + 0x45B148, obj->LockState[2]);
				obj->SetLockState(address + 0x45AC08, obj->LockState[3]);
				obj->SetLockState(address + 0x45AF88, obj->LockState[4]);
				obj->SetLockState(address + 0x45ADC8, obj->LockState[5]);
				obj->SetBuildLockState(address + 0x4D369C, obj->LockState[6]);
			}
			Sleep(5000);
		}
		return 0;
	}
	
	void getAddr(DWORD64 &address, DWORD64 offset[],int size) {
		SIZE_T dwSize;
		for (int i = 0; i < size; i++)
		{
			if (!ReadProcessMemory(hProcess, (LPVOID)(address + offset[i]), &address, 8, &dwSize)) {
				address = 0;
				return;
			}
		}
	}

	void findGame() {
		gameHwnd = FindWindow(className, capName);
		if (gameHwnd) {
			GetWindowThreadProcessId(gameHwnd, &pid);
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
			if (!gameisRun) {
				gameisRun = true;
			}
		}
		else {
			if (gameisRun) {
				gameisRun = false;
			}
		}
	}
};
