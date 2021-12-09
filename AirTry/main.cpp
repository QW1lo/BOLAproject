#include "Lab2.h"
#include "Timer.h"
#include "OPS.h"
#include "integrator.h"
#include "atom.h"
#include "model.h"
#include "OPS.cpp"


int main()
{
	//setlocale(LC_ALL, "rus");

	char buff[10 * 1014];

	// Шаг 1 - иницилизация библиотеки Winsocks
	if (WSAStartup(0x202, (WSADATA*)&buff[0]))
	{
		printf("WSAStartup error: %d\n",
			WSAGetLastError());
		return -1;
	}

	// Шаг 2 - открытие сокета
	_s = socket(AF_INET, SOCK_DGRAM, 0);
	if (_s == INVALID_SOCKET)
	{
		printf("socket() error: %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// Шаг 3 - обмен сообщений с сервером
	HOSTENT* hst;
	_destAddr;
	_destAddr.sin_family = AF_INET;
	_destAddr.sin_port = htons(PORT);
	
	// определение IP-адреса узла
	if (inet_addr(SERVERADDR))
		_destAddr.sin_addr.s_addr = inet_addr(SERVERADDR);
	
	else
	{
		if (hst = gethostbyname(SERVERADDR))
			_destAddr.sin_addr.s_addr = ((unsigned long**)hst->h_addr_list)[0][0];

		else
		{
			printf("Unknown host: %d\n", WSAGetLastError());
			closesocket(_s);
			WSACleanup();
			return -1;
		}
	}

	end.store(false);

	//Точка старта в гео
	//double phi0 = 61.0 * M_PI / 180;					// Для первого маршрута
	//double lambda0 = 62.3231936777456 * M_PI / 180;	//
	//double h0 = 0;									//

	double phi0 = 54.90 * M_PI / 180;					// Для второго маршрута
	double lambda0 = 39.03 * M_PI / 180;				//
	double h0 = 0;										//

	// Начальынй ВС
	Lin::Vector X;
	X = { phi0, lambda0, h0, 0.001, 0 };

	// Массив ппм
	KML_Transformer kml_trns;

	vector<Lin::Vector> vec_coord;
	//vec_coord = parser("pyt.kml");						// Для первого маршрута
	vec_coord = parser("pyt2.kml");					    // Для второого маршрута

	// Характеристики ОПС
	double gam_max = 45;
	double gam_min = -45;
	double th_max = 0;
	double th_min = -90;
	double rng_max = 20000;

	// Цель в гео координатах
	Lin::Vector tar;
	tar = vec_coord[2];

	LA model(X, vec_coord, tar);
	std::cout << "model\n";


	OPS system(&model, gam_max, gam_min, th_max, th_min, rng_max);
	std::cout << "ops\n";

	// СНС
	Sat_NS SNS(&model, 10, 10.0, 10.0, 5.0, 55, 3, 35, 4, 13.3, 6.0, 2.0, 1.0, 21, 10, 18);
	In_NS INS(&model, 33, 55, 130, 15.3, 3.5, 6.3245, 400, 200, 6400, 0, 0, 0);

	TRunge integrator(0, 1000, 1);
	std::cout << "integrator\n";

	Timer timer;
	timer.add(std::chrono::microseconds(500), [&]() {system.get_angles(); });
	timer.add(std::chrono::microseconds(5), [&]() {integrator.integrate(model); });


	timer.add(std::chrono::milliseconds(100), [&]() {SNS.run_sns(); });
	timer.add(std::chrono::milliseconds(1000), [&]() {SNS.send_pack(); });
	timer.add(std::chrono::milliseconds(10), [&]() {INS.send_pack(); });
	timer.add(std::chrono::microseconds(2500), [&]() {INS.run_ins(); });

	while (1) {
		if (end.load()) {
			break;
		}
	}

	std::cout << "done\n";

	kml_trns.CreateKML("result");
	for (int i = 0; i < model.Way.size(); i++)
	{
		kml_trns.KMLNewValue(model.Way[i]);
	}
	std::cout << "kml la writen\n";


	for (int i = 0; i < system.Point.size(); i++)
	{
		kml_trns.KML_ops_point(i, system.Point[i]);
	}
	std::cout << "kml ops writen\n";

}