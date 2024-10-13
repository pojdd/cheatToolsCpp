#pragma once
#include "AddrHelper.h"
#include <vector>
using namespace std;

//每个游戏对应一个游戏对象，一个游戏对象需要一个线程。
class GameObj;

//作弊任务,一个游戏对象可以拥有多个作弊任务
class CheatWork;


//每个游戏对于一个游戏对象，一个游戏对象需要一个线程。
class GameObj
{
	char * pname = 0;//游戏进程名,用以锁定游戏
	bool run = 1;//线程关闭标志
	DWORD pid;
	HANDLE hProcess = 0;
	bool gameRun = 0;//标志游戏是否运行

	//游戏对象的作弊列表
	vector <CheatWork> workList;

	//作弊线程，监控游戏状态,完成作弊任务
	static DWORD WINAPI Update(LPVOID lpParamter) {
		GameObj* obj =(GameObj*) lpParamter;
		while (obj->run)
		{
			bool t = true;
			if (t) {
				t = FindProcessPid(obj->pname, obj->pid);//获取游戏进程pid
			}
			if (t) {
				obj->hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, obj->pid);//获取游戏进程
			}
			if (t) {//游戏开启
				obj->gameRun = true;
			}
			else {//游戏关闭
				obj->gameRun = false;
			}
			Sleep(100);
		}
	}
public:
	GameObj(char * name) {
		pname = name;
		//开启数据更新线程
		HANDLE hThread = CreateThread(NULL, 0, Update, this, 0, NULL);
		CloseHandle(hThread);
	}
	~GameObj() {
		run = 0;
	}

private:

};

//作弊方法
//1.利用模块地址，锁定指针对应的值
//2.利用模块地址，修改指针指向的地址对应的代码
//3.修改搜索特征码加偏移后对应的地址的代码

//作弊步骤,或类型
enum WorkStep
{

};
//作弊任务
class CheatWork
{
	bool isActive = 0;//标志作弊任务是否激活
	bool isChange = 0;//标志状态是否改变
	bool isUpdata = 1;//是否需要更新
public:
	CheatWork() {
		isActive = false;
	}
	~CheatWork() {

	}
	void start() {
		if (!isActive) {
			isChange = 1;
			isActive = 1;
		}
	}
	void stop() {
		if (isActive) {
			isChange = 1;
			isActive = 0;
		}
	}

private:

};
