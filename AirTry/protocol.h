#pragma once
//#include <map>

#pragma pack(push,1)
union ARINC_BNR {
    unsigned int Value;
    struct data {
        unsigned adress : 8;
        unsigned value : 20;
        unsigned sign : 1;
        unsigned SM : 2;
        unsigned P : 1;
    } bits;
};
#pragma pack(pop)

#pragma pack(push,1)
union ARINC_BCD {
    unsigned int Value;
    struct data {
        unsigned adress : 8;
        unsigned empty1 : 3;
        unsigned s : 6;
        unsigned m : 6;
        unsigned h : 5;
        unsigned empty2 : 1;
        unsigned SM : 2;
        unsigned P : 1;
    } bits;
};
#pragma pack(pop)

#pragma pack(push,1)
union ARINC_DISCRETE {
    unsigned int Value;
    struct data {
        unsigned adress : 8;
        unsigned SDI : 2;
        unsigned preparation : 1;
        unsigned control : 1;
        unsigned navigation : 1;
        unsigned gyro : 1;
        unsigned empty1 : 1;
        unsigned re_start : 1;
        unsigned scale : 3;
        unsigned heat : 1;
        unsigned therm : 1;
        unsigned init_data : 1;
        unsigned rec_H : 1;
        unsigned INS : 1;
        unsigned acceleration : 1;
        unsigned ready : 1;
        unsigned empty2 : 3;
        unsigned SM : 2;
        unsigned P : 1;
    } bits;
};
#pragma pack(pop)

union ARINC_DATE {
    unsigned int Value;
    struct data {
        unsigned adress : 8;
        unsigned empty1 : 2;
        unsigned Y_u : 4;  
        unsigned Y_d : 4;
        unsigned M_u : 4;
        unsigned M_d : 1;
        unsigned D_u : 4;
        unsigned D_d : 1;
        unsigned empty2 : 1;    
        unsigned SM : 2;
        unsigned P : 1;
    } bits;
};

#pragma pack(push,1)
union ARINC_FEATURES {
    unsigned int Value;
    struct data {
        unsigned adress : 8;
        unsigned R_data : 1;
        unsigned type_sns : 3;
        unsigned almanach_gps : 1;
        unsigned almanach_glo : 1;
        unsigned mode : 2;
        unsigned submode : 1;
        unsigned time_f : 1;
        unsigned empty : 2;
        unsigned diff : 1;
        unsigned empty1 : 1;
        unsigned reject : 1;
        unsigned signal_step : 1;
        unsigned SK : 2;
        unsigned empty2 : 3;
        unsigned SM : 2;
        unsigned P : 1;
    } bits;
};
#pragma pack(pop)

#pragma pack(push,1)
struct Plot_Python 
{
    int Number;     // Борт номер
    double phi;        // Широта
    double lbd;        // Долгота
};
#pragma pack(pop)


//struct Value {
//	std::string name;
//	int height;
//	double max_value;
//
//};

//Value latitude = { "широта", 20, 90 };
//Value longitude = { "долгота", 20, 90 };
//Value height = { "высота", 19, 19975.3728 };
//Value course = { "курс истинный", 16, 90 };
//Value pitch = { "тангаж", 16, 90 };
//Value roll = { "крен", 16, 90 };
//Value velocityNS = { "скорость север юг", 19, 1053.5822 };
//Value velocityEW = { "скорость восток запад", 19, 1053.5822 };
//Value velocity = { "скорость вертикальная инерциальная", 19, 83.2307 };
//Value ax = { "ускорение продольное", 12, 19.62 };
//Value az = { "ускорение поперечное", 12, 19.62 };
//Value ay = { "ускорение нормальное", 12, 2 };
//Value word_state = { "слово БИНС", 32, 0 };
//
//Value height2 = { "высота", 20, 65536 };
//Value HDOP = { "HDOP", 15, 512 };
//Value VDOP = { "VDOP", 15, 512 };
//Value trackangle = { "путевой угол", 15, 90 };
//Value curlatitude = { "текущая широта ", 20, 90 };
//Value curlatitude1 = { "текущая широта (точно)", 11, 0.000085830 };
//Value curlongitude = { "текущая долгота", 20, 90 };
//Value curlongitude1 = { "текущая долгота (точно) ", 11, 0.000085830 };
//Value delay = { "задержка выдачи", 20, 512 };
//Value utс = { "текущее время UTC (старшие разряды)", 6, 32 };
//Value utс1 = { "текущее время UTC (младшие разряды)", 20, 512 };
//Value Vh = { "вертикальная скорость", 15, 16384 };
//Value data = { "дата", 32, 0 };
//Value word_state1 = { "слово СНС", 32, 0 };
//
//
//std::map <int, struct Value > table = { {310 - 256, latitude }, {311 - 256, longitude},
//								  {361 - 256, height},    {314 - 256, course},
//								  {324 - 256, pitch},     {325 - 256, roll },
//								  {366 - 256, velocityNS},{367 - 256, velocityEW},
//								  {365 - 256, velocity }, {331 - 256, ax},
//								  {332 - 256, az},        {333 - 256, ay},
//								  {76, height2},    {101, HDOP},
//								  {102, VDOP},      {103, trackangle},
//								  {110, curlatitude},{120, curlatitude1},
//								  {111, curlongitude},{121, curlongitude1},
//								  {113, delay},     {150, utс},
//								  {140, utс1},      {165, Vh},
//								  {270 - 256, word_state},{273 - 256, word_state1},
//								  {260 - 256, data} };

