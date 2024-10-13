#pragma once
#include <Windows.h>
#include "AddrHelper.h"
#include "midi.h"
class gameCheater
{
	char* className = "UnityWndClass";
	char* capName = "Dig or Die";
	HWND gameHwnd = 0;//窗口句柄
	HANDLE hProcess = 0;
	DWORD pid;
	DWORD coMaddress = 0;//钱包地址
	DWORD HPaddress=0;//位置地址
	bool run;//对象是否析构
public:
	Midi midi;
	bool gameisRun = true;
	int LockState[9] = { 0 };
	gameCheater() {
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
	void GetComAddr() {
		DWORD address = 0;
		SIZE_T dwSize;
		GetModuleBaseAddress(pid, "mono.dll", address);
		DWORD offset[] = { 0x001F30AC,0x14,0x6C,0xA8,0x8,0x20,0xC,0x40 };
		getAddr(address, offset, 8);
		coMaddress = address;
	}

	void GetHpAddr() {
		DWORD address = 0;
		SIZE_T dwSize;
		GetModuleBaseAddress(pid, "mono.dll", address);
		DWORD offset[] = { 0x001F52CC,0x20,0xA0,0x44,0xD4,0x0};
		getAddr(address, offset, 6);
		HPaddress = address;
	}

	void SetHP() {
		DWORD address = 0;
		SIZE_T dwSize;
		float Hp = 75;
		if (!ReadProcessMemory(hProcess, (LPVOID)(HPaddress + 0x88), &Hp, 4, &dwSize)) {
			return;
		}
		Hp = 75;
		if (!WriteProcessMemory(hProcess, (LPVOID)(HPaddress + 0x88), &Hp, 4, &dwSize)) {
			return;
		}
	}

	void SetCot1() {
		//最后一格数量
		DWORD address = 0;
		SIZE_T dwSize;
		int count = 0;
		if (!ReadProcessMemory(hProcess, (LPVOID)(coMaddress + 0xc), &count, 4, &dwSize)) {
			return;
		}
		if (count > 2&& count<9999) {
			count = 999;
			if (!WriteProcessMemory(hProcess, (LPVOID)(coMaddress + 0xc), &count, 4, &dwSize)) {
				return;
			}
		}
	}
	void SetCot2() {
		//最后一格耐久
		DWORD address = 0;
		SIZE_T dwSize;
		int count = 0;
		if (!ReadProcessMemory(hProcess, (LPVOID)(coMaddress + 0x8), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x3c), &count, 4, &dwSize)) {
			return;
		}
		if (count < 2000 && count>0) {
			count = 1000;
			if (!WriteProcessMemory(hProcess, (LPVOID)(address + 0x3c), &count, 4, &dwSize)) {
				return;
			}
		}
	}
	void SetCot3() {
		//最后一格武器速度
		DWORD address = 0;
		SIZE_T dwSize;
		float speed = 0;
		if (!ReadProcessMemory(hProcess, (LPVOID)(coMaddress + 0x8), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x38), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x20), &speed, 4, &dwSize)) {
			return;
		}
		if (speed > 0 && speed < 10) {
			speed = 0.001;
			if (!WriteProcessMemory(hProcess, (LPVOID)(address + 0x20), &speed, 4, &dwSize)) {
				return;
			}
		}
	}
	void SetCot4() {
		//最后一格射击间隔
		DWORD address = 0;
		SIZE_T dwSize;
		float speed = 0;
		if (!ReadProcessMemory(hProcess, (LPVOID)(coMaddress + 0x8), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x3c), &speed, 4, &dwSize)) {
			return;
		}
		if (speed > 0 && speed < 10) {
			speed = 0.001;
			if (!WriteProcessMemory(hProcess, (LPVOID)(address + 0x3c), &speed, 4, &dwSize)) {
				return;
			}
		}
			
	}
	void SetCot5() {
		//最后一格自动射击
		DWORD address = 0;
		SIZE_T dwSize;
		int speed = 0;
		if (!ReadProcessMemory(hProcess, (LPVOID)(coMaddress + 0x8), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x40), &speed, 4, &dwSize)) {
			return;
		}
		if (speed == 0) {
			speed = 1;
			if (!WriteProcessMemory(hProcess, (LPVOID)(address + 0x40), &speed, 4, &dwSize)) {
				return;
			}
		}

	}
	void SetCot6() {
		//最后一格炮塔速度
		DWORD address = 0;
		SIZE_T dwSize;
		float speed = 0;
		printf("%x\n", coMaddress);
		if (!ReadProcessMemory(hProcess, (LPVOID)(coMaddress + 0x8), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x5c), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x20), &speed, 4, &dwSize)) {
			return;
		}
		if (speed > 0 && speed < 10) {
			speed = 0.001;
			if (!WriteProcessMemory(hProcess, (LPVOID)(address + 0x20), &speed, 4, &dwSize)) {
				return;
			}
		}

	}
	void SetCot7() {
		//最后一格炮塔范围
		DWORD address = 0;
		SIZE_T dwSize;
		float speed = 0;
		if (!ReadProcessMemory(hProcess, (LPVOID)(coMaddress + 0x8), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x5c), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x14), &speed, 4, &dwSize)) {
			return;
		}
		if (speed > 0 && speed < 30) {
			speed = 20;
			if (!WriteProcessMemory(hProcess, (LPVOID)(address + 0x14), &speed, 4, &dwSize)) {
				return;
			}
		}

	}
private:
	static DWORD WINAPI UpdateAddr(LPVOID lpParamter) {
		gameCheater* obj = (gameCheater*)lpParamter;
		while (obj->run)
		{
			obj->findGame();
			if (obj->gameisRun) {
				obj->GetComAddr();
				obj->GetHpAddr();
			}
			Sleep(5000);
		}
		return 0;
	}
	static DWORD WINAPI Update(LPVOID lpParamter) {
		gameCheater* obj = (gameCheater*)lpParamter;
		while (obj->run)
		{
			obj->findGame();
			if (obj->gameisRun) {
				if (obj->LockState[0]) {
					obj->SetHP();
				}
				if (obj->LockState[1]) {
					obj->SetCot1();
				}
				if (obj->LockState[2]) {
					obj->SetCot2();
				}
				if (obj->LockState[3]) {
					obj->SetCot3();
				}
				if (obj->LockState[4]) {
					obj->SetCot4();
				}
				if (obj->LockState[5]) {
					obj->SetCot5();
				}
				if (obj->LockState[6]) {
					obj->SetCot6();
				}
				if (obj->LockState[7]) {
					obj->SetCot7();
				}
			}
			Sleep(100);
		}
		return 0;
	}
	
	void getAddr(DWORD &address, DWORD offset[],int size) {
		SIZE_T dwSize;
		for (int i = 0; i < size; i++)
		{
			if (!ReadProcessMemory(hProcess, (LPVOID)(address + offset[i]), &address, 4, &dwSize)) {
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
			gameisRun = true;
		}
		else {
			gameisRun = false;
		}
	}
};
