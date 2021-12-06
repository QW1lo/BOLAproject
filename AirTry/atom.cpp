#include "atom.h"

SOCKET _s;
sockaddr_in _destAddr;
std::atomic<bool> end;
std::mutex mut;

Value latitude = { "������", 20, 90 };
Value longitude = { "�������", 20, 90 };
Value height = { "������", 19, 19975.3728 };
Value course = { "���� ��������", 16, 90 };
Value pitch = { "������", 16, 90 };
Value roll = { "����", 16, 90 };
Value velocityNS = { "�������� ����� ��", 19, 1053.5822 };
Value velocityEW = { "�������� ������ �����", 19, 1053.5822 };
Value velocity = { "�������� ������������ ������������", 19, 83.2307 };
Value ax = { "��������� ����������", 12, 19.62 };
Value az = { "��������� ����������", 12, 19.62 };
Value ay = { "��������� ����������", 12, 2 };
Value word_state = { "����� ����", 32, 0 };

Value height2 = { "������", 20, 65536 };
Value HDOP = { "HDOP", 15, 512 };
Value VDOP = { "VDOP", 15, 512 };
Value trackangle = { "������� ����", 15, 90 };
Value curlatitude = { "������� ������ ", 20, 90 };
Value curlatitude1 = { "������� ������ (�����)", 11, 0.000085830 };
Value curlongitude = { "������� �������", 20, 90 };
Value curlongitude1 = { "������� ������� (�����) ", 11, 0.000085830 };
Value delay = { "�������� ������", 20, 512 };
Value ut� = { "������� ����� UTC (������� �������)", 6, 32 };
Value ut�1 = { "������� ����� UTC (������� �������)", 20, 512 };
Value Vh = { "������������ ��������", 15, 16384 };
Value data = { "����", 32, 0 };
Value word_state1 = { "����� ���", 32, 0 };


std::map <int, struct Value > table = { {310 - 256, latitude }, {311 - 256, longitude},
								  {361 - 256, height},    {314 - 256, course},
								  {324 - 256, pitch},     {325 - 256, roll },
								  {366 - 256, velocityNS},{367 - 256, velocityEW},
								  {365 - 256, velocity }, {331 - 256, ax},
								  {332 - 256, az},        {333 - 256, ay},
								  {76, height2},    {101, HDOP},
								  {102, VDOP},      {103, trackangle},
								  {110, curlatitude},{120, curlatitude1},
								  {111, curlongitude},{121, curlongitude1},
								  {113, delay},     {150, ut�},
								  {140, ut�1},      {165, Vh},
								  {270 - 256, word_state},{273 - 256, word_state1},
								  {260 - 256, data} };
