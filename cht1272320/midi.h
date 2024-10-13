#pragma once
#include <Windows.h>
#pragma comment(lib,"winmm.lib")
class Midi
{
public:
	Midi() {
		velocity = 0x7F;
		channel = 0x90;
		midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
	}
	//������ɫ0-127
	void setTone(int id) {
		int msg =(id << 8) + 0xC0;//�ϳ�msg
		midiOutShortMsg(handle, msg);
	}
	//��������0-127
	void setVel(int vel) {
		velocity = vel;
	}
	//����ͨ��16��ͨ��,GM��׼��ĵ�10ͨ����רΪ������趨�ġ�
	void setCh(int ch) {
		channel = ch+0x90;
	}
	//��Χ0-127����ָ������//��10ͨ����Χ��27-87
	void out(int id) {
		int msg = (velocity << 16) + (id << 8) + channel;//�ϳ�msg
		midiOutShortMsg(handle, msg);//���
	}
	void stop(int id) {//0-127ֹͣ
		int msg = (0 << 16) + (id << 8) + channel;//�ϳ�msg
		midiOutShortMsg(handle, msg);//���
	}
	~Midi() {
		midiOutClose(handle);
	}

private:
	HMIDIOUT handle;
	int velocity;
	int channel;
};

//HMIDIOUT handle;
//midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
//int a = 0x7f, b = 0x3c, c = 0x90;//����velocity ; ����middle C��60Ҳ����0x3C ;ͨ��0x9X x��channel
//int msg = (a << 16) + (b << 8) + c;//�ϳ�msg
//midiOutShortMsg(handle, msg);//���
//Sleep(1000);
//
//a = 0x00, b = 0x12, c = 0xC1;//���� ; �̶�ֵ00 ;�޸�����ָ��
//msg = (a << 16) + (b << 8) + c;//�ϳ�msg
//midiOutShortMsg(handle, msg);
//
//a = 0x7f, b = 0x3c, c = 0x91;//����velocity ; ����middle C��60Ҳ����0x3C ;ͨ��0x9X x��channel
//msg = (a << 16) + (b << 8) + c;//�ϳ�msg
//midiOutShortMsg(handle, msg);//���
//
//Sleep(1000);
//midiOutClose(handle);

/* Open default MIDI Out device */

//if (!midiOutOpen(&handle, (UINT)-1, 0, 0, CALLBACK_NULL))
//{
//    /* Output the C note (ie, sound the note) */
//    midiOutShortMsg(handle, 0x00403C90);
//    Sleep(100);
//    /* Output the E note */
//    midiOutShortMsg(handle, 0x00404091);
//    Sleep(100);
//    /* Output the G note */
//    midiOutShortMsg(handle, 0x00404392);

//    Sleep(100);
//    /* Output the G note */
//    midiOutShortMsg(handle, 0x00404992);

//    /* Here you should insert a delay so that you can hear the notes sounding */
//    Sleep(1000);
//    /* Output the C note (ie, sound the note) */
//    midiOutShortMsg(handle, 0x00404990);
//    Sleep(100);
//    /* Output the E note */
//    midiOutShortMsg(handle, 0x00404391);
//    Sleep(100);
//    /* Output the G note */
//    midiOutShortMsg(handle, 0x00404092);

//    Sleep(100);
//    /* Output the G note */
//    midiOutShortMsg(handle, 0x00403C92);
//    Sleep(1000);

//    /* Now let's turn off those 3 notes */
//    midiOutShortMsg(handle, 0x00003C90);
//    midiOutShortMsg(handle, 0x00004091);
//    midiOutShortMsg(handle, 0x00004392);

//    /* Close the MIDI device */
//    midiOutClose(handle);
//}

//Sleep(1000);
//midiOutClose(handle);