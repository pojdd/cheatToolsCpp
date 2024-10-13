#pragma once
#include <windows.h>
#include <tlhelp32.h> 
#include <tchar.h> 
#include "atlbase.h"
#include "atlstr.h"
#include <dbghelp.h>
#pragma comment(lib,"dbghelp")
PSTR locModuleName;
DWORD64 locAddr=0;

BOOL CALLBACK EnumerateLoadedModulesProc(
	PSTR ModuleName,
	DWORD64 ModuleBase,
	ULONG ModuleSize,
	PVOID UserContext)
{
	//printf("%s\n", locModuleName);
	//printf("%#llx %s\n", ModuleBase, ModuleName);
	int len = strlen(ModuleName);
	int offiset = 0;
	for (int i = len - 1; i > 1; i--)
	{
		if (ModuleName[i] == '\\') {
			offiset = i+1;
			break;
		}
	}
	//printf("%s\n", ModuleName + offiset);
	if (!strcmp(locModuleName, ModuleName+offiset)) {
		locAddr = ModuleBase;
	}
	return TRUE;
}
void GetModuleBaseAddress64(HANDLE hProcess, PSTR ModuleName, DWORD64& addr) {
	locModuleName = ModuleName;
	EnumerateLoadedModules(hProcess, (PENUMLOADED_MODULES_CALLBACK)EnumerateLoadedModulesProc, 0);
	addr = locAddr;
}

BOOL FindProcessPid(char* ProcessName, DWORD& dwPid)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return(FALSE);
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
	}
	BOOL    bRet = FALSE;
	do
	{
		if (!strcmp(ProcessName, pe32.szExeFile))
		{
			dwPid = pe32.th32ProcessID;
			bRet = TRUE;
			break;
		}

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return bRet;
}
void GetModuleBaseAddress(DWORD procId, const char* modName, DWORD& addr)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				//printf("%ls %x\n", modEntry.szModule, modEntry.modBaseAddr);
				if (!strcmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					addr = modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	//return modBaseAddr;
}

BOOL isUp(int a) {
	return (GetAsyncKeyState(a) & 0xffff);
}