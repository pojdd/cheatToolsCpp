#pragma once
#include <Windows.h>
#include "AddrHelper.h"
#include "midi.h"
#include <stdio.h>


char buttonName[][20] = {
	"��Դ�������",
};
char labelName[][50] = {
	"CTRL+ATL+F1",
};
const int checkbn = 1;
char wintitle[] = "Ư������0.4.1�޸���";
const char upurl[] = "https://www.bilibili.com/video/BV1um4y1i7bm/";

class gameCheater
{
	char* className = "UnityWndClass";
	char* capName = "The Wandering Village";
	HWND gameHwnd = 0;//���ھ��
	HANDLE hProcess = 0;
	DWORD pid;
	SIZE_T HPaddress = 0;//λ�õ�ַ
	SIZE_T PWaddress = 0;//λ�õ�ַ
	SIZE_T SANaddress=0;//λ�õ�ַ
	bool run;//�����Ƿ�����
	MemSearch memsearch;//�ڴ�������
	Signature SubsignEnable;//������Դ��������
public:
	Midi midi;
	bool gameisRun = false;
	int LockState[9] = { 0 };
	gameCheater() {
		SubsignEnable.setBase(2);
		SubsignEnable[0x2] = 0x2B;
		SubsignEnable += 0xC7;
		SubsignEnable += 0x41;
		SubsignEnable += 0x89;
		SubsignEnable += 0x46;
		SubsignEnable += 0x0C;
		SubsignEnable += 0x49;
		SubsignEnable += 0x63;
		SubsignEnable += 0x46;
		SubsignEnable += 0x14;

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

	void GetPOWERAddr() {
		if (!memsearch.verifyAddr(SubsignEnable, PWaddress)) {
			SubsignEnable.signlist[0][0] = 0x2B;
			SubsignEnable.signlist[1][0] = 0xC7;
			PWaddress = (DWORD64)memsearch.getAddr(SubsignEnable);
		}
		//�������Ѿ��޸���,���޸ĺ����������
		if (PWaddress == 0) {
			SubsignEnable.signlist[0][0] = 0x90;
			SubsignEnable.signlist[1][0] = 0x90;
			if (!memsearch.verifyAddr(SubsignEnable, PWaddress)) {
				PWaddress = (DWORD64)memsearch.getAddr(SubsignEnable);
			}
		}
		//printf("%p\n", PWaddress);
	}

	void EnablePOWER(int enable) {
		SIZE_T dwSize;
		if (memsearch.verifyAddr(SubsignEnable, PWaddress)) {
			if (enable) {
				//printf("write1 %2X\n", SubsignEnable.signlist[0][0]);
				//�����δ����������
				if ((SubsignEnable.signlist[0][0] & 0xFF) == 0x2B) {
					char data[2] = { 0x90,0x90 };
					WriteProcessMemory(hProcess, (LPVOID)(PWaddress), data, 2, &dwSize);
					//printf("write1\n");
				}
			}
			else {
				//printf("write2 %2X\n", SubsignEnable.signlist[0][0]);
				if ((SubsignEnable.signlist[0][0] & 0xFF) == 0x90) {
					char data[2] = { 0x2B,0xC7 };
					WriteProcessMemory(hProcess, (LPVOID)(PWaddress), data, 2, &dwSize);
					//printf("write2\n");
				}
			}
		}
		
		//SIZE_T dwSize;
		//float Hp = 0;
		//if (!ReadProcessMemory(hProcess, (LPVOID)(PWaddress), &Hp, 4, &dwSize)) {
		//	return;
		//}
		//if (Hp < 10 || Hp > 50) {
		//	return;
		//}
		//Hp = 35;
		//if (!WriteProcessMemory(hProcess, (LPVOID)(PWaddress), &Hp, 4, &dwSize)) {
		//	return;
		//}
	}

private:
	static DWORD WINAPI UpdateAddr(LPVOID lpParamter) {
		gameCheater* obj = (gameCheater*)lpParamter;
		while (obj->run)
		{
			obj->findGame();
			if (!obj->memsearch.lasttest) {
				obj->memsearch.updataMemMap();
			}
			if (obj->gameisRun) {
				obj->GetPOWERAddr();
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
				obj->EnablePOWER(obj->LockState[0]);
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
				memsearch.setProcess(hProcess);
				memsearch.updataMemMap();
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
