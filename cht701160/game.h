#pragma once
#include <Windows.h>
#include "AddrHelper.h"
#include "midi.h"
//位置和乞丐营同根。可以先找位置。
class gameCheater
{
	char* className = "UnityWndClass";
	char* capName = "KingdomTwoCrowns";
	HWND gameHwnd = 0;//窗口句柄
	HANDLE hProcess = 0;
	DWORD pid;
	DWORD wpaddress = 0;//钱包地址
	DWORD bgddress = 0;//乞丐地址
	DWORD posaddress=0;//位置地址
	bool run;//对象是否析构
public:
	Midi midi;
	bool gameisRun = true;
	bool isLockCoin = false;
	bool isLockDm = false;
	bool isLockBg = false;
	bool isFmove = false;
	
	gameCheater() {
		run = true;
		gameHwnd = FindWindow(className, capName);
		if (gameHwnd) {
			GetWindowThreadProcessId(gameHwnd, &pid);
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
		}
		else {
			gameisRun = false;
		}
		HANDLE hThread = CreateThread(NULL, 0, Update, this, 0, NULL);
		CloseHandle(hThread);
		hThread = CreateThread(NULL, 0, UpdateAddr, this, 0, NULL);
		CloseHandle(hThread);
	}
	void LockCoin(bool isLock) {
		if (isLock) {
			midi.setCh(0);
			midi.out(60);
			isLockCoin = true;
		}
		else {
			midi.setCh(9);
			midi.out(50);
			isLockCoin = false;
		}
	}
	void LockDm(bool isLock) {
		if (isLock) {
			midi.setCh(0);
			midi.out(62);
			isLockDm = true;
		}
		else {
			midi.setCh(9);
			midi.out(50);
			isLockDm = false;
		}
	}
	void LockBg(bool isLock) {
		if (isLock) {
			midi.setCh(0);
			midi.out(65);
			isLockBg = true;
		}
		else {
			midi.setCh(9);
			midi.out(50);
			isLockBg = false;
		}
	}
	void Fmove(bool isLock) {
		if (isLock) {
			midi.setCh(0);
			midi.out(64);
			isFmove = true;
		}
		else {
			midi.setCh(9);
			midi.out(50);
			isFmove = false;
		}
	}
	~gameCheater() {
		run = false;
	}

	void SetPos(float pos) {
		if (gameisRun) {
			if (posaddress) {
				SIZE_T dwSize;
				if (!WriteProcessMemory(hProcess, (LPVOID)(posaddress + 0x2C), &pos, 4, &dwSize)) {
					return;
				}
			}
			else {
				GetPosAddr();
			}
		}
		else {
			findGame();
		}
	}
	void GetPos(float& pos) {
		if (gameisRun) {
			if (posaddress) {
				SIZE_T dwSize;
				if (!ReadProcessMemory(hProcess, (LPVOID)(posaddress + 0x2C), &pos, 4, &dwSize)) {
					pos = -999;
					return;
				}
			}
			else {
				GetPosAddr();
			}
		}
		else {
			findGame();
		}
	}
	void GetWpAddr() {
		DWORD address = 0;
		SIZE_T dwSize;
		GetModuleBaseAddress(pid, "GameAssembly.dll", address);
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x01C4747C), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x408), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x50), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x70), &address, 4, &dwSize)) {
			return;
		}
		wpaddress = address;
	}

	void GetBgAddr() {
		DWORD address = 0;
		SIZE_T dwSize;
		GetModuleBaseAddress(pid, "GameAssembly.dll", address);
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x01C4747C), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x408), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x14C), &address, 4, &dwSize)) {
			return;
		}
		//HASHSET
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0xC), &address, 4, &dwSize)) {
			return;
		}
		bgddress = address;
	}

	void GetPosAddr() {
		//基地附近是0 float类型;
		DWORD address = 0;
		SIZE_T dwSize;
		GetModuleBaseAddress(pid, "UnityPlayer.dll", address);
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x0127BF00), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x20), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address+ 0x298), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x88), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x5C), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x5C), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0xA0), &address, 4, &dwSize)) {
			return;
		}
		if (!ReadProcessMemory(hProcess, (LPVOID)(address + 0x5C), &address, 4, &dwSize)) {
			return;
		}
		posaddress = address;
	}
private:
	static DWORD WINAPI UpdateAddr(LPVOID lpParamter) {
		gameCheater* obj = (gameCheater*)lpParamter;
		while (obj->run)
		{
			obj->findGame();
			if (obj->gameisRun) {
				obj->GetPosAddr();
				obj->GetWpAddr();
				obj->GetBgAddr();
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
				if (obj->isLockCoin) {
					obj->SetCoin();
				}
				if (obj->isLockDm) {
					obj->SetDm();
				}
				if (obj->isLockBg) {
					obj->SetBg();
				}

				if (obj->isFmove) {
					if(GetAsyncKeyState(0x46) & 0xffff){
						float dr = 0;
						if (GetAsyncKeyState(0x41) & 0xffff || GetAsyncKeyState(0x25) & 0xffff) {
							dr = -1;
						}else if (GetAsyncKeyState(0x44) & 0xffff || GetAsyncKeyState(0x27) & 0xffff) {
							dr = 1;
						}
						if (dr!=0) {
							float pos;
							printf("pos : %f\n", pos);
							obj->GetPos(pos);
							if (pos != -999) {
								pos += dr;
								obj->SetPos(pos);
							}
						}
					}
				}
			}
			Sleep(100);
		}
		return 0;
	}

	void SetCoin() {
		if (gameisRun) {
			if (wpaddress) {
				int coin = 5;
				SIZE_T dwSize;
				if (!WriteProcessMemory(hProcess, (LPVOID)(wpaddress+0x94), &coin, 4, &dwSize)) {
					return;
				}
			}
			else {
				GetWpAddr();
			}
		}
		else {
			findGame();
		}
	}

	void SetBg() {
		if (gameisRun) {
			//难民营列表bgddress地址
			if (bgddress) {
				int count = 0;
				SIZE_T dwSize;
				if (!ReadProcessMemory(hProcess, (LPVOID)(bgddress + 0xC), &count, 4, &dwSize)) {
					return;
				}
				//printf("%d\n", count);

				for (int i = 0; i < 9; i++)
				{
					int addr = 0;
					if (!ReadProcessMemory(hProcess, (LPVOID)(bgddress + 0x10+i*0x4), &addr, 4, &dwSize)) {
						return;
					}

					int mbg = 0;
					if (!ReadProcessMemory(hProcess, (LPVOID)(addr + 0xc), &mbg, 4, &dwSize)) {
						continue;
					}
					//printf("%d\n", mbg);

					if (mbg > 10||mbg<0)continue;
					float d = 1;

					mbg = 10;
					if (!WriteProcessMemory(hProcess, (LPVOID)(addr + 0xc), &mbg, 4, &dwSize)) {
						continue;
					}
					d = 1;
					if (!WriteProcessMemory(hProcess, (LPVOID)(addr + 0x10), &d, 4, &dwSize)) {
						continue;
					}
					if (!WriteProcessMemory(hProcess, (LPVOID)(addr + 0x14), &d, 4, &dwSize)) {
						continue;
					}
					count--;
				}
			}
			else {
				GetBgAddr();
			}
		}
		else {
			findGame();
		}
	}

	void SetDm() {
		if (gameisRun) {
			if (wpaddress) {
				int coin = 5;
				SIZE_T dwSize;
				if (!WriteProcessMemory(hProcess, (LPVOID)(wpaddress+ 0x98), &coin, 4, &dwSize)) {
					return;
				}
			}
			else {
				GetWpAddr();
			}
		}
		else {
			findGame();
		}
	}
	void getAddr(DWORD& address, DWORD offset[], int size) {
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
			wpaddress = 0;
		}
	}
};
