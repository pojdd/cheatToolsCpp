#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <windows.h>

// list all PIDs and TIDs
#include <tlhelp32.h>
#include <Psapi.h>

#include "ntinfo.h"

std::vector<DWORD> threadList(DWORD pid);
DWORD GetThreadStartAddress(HANDLE processHandle, HANDLE hThread);

DWORD getThreadStartAddress(HWND gameHwnd,int num) {
	DWORD dwProcID;
	GetWindowThreadProcessId(gameHwnd, &dwProcID);


	if (!dwProcID) {
		return 0;
	}

	HANDLE hProcHandle = NULL;

	//printf("PID %d (0x%x)\n", dwProcID, dwProcID);
	//std::cout << "Grabbing handle" << std::endl;
	hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);

	if (hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL) {
		//std::cerr << "Failed to open process -- invalid handle" << std::endl;
		//std::cerr << "Error code: " << GetLastError() << std::endl;
		return 0;
	}

	std::vector<DWORD> threadId = threadList(dwProcID);
	int stackNum = 0;
	for (auto it = threadId.begin(); it != threadId.end(); ++it) {
		HANDLE threadHandle = OpenThread(THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, FALSE, *it);
		DWORD threadStartAddress = GetThreadStartAddress(hProcHandle, threadHandle);
		//printf("TID: 0x%04x = THREADSTACK%2d BASE ADDRESS: 0x%04x\n", *it, stackNum, threadStartAddress);
		stackNum++;
	}

	HANDLE threadHandle = OpenThread(THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, FALSE, threadId[num]);
	DWORD threadStartAddress = GetThreadStartAddress(hProcHandle, threadHandle);
	return threadStartAddress;
}

std::vector<DWORD> threadList(DWORD pid) {
	/* solution from http://stackoverflow.com/questions/1206878/enumerating-threads-in-windows */
	std::vector<DWORD> vect = std::vector<DWORD>();
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (h == INVALID_HANDLE_VALUE)
		return vect;

	THREADENTRY32 te;
	te.dwSize = sizeof(te);
	if (Thread32First(h, &te)) {
		do {
			if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) +
				sizeof(te.th32OwnerProcessID)) {


				if (te.th32OwnerProcessID == pid) {
					//printf("PID: %04d Thread ID: 0x%04x\n", te.th32OwnerProcessID, te.th32ThreadID);
					vect.push_back(te.th32ThreadID);
				}

			}
			te.dwSize = sizeof(te);
		} while (Thread32Next(h, &te));
	}

	return vect;
}

DWORD GetThreadStartAddress(HANDLE processHandle, HANDLE hThread) {
	/* rewritten from https://github.com/cheat-engine/cheat-engine/blob/master/Cheat%20Engine/CEFuncProc.pas#L3080 */
	DWORD used = 0, ret = 0;
	DWORD stacktop = 0, result = 0;

	MODULEINFO mi;

	GetModuleInformation(processHandle, GetModuleHandle("kernel32.dll"), &mi, sizeof(mi));
	stacktop = (DWORD)GetThreadStackTopAddress_x86(processHandle, hThread);

	/* The stub below has the same result as calling GetThreadStackTopAddress_x86()
	change line 54 in ntinfo.cpp to return tbi.TebBaseAddress
	Then use this stub
	*/
	//LPCVOID tebBaseAddress = GetThreadStackTopAddress_x86(processHandle, hThread);
	//if (tebBaseAddress)
	//	ReadProcessMemory(processHandle, (LPCVOID)((DWORD)tebBaseAddress + 4), &stacktop, 4, NULL);

	/* rewritten from 32 bit stub (line3141)
	Result: fail -- can't get GetThreadContext()
	*/
	//CONTEXT context;
	//LDT_ENTRY ldtentry;
	//GetModuleInformation(processHandle, LoadLibrary("kernel32.dll"), &mi, sizeof(mi));
	//
	//if (GetThreadContext(processHandle, &context)) {
	//	
	//	if (GetThreadSelectorEntry(hThread, context.SegFs, &ldtentry)) {
	//		ReadProcessMemory(processHandle,
	//			(LPCVOID)( (DWORD*)(ldtentry.BaseLow + ldtentry.HighWord.Bytes.BaseMid << ldtentry.HighWord.Bytes.BaseHi << 24) + 4),
	//			&stacktop,
	//			4,
	//			NULL);
	//	}
	//}

	CloseHandle(hThread);

	if (stacktop) {
		//find the stack entry pointing to the function that calls "ExitXXXXXThread"
		//Fun thing to note: It's the first entry that points to a address in kernel32

		DWORD* buf32 = new DWORD[4096];

		if (ReadProcessMemory(processHandle, (LPCVOID)(stacktop - 4096), buf32, 4096, NULL)) {
			for (int i = 4096 / 4 - 1; i >= 0; --i) {
				if (buf32[i] >= (DWORD)mi.lpBaseOfDll && buf32[i] <= (DWORD)mi.lpBaseOfDll + mi.SizeOfImage) {
					result = stacktop - 4096 + i * 4;
					break;
				}

			}
		}

		delete buf32;
	}

	return result;
}
