#pragma once
#include <Windows.h>
#include "AddrHelper.h"
#include "midi.h"
#include <stdio.h>

char buttonName[][20] = {
	"修改为6人",
	"增加4人",
	"增加8人",
	"增加16人",
	"增加32人",
	"无限耐力",
	"无限SAN",
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
char wintitle[] = "逃离后室多人工具";
const char upurl[] = "https://www.bilibili.com/video/BV1um4y1i7bm/";

class gameCheater
{
	char* className = "UnrealWindow";
	char* capName = "EscapeTheBackrooms  ";
	HWND gameHwnd = 0;//窗口句柄
	HANDLE hProcess = 0;
	DWORD pid;
	SIZE_T HPaddress = 0;//位置地址
	SIZE_T PWaddress = 0;//位置地址
	SIZE_T SANaddress=0;//位置地址
	bool run;//对象是否析构
	//MemSearch memsearch;//内存搜索器
	//Signature Numbsign;//人数特征
public:
	Midi midi;
	bool gameisRun = false;
	int LockState[9] = { 0 };
	gameCheater() {
		//Numbsign.searchstep = 0x100;
		//Numbsign.setBase(-0x100 + 0xD8);
		//Numbsign[0xE0] = 0xC0;
		//Numbsign[0xE8] = 0x60;
		//Numbsign += 0x1A;
		//Numbsign[0xD8] = 0x02;
		//Numbsign += 0x00;
		//Numbsign += 0x00;
		//Numbsign += 0x00;
		//Numbsign[0xF8] = 0x04;
		//Numbsign += 0x00;
		//Numbsign += 0x00;
		//Numbsign += 0x00;
		//Numbsign[0xFD] = 0x00;
		//Numbsign[0x103] = 0x70;

		run = true;
		findGame();
		CloseHandle(CreateThread(NULL, 0, Update, this, 0, NULL));
		CloseHandle(CreateThread(NULL, 0, UpdateAddr, this, 0, NULL));
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

	void GetHpAddr() {
		//if (!memsearch.verifyAddr(Numbsign, HPaddress)) {
		//	HPaddress = (DWORD64)memsearch.getAddr(Numbsign);
		//}
		DWORD64 address = 0;
		//SIZE_T dwSize;
		GetModuleBaseAddress64(hProcess, "Backrooms-Win64-Shipping.exe", address);
		//DWORD64 offset[] = { 0x046CF020,0x610,0x510,0x460,0xB0,0x168,0x20,0x340,0x270,0x20,0x20};
		//getAddr(address, offset, sizeof(offset)/sizeof(DWORD64));
		if (address == 0) {
			HPaddress = 0;
			return;
		}
		HPaddress = address + 0x2F35CB6;
		//printf("%X\n", HPaddress);
	}
	//POWER 0-50 float
	void GetPOWERAddr() {
		DWORD64 address = 0;
		SIZE_T dwSize;
		GetModuleBaseAddress64(hProcess, "Backrooms-Win64-Shipping.exe", address);
		if (address == 0) {
			PWaddress = 0;
			return;
		}
		PWaddress = address + 0x2F35CC1;
		//if (!memsearch.verifyAddr(POWERsign, PWaddress)) {
		//	printf("research\n");
		//	PWaddress = (DWORD64)memsearch.getAddr(POWERsign);
		//}
		//printf("%p\n", PWaddress);
	}
	//SAN 0-100 float
	void GetSANAddr() {
		SIZE_T address = 0;
		SIZE_T dwSize;
		GetModuleBaseAddress64(hProcess, "Backrooms-Win64-Shipping.exe", address);
		SIZE_T offset[] = { 0x046CF020,0x30,0x228};
		getAddr(address, offset, 3);
		if (address == 0) {
			SANaddress = 0;
			return;
		}
		SANaddress = address + 0x328;
	}

	void SetHP(int islock) {
		SIZE_T dwSize;
		DWORD Hp = 0;
		Hp = 6 + LockState[1] * 4 + LockState[2] * 8 + LockState[3] * 16 + LockState[4] * 32;
		//printf("here %p\n", HPaddress);
		if (islock) {
			char data[] = { 0x90 ,0x90 ,0x8B ,0x44 ,0x24 ,0x58 ,0xB8 ,Hp&0xFF ,0x00 ,0x00 ,0x00};
			if (!WriteProcessMemory(hProcess, (LPVOID)(HPaddress), data, sizeof(data), &dwSize)) {
				//printf("no : %d\n", dwSize);
				return;
			}
			else {
				//printf("ok : %d\n", dwSize);
			}
		}
		else {
			char data[] = { 0x7C ,0x09 ,0x8B ,0x44 ,0x24 ,0x58 ,0x3B ,0xC8 ,0x0F ,0x4C ,0xC1};
			if (!WriteProcessMemory(hProcess, (LPVOID)(HPaddress), data, sizeof(data), &dwSize)) {
				//printf("no : %d\n", dwSize);
				return;
			}
			else {
				//printf("ok : %d\n", dwSize);
			}
		}
	}
	void SetPOWER() {
		SIZE_T dwSize;
		float Hp = 0;
		if (!ReadProcessMemory(hProcess, (LPVOID)(PWaddress), &Hp, 4, &dwSize)) {
			return;
		}
		if (Hp < 10 || Hp > 50) {
			return;
		}
		Hp = 35;
		if (!WriteProcessMemory(hProcess, (LPVOID)(PWaddress), &Hp, 4, &dwSize)) {
			return;
		}
	}
	void SetSAN() {
		SIZE_T dwSize;
		float Hp = 0;
		if (!ReadProcessMemory(hProcess, (LPVOID)(SANaddress), &Hp, 4, &dwSize)) {
			return;
		}
		if (Hp < 10 || Hp > 200) {
			return;
		}
		Hp = 100;
		if (!WriteProcessMemory(hProcess, (LPVOID)(SANaddress), &Hp, 4, &dwSize)) {
			return;
		}
	}
private:
	static DWORD WINAPI UpdateAddr(LPVOID lpParamter) {
		gameCheater* obj = (gameCheater*)lpParamter;
		while (obj->run)
		{
			obj->findGame();
			//if (!obj->memsearch.lasttest) {
			//	obj->memsearch.updataMemMap();
			//}
			if (obj->gameisRun) {
				obj->GetHpAddr();
				obj->GetPOWERAddr();
				obj->GetSANAddr();
			}
			Sleep(1800);
		}
		return 0;
	}
	static DWORD WINAPI Update(LPVOID lpParamter) {
		gameCheater* obj = (gameCheater*)lpParamter;
		while (obj->run)
		{
			obj->findGame();
			if (obj->gameisRun) {
				obj->SetHP(obj->LockState[0]);
				if (obj->LockState[5]) {
					obj->SetPOWER();
				}
				if (obj->LockState[6]) {
					obj->SetSAN();
				}
			}
			Sleep(1400);
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
				//memsearch.setProcess(hProcess);
				//memsearch.updataMemMap();
				//memsearch.showMemMap();
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
