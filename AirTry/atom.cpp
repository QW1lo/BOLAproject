#include "atom.h"

SOCKET _s;
sockaddr_in _destAddr;
std::atomic<bool> end;
std::mutex mut;

Value latitude = { "широта", 20, 90 };
Value longitude = { "долгота", 20, 90 };
Value height = { "высота", 19, 19975.3728 };
Value course = { "курс истинный", 16, 90 };
Value pitch = { "тангаж", 16, 90 };
Value roll = { "крен", 16, 90 };
Value velocityNS = { "скорость север юг", 19, 1053.5822 };
Value velocityEW = { "скорость восток запад", 19, 1053.5822 };
Value velocity = { "скорость вертикальная инерциальная", 19, 83.2307 };
Value ax = { "ускорение продольное", 12, 19.62 };
Value az = { "ускорение поперечное", 12, 19.62 };
Value ay = { "ускорение нормальное", 12, 2 };
Value word_state = { "слово БИНС", 32, 0 };

Value height2 = { "высота", 20, 65536 };
Value HDOP = { "HDOP", 15, 512 };
Value VDOP = { "VDOP", 15, 512 };
Value trackangle = { "путевой угол", 15, 90 };
Value curlatitude = { "текущая широта ", 20, 90 };
Value curlatitude1 = { "текущая широта (точно)", 11, 0.000085830 };
Value curlongitude = { "текущая долгота", 20, 90 };
Value curlongitude1 = { "текущая долгота (точно) ", 11, 0.000085830 };
Value delay = { "задержка выдачи", 20, 512 };
Value utс = { "текущее время UTC (старшие разряды)", 6, 32 };
Value utс1 = { "текущее время UTC (младшие разряды)", 20, 512 };
Value Vh = { "вертикальная скорость", 15, 16384 };
Value data = { "дата", 32, 0 };
Value word_state1 = { "слово СНС", 32, 0 };


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
								  {113, delay},     {150, utс},
								  {140, utс1},      {165, Vh},
								  {270 - 256, word_state},{273 - 256, word_state1},
								  {260 - 256, data} };
