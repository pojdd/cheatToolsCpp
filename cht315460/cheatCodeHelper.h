#pragma once
#include "AddrHelper.h"
#include <vector>
using namespace std;

//ÿ����Ϸ��Ӧһ����Ϸ����һ����Ϸ������Ҫһ���̡߳�
class GameObj;

//��������,һ����Ϸ�������ӵ�ж����������
class CheatWork;


//ÿ����Ϸ����һ����Ϸ����һ����Ϸ������Ҫһ���̡߳�
class GameObj
{
	char * pname = 0;//��Ϸ������,����������Ϸ
	bool run = 1;//�̹߳رձ�־
	DWORD pid;
	HANDLE hProcess = 0;
	bool gameRun = 0;//��־��Ϸ�Ƿ�����

	//��Ϸ����������б�
	vector <CheatWork> workList;

	//�����̣߳������Ϸ״̬,�����������
	static DWORD WINAPI Update(LPVOID lpParamter) {
		GameObj* obj =(GameObj*) lpParamter;
		while (obj->run)
		{
			bool t = true;
			if (t) {
				t = FindProcessPid(obj->pname, obj->pid);//��ȡ��Ϸ����pid
			}
			if (t) {
				obj->hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, obj->pid);//��ȡ��Ϸ����
			}
			if (t) {//��Ϸ����
				obj->gameRun = true;
			}
			else {//��Ϸ�ر�
				obj->gameRun = false;
			}
			Sleep(100);
		}
	}
public:
	GameObj(char * name) {
		pname = name;
		//�������ݸ����߳�
		HANDLE hThread = CreateThread(NULL, 0, Update, this, 0, NULL);
		CloseHandle(hThread);
	}
	~GameObj() {
		run = 0;
	}

private:

};

//���׷���
//1.����ģ���ַ������ָ���Ӧ��ֵ
//2.����ģ���ַ���޸�ָ��ָ��ĵ�ַ��Ӧ�Ĵ���
//3.�޸������������ƫ�ƺ��Ӧ�ĵ�ַ�Ĵ���

//���ײ���,������
enum WorkStep
{

};
//��������
class CheatWork
{
	bool isActive = 0;//��־���������Ƿ񼤻�
	bool isChange = 0;//��־״̬�Ƿ�ı�
	bool isUpdata = 1;//�Ƿ���Ҫ����
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
