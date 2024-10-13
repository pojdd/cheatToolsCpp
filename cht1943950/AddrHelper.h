#pragma once
#include <windows.h>
#include <tlhelp32.h> 
#include <tchar.h> 
#include <vector>
#include <dbghelp.h>
#pragma comment(lib,"dbghelp")
using namespace std;
PSTR locModuleName;
DWORD64 locAddr = 0;

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
			offiset = i + 1;
			break;
		}
	}
	//printf("%s\n", ModuleName + offiset);
	if (!strcmp(locModuleName, ModuleName + offiset)) {
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
					//printf("here %s %s %x\n", modEntry.szModule, modName,modEntry.modBaseAddr);
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

//class MemObj
//{
//public:
//	MemObj() {
//		point = 0;
//		memsize = 0;
//	}
//	~MemObj() {
//
//	}
//	SIZE_T point;
//	SIZE_T memsize;
//};

class MemManger
{
public:
	MemManger() {
		hProcess = 0;
		allSize = 0;
	}
	~MemManger() {
		//vector <MemObj>().swap(memlist);

	}
	void updata() {
		//memlist.clear();
		if (hProcess) {
			WalkVM();
		}
	}
	void setProcess(HANDLE p) {
		hProcess = p;
	}

	void showAll() {
		//auto signfirst = memlist.begin();
		//auto signend = memlist.end();
		//while (signfirst != signend)
		//{
		//	printf("(%p - %X)\n", signfirst->point, signfirst->memsize);
		//	++signfirst;
		//}
		printf("map\n");
		for (size_t i = 0; i < listsize; i++)
		{
			printf("(%p - %X)\n", memlist[i][0], memlist[i][1]);
		}
	}
	int getMemSize() {
		return allSize;
	}
	//vector <MemObj> memlist;
	//0 地址 1大小
	SIZE_T memlist[20000][2];
	int listsize;
private:
	HANDLE hProcess;
	int allSize;
	void WalkVM() {
		allSize = 0;
		listsize = 0;

		SYSTEM_INFO si;
		ZeroMemory(&si, sizeof(si));
		GetSystemInfo(&si);
		MEMORY_BASIC_INFORMATION mbi;
		ZeroMemory(&mbi, sizeof(mbi));
		LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;
		while (pBlock < si.lpMaximumApplicationAddress) {
			if (VirtualQueryEx(
				hProcess,
				pBlock,
				&mbi,
				sizeof(mbi)
			) == sizeof(mbi)) {
				LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;

				if (mbi.State != MEM_COMMIT) {
					pBlock = pEnd;
					continue;
				}
				if (mbi.Protect != PAGE_READWRITE) {
					pBlock = pEnd;
					continue;
				}
				if (mbi.Type != MEM_PRIVATE) {
					pBlock = pEnd;
					continue;
				}

				if (mbi.RegionSize != 0x10000) {
					pBlock = pEnd;
					continue;
				}

				//MemObj obj;
				//obj.point = (SIZE_T)pBlock;
				//obj.memsize = mbi.RegionSize;
				//memlist.push_back(obj);
				memlist[listsize][0] = (SIZE_T)pBlock;
				memlist[listsize][1] = mbi.RegionSize;
				listsize++;
				allSize += mbi.RegionSize;

				pBlock = pEnd;
			}
			else {
				break;
			}
		}
	}
};

//class SignatureObj
//{
//public:
//	char value;
//	int offset;
//};

class Signature
{
private:
	int offsetnow;
public:
	int baseoffset;
	int maxsize;
	int searchstep = 0x10;
	//vector<SignatureObj> signlist;
	//0 valuse 1 offset
	char signlist[100][2];
	int listsize;
	Signature() {
		maxsize = 0;
		listsize = 0;
		//offsetnow = 0;
	}
	~Signature() {
		//vector <SignatureObj>().swap(signlist);
		listsize = 0;
	}
	void setBase(int offset) {
		baseoffset = offset;
	}

	void addsign(int value, int offset) {
		if (offset > maxsize) {
			maxsize = offset;
		}
		signlist[listsize][0] = value;
		signlist[listsize][1] = offset;
		listsize++;
		offsetnow = offset;
		//SignatureObj obj;
		//obj.value = value;
		//obj.offset = offset;
		//offsetnow = offset;
		//signlist.push_back(obj);
	}

	void operator = (const Signature& sign)
	{
		//signlist.assign(sign.signlist.begin(), sign.signlist.end());
		offsetnow = sign.offsetnow;
		for (size_t i = 0; i < sign.listsize; i++)
		{
			signlist[i][0] = sign.signlist[i][0];
			signlist[i][1] = sign.signlist[i][1];
		}
	}

	void operator = (const int& value)
	{
		//SignatureObj obj;
		//obj.value = value;
		//obj.offset = offsetnow;
		//signlist.push_back(obj);

		signlist[listsize][0] = value;
		signlist[listsize][1] = offsetnow;
		listsize++;
	}

	Signature& operator[](unsigned int i) {
		offsetnow = i;
		if (offsetnow > maxsize) {
			maxsize = offsetnow;
		}
		return *this;
	}

	Signature& operator+=(int value)
	{
		offsetnow++;
		if (offsetnow > maxsize) {
			maxsize = offsetnow;
		}
		//SignatureObj obj;
		//obj.value = value;
		//obj.offset = offsetnow;
		//signlist.push_back(obj);
		signlist[listsize][0] = value;
		signlist[listsize][1] = offsetnow;
		listsize++;
		return *this;
	}

	void showAll() {
		for (size_t i = 0; i < listsize; i++)
		{
			printf("(%X %X)", signlist[i][1] & 0xFF, signlist[i][0] & 0xFF);
		}
		printf("\n");
	}

	void clear() {
		listsize = 0;
		//signlist.clear();
	}
};

class MemSearch
{
public:
	//上次验证是否成功
	int lasttest;
	MemSearch() {
		manger.setProcess(hProcess);
	}
	~MemSearch() {

	}
	void updataMemMap() {
		manger.updata();
	}

	int getAddrNum(Signature signature, int isshowAddr) {
		char* buffer;
		int Numb = 0;
		//printf("listsize %d\n", manger.listsize);
		for (size_t i = 0; i < manger.listsize; i++)
		{
			SIZE_T Size;
			buffer = new char[manger.memlist[i][1] + 1];
			//获取区块
			//printf("%p\n", first->point);
			//printf("%p\n", first->memsize);
			if (!ReadProcessMemory(hProcess
				, (LPVOID)(manger.memlist[i][0])
				, buffer
				, manger.memlist[i][1]
				, &Size
			)) {
				delete[]buffer;
				continue;
			}
			//printf("%p %X\n", manger.memlist[i][0], manger.memlist[i][1]);

			//遍历寻找地址。
			//#pragma omp parallel
			int offset = 0;
			for (offset = 0; offset < manger.memlist[i][1]; offset += signature.searchstep)
			{
				if (confirm(signature, buffer, manger.memlist[i][1], offset)) {
					if (isshowAddr) {
						printf("%p\n", manger.memlist[i][0] + offset + signature.baseoffset);
					}
					Numb++;
					break;
				}
			}

			delete[]buffer;
		}
		return Numb;
	}
	//看看地址对不对
	bool verifyAddr(Signature signature, SIZE_T Addr) {
		SIZE_T ad = Addr - signature.baseoffset;
		char* buffer = new char[signature.maxsize + 1];
		if (!ReadProcessMemory(hProcess
			, (LPVOID)(ad)
			, buffer
			, signature.maxsize + 1
			, 0
		)) {
			delete[]buffer;
			return false;
		}

		bool ret = confirm(signature, buffer, signature.maxsize, 0);
		delete[]buffer;
		lasttest = ret;
		return ret;
	}

	LPCVOID getAddr(Signature signature) {
		char* buffer;
		for (size_t i = 0; i < manger.listsize; i++)
		{
			SIZE_T Size;
			buffer = new char[manger.memlist[i][1] + 1];
			//获取区块
			//printf("%p\n", first->point);
			//printf("%p\n", first->memsize);
			if (!ReadProcessMemory(hProcess
				, (LPVOID)(manger.memlist[i][0])
				, buffer
				, manger.memlist[i][1]
				, &Size
			)) {
				delete[]buffer;
				continue;
			}

			//遍历寻找地址。
			bool find = 0;
			int offset = 0;
			//#pragma omp parallel
			for (offset = 0; offset < manger.memlist[i][1]; offset += signature.searchstep)
			{
				if (confirm(signature, buffer, manger.memlist[i][1], offset)) {
					find = 1;
					break;
				}
			}

			delete[]buffer;

			if (find) {
				return LPCVOID(manger.memlist[i][0] + offset + signature.baseoffset);
			}
		}
		//没有找到所以没办法
		return 0;
	}
	void setProcess(HANDLE p) {
		hProcess = p;
		manger.setProcess(p);
	}
	void showMemMap() {
		manger.showAll();
	}
	int getMemBlockSize() {
		return manger.listsize;
	}
	int getMemSize() {
		return manger.getMemSize();
	}
private:
	HANDLE hProcess;
	MemManger manger;
	bool confirm(Signature sign, char* buffer, int size, int offset) {
		for (size_t i = 0; i < sign.listsize; i++)
		{
			int off = offset + sign.signlist[i][1];

			//越界无效
			if (off < 0 || off > size) {
				return false;
			}
			//对比失败
			if (sign.signlist[i][0] != buffer[off]) {
				return false;
			}

			//printf("(%X %X)", sign.signlist[i][1] & 0xFF, sign.signlist[i][0] & 0xFF);
		}
		return true;
	}
};

