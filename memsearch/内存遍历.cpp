#include "iostream"
#include "windows.h"
#include "shlwapi.h"
#include  "iomanip"
#include "AddrHelper.h"

#pragma comment(lib,"shlwapi.lib")
using namespace std;

inline bool TestSet(DWORD dwTarget, DWORD dwMask) {
    return ((dwTarget & dwMask) == dwMask);
}

#define SHOWMASK(dwTarget,type)\
    if(TestSet(dwTarget,PAGE_##type))\
{cout<<","<<#type;}

void ShowProtection(DWORD dwTarget)
{
    SHOWMASK(dwTarget, READONLY);
    SHOWMASK(dwTarget, GUARD);
    SHOWMASK(dwTarget, NOCACHE);
    SHOWMASK(dwTarget, READWRITE);
    SHOWMASK(dwTarget, WRITECOPY);
    SHOWMASK(dwTarget, EXECUTE);
    SHOWMASK(dwTarget, EXECUTE_READ);
    SHOWMASK(dwTarget, EXECUTE_READWRITE);
    SHOWMASK(dwTarget, EXECUTE_WRITECOPY);
    SHOWMASK(dwTarget, NOACCESS);
}
int index = 0;
SIZE_T memsize = 0;
void WalkVM(HANDLE hProcess) {
    SYSTEM_INFO si;
    ZeroMemory(&si, sizeof(si));
    GetSystemInfo(&si);

    MEMORY_BASIC_INFORMATION mbi;
    ZeroMemory(&mbi, sizeof(mbi));
    //000001BC91730000 - 10000 Committed, READWRITE, Private
    LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;
    while (pBlock < si.lpMaximumApplicationAddress) {
        if (VirtualQueryEx(
            hProcess,
            pBlock,
            &mbi,
            sizeof(mbi)
        ) == sizeof(mbi)) {
            LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
            //TCHAR szSize[MAX_PATH];
            //StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);
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
            printf("%p-%X ", pBlock, mbi.RegionSize);
            index++;
            memsize += mbi.RegionSize;

            //cout.fill('0');
            //cout << hex << setw(16) << (DWORD)pBlock
            //    << "-"
            //    << hex << setw(16) << (DWORD)mbi.RegionSize
            //    << (strlen(szSize) == 7 ? "(" : "(") << szSize
            //    << ")";

            //switch (mbi.State) {
            //case MEM_COMMIT:
            //    printf("Committed");
            //    break;
            //case MEM_FREE:
            //    printf("Free");
            //    break;
            //case MEM_RESERVE:
            //    printf("Reserved");
            //    break;

            //}

            //if (mbi.Protect == 0 && mbi.State != MEM_FREE) {
            //    mbi.Protect = PAGE_READONLY;
            //}
            //ShowProtection(mbi.Protect);

            //switch (mbi.Type) {
            //case MEM_IMAGE:
            //    printf(",Image");
            //    break;
            //case MEM_MAPPED:
            //    printf(",Mapped");
            //    break;
            //case MEM_PRIVATE:
            //    printf(",Private");
            //    break;
            //}

            //TCHAR szFilename[MAX_PATH];
            //if (GetModuleFileName((HMODULE)pBlock,
            //    szFilename,
            //    MAX_PATH
            //) > 0) {
            //    PathStripPath(szFilename);
            //    printf(",Module:%s", szFilename);
            //}
            printf("\n");
            pBlock = pEnd;
        }
    }
}
char* className = "UnityWndClass";
char* capName = "Circle Empires Rivals";
DWORD pid;

int main() {
    HWND gameHwnd = FindWindow(className, capName);
    GetWindowThreadProcessId(gameHwnd, &pid);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);

    //Sleep(100000);

    MemSearch memsearch;
    memsearch.setProcess(hProcess);
    memsearch.updataMemMap();
    memsearch.showMemMap();
    //printf("memsize:%d\n", memsearch.getMemSize());
    //printf("memsize:%d\n", memsearch.getMemBlockSize());
    //Sleep(100000);
    Signature sign;
    sign.searchstep = 0x10;

    //最后用于计算偏移。不参与搜索。特征码和目标内存的地址偏移。特征码地址+Base=最后结果
    //例如特征使用1C7B78A0作为起始地址记录偏移，真实地址是1C7B78A8则Base是0x8。
    sign.setBase(0x8);

    //从任意个位是0的地址开始记为偏移。
    sign[0x4] = 0x04;
    sign[0x8] = 0xF3;

    LPCVOID addr;
    addr=memsearch.getAddr(sign);
    int start = clock();
    int num=memsearch.getAddrNum(sign,1);
    int end = clock();
    printf("time :%d\n", end - start);
    printf("num :%d\n", num);
    printf("\n");
    Sleep(1000000);
    while (true)
    {
        int num = memsearch.getAddrNum(sign,1);
        int start = clock();
        if (memsearch.verifyAddr(sign, (SIZE_T)addr)) {
            printf("ok\n");
        }
        else {
            addr = memsearch.getAddr(sign);
            printf("research\n");
        }
        int end = clock();
        printf("%p\n", addr);
        printf("time: %d ms\n", end - start);
        printf("num %d\n", num);
        printf("\n");
        Sleep(1000);
    }

    while (true)
    {
        getchar();
    }
    return 0;
}
