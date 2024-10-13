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
	//设置音色0-127
	void setTone(int id) {
		int msg =(id << 8) + 0xC0;//合成msg
		midiOutShortMsg(handle, msg);
	}
	//设置音量0-127
	void setVel(int vel) {
		velocity = vel;
	}
	//设置通道16个通道,GM标准里的第10通道是专为打击乐设定的。
	void setCh(int ch) {
		channel = ch+0x90;
	}
	//范围0-127播放指定音阶//第10通道范围是27-87
	void out(int id) {
		int msg = (velocity << 16) + (id << 8) + channel;//合成msg
		midiOutShortMsg(handle, msg);//输出
	}
	void stop(int id) {//0-127停止
		int msg = (0 << 16) + (id << 8) + channel;//合成msg
		midiOutShortMsg(handle, msg);//输出
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
//int a = 0x7f, b = 0x3c, c = 0x90;//音量velocity ; 音阶middle C是60也就是0x3C ;通道0x9X x是channel
//int msg = (a << 16) + (b << 8) + c;//合成msg
//midiOutShortMsg(handle, msg);//输出
//Sleep(1000);
//
//a = 0x00, b = 0x12, c = 0xC1;//乐器 ; 固定值00 ;修改乐器指令
//msg = (a << 16) + (b << 8) + c;//合成msg
//midiOutShortMsg(handle, msg);
//
//a = 0x7f, b = 0x3c, c = 0x91;//音量velocity ; 音阶middle C是60也就是0x3C ;通道0x9X x是channel
//msg = (a << 16) + (b << 8) + c;//合成msg
//midiOutShortMsg(handle, msg);//输出
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