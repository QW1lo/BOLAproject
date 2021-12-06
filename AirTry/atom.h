#pragma once

#include <WinSock2.h>
#include <atomic>
#include <mutex>
#include <map>
#pragma comment(lib,"Ws2_32.lib")
#define PORT 12346
#define SERVERADDR "127.0.0.1"
#pragma warning(disable: 4996)

extern SOCKET _s;
extern sockaddr_in _destAddr;
extern std::atomic<bool> end;
extern std::mutex mut;

struct Value {
	std::string name;
	int height;
	double max_value;

};

extern Value latitude;
extern Value longitude;
extern Value height;
extern Value course;
extern Value pitch;
extern Value roll;
extern Value velocityNS;
extern Value velocityEW;
extern Value velocity;
extern Value ax;
extern Value az;
extern Value ay;
extern Value word_state;

extern Value height2;
extern Value HDOP;
extern Value VDOP;
extern Value trackangle;
extern Value curlatitude;
extern Value curlatitude1;
extern Value curlongitude;
extern Value curlongitude1;
extern Value delay;
extern Value utñ;
extern Value utñ1;
extern Value Vh;
extern Value data;
extern Value word_state1;


extern std::map <int, struct Value > table;