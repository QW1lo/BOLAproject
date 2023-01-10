#include "Lab2.h"
#include "Timer.h"
#include "OPS.h"
#include "integrator.h"
#include "atom.h"
#include "model.h"
#include "TCAS.h"
#include "OPS.cpp"
#include "Bomb.h"

void fill_struct_py(LA& obj, Plot_Python& struc)
{
	struc.Number = obj.N;
	
	Lin::Vector geo;
	Lin::Vector tsk;
	tsk = { obj.X[0],  0 , obj.X[2] };
	geo = obj.TSK_to_Geo(tsk, 0) * 180 / M_PI;
	geo[2] = obj.X[1];

	struc.phi = geo[1];
	struc.lbd = geo[0];
	tsk[1] = obj.X[1];

	struc.D = tsk.length();
	struc.Az = atan2(tsk[2], tsk[0]);
	struc.h = obj.X[1];
	struc.delY = obj.del_glissade[1];
	struc.X = obj.X[0];
	struc.Z = obj.X[2];

	struc.V = obj.X[3];
	struc.PSI = obj.X[4];
	struc.vx = obj.dvxz[0];
	struc.vz = obj.dvxz[1];

	struc.TCAS = obj.TCAS;
}


int main()
{
	std::cout << "HI\n";
	
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

	// Начальные координаты ВПП Аэропорта

	double phiL = 56.1448638889 * M_PI / 180;
	double lambdaL = 34.9926805556 * M_PI / 180;
	double hL = 0;
	double KL = (67.05457948536832 ) * M_PI / 180;		// Курс ВПП
	double thetaL = 3 * GR2RAD;
	thetaL = 0 * GR2RAD;
	// Начальные координаты ЛА

	//Точка старта в гео
	//double phi0 = 61.0 * M_PI / 180;					// Для первого маршрута
	//double lambda0 = 62.3231936777456 * M_PI / 180;	//
	//double h0 = 0;									//

	double phi0 = 54.90 * M_PI / 180;					// Для второго маршрута
	double lambda0 = 39.03 * M_PI / 180;				//
	double h0 = 0;										//



	double phi1 = 56.077251 * M_PI / 180;					// Первый ЛА ВКР
	double lambda1 = 34.828938 * M_PI / 180;				//
	double h1 = 2500;										//

	double phi2 = 56.08094624641132 * M_PI / 180;					// Первый ЛА ВКР
	double lambda2 = 35.32249558691617 * M_PI / 180;				//
	double h2 = 2500;												//

	Lin::Vector X_land;
	//X = { phi0, lambda0, h0, 0.001, 0 };
	X_land = { phiL, lambdaL, hL, 0, KL };

	// Начальный ВС : {phi0, lbd0, h0, V0, PSI0}
	Lin::Vector X;
	X = { phi0, lambda0, h0, 50, M_PI - KL + 2 * GR2RAD};
	
	Lin::Vector X_VKR1; // -25 1
	Lin::Vector X_VKR2; // 80 3
	Lin::Vector X_VKR3; 
	Lin::Vector X_VKR4; 
	Lin::Vector X_VKR6; 
	Lin::Vector X_VKR5; 

	X_VKR1 = { phi1, lambda1, h1, 200, M_PI / 180 * 0 };
	X_VKR2 = { phi2, lambda2, h2, 200, M_PI / 180 * 120 };
	X_VKR3 = { phi1, lambda1, h1, 200, M_PI / 180 * -120 };
	X_VKR4 = { phi2, lambda2, h2, 200, M_PI / 180 * 0 };
	X_VKR5 = { phi1, lambda1, h1, 200, M_PI / 180 * 0 };
	//X_VKR6 = { phi2, lambda2, h2, 150, M_PI / 180 * 80 };

	// Массив ппм
	KML_Transformer kml_trns;
	
	vector<Lin::Vector> vec_coord;
	vector<Lin::Vector> vec_coord2;
	vector<Lin::Vector> vec_coord3;
	vector<Lin::Vector> vec_coord4;
	vector<Lin::Vector> vec_coord5;
	vector<Lin::Vector> vec_coord6;
	//vec_coord = parser("pyt.kml");	
	// Для первого маршрута
	vec_coord = parser("PYT_VKR6-6.kml");					    // Для второого маршрута
	vec_coord2 = parser("PYT_VKR6-2.kml");
	vec_coord3 = parser("PYT_VKR6-10.kml");					    // Для второого маршрута
	vec_coord4 = parser("PYT_VKR6left.kml");					    // Для второого маршрута
	vec_coord5 = parser("PYT_VKR6right.kml");
	vec_coord6 = parser("PYT_VKR3.kml");

	// Характеристики ОПС
	double gam_max = 45;
	double gam_min = -45;
	double th_max = 0;
	double th_min = -90;
	double rng_max = 20000;

	// Цель ОПС в гео координатах
	Lin::Vector tar;
	Lin::Vector tar2;
	tar = vec_coord[1];
	tar2 = vec_coord2[1];

	

	// Нач вектор состояния АСП
	Lin::Vector X_asp;
    X_asp = { 0, 6000, 0, 300, 0 * 3.14 / 180., 0 };


	LA modelVKR1(X_VKR1, vec_coord, 3000, 1);
	//modelVKR1.N = 1;

	LA modelVKR2(X_VKR2, vec_coord2, 3000, 2);
	LA modelVKR3(X_VKR3, vec_coord3, 3000, 3);
	LA modelVKR4(X_VKR4, vec_coord4, 3000, 4);
	LA modelVKR5(X_VKR5, vec_coord5, 3000, 5);
	//LA modelVKR6(X_VKR2, vec_coord2, 3000, 2);

	LA model(X, X_land, KL, thetaL, 80000, 4200, 2 * GR2RAD, 1);
	std::cout << "model\n";
	
	double m_bomb = 270;
	double theta_t = 23.424;		// Характерестическое время бомбы
	double S_mid = 0.0829;			// Площадб миделя

	//Bomb asp(&model, X_asp, m_bomb, theta_t, S_mid);

	//OPS system(&model, &asp, gam_max, gam_min, th_max, th_min, rng_max);
	std::cout << "ops\n";

	// СНС
	Sat_NS SNS(&model, 10, 10.0, 10.0, 5.0, 55, 3, 35, 4, 13.3, 6.0, 2.0, 1.0, 21, 10, 18);
	In_NS INS(&model, 33, 55, 130, 15.3, 3.5, 6.3245, 400, 200, 6400, 0, 0, 0);

	TEuler integratorLA(0, 1000, 0.1);
	TEuler integratorLA2(0, 1000, 0.1);
	TRunge integratorASP(0, 1000, 0.01);
	

	Timer timer;
	//asp.drop++;
	// 
	// Работа ОПС
	//timer.add(std::chrono::microseconds(50), [&]() {system.get_angles(); });
	LA model2(X, X_land, KL, thetaL, 40000, 2000, -60 * GR2RAD, 2);
	//LA model3(X, X_land, KL, thetaL, 60000, 3100, -0.5 * GR2RAD, 3);
	LA model3(X, X_land, KL, thetaL, 41000, 2100, 0.5 * GR2RAD, 3);
	LA model6(X, X_land, KL, thetaL, 42000, 2200, 0.5 * GR2RAD, 6);

	Lin::Vector X1;
	X1 = { phi0, lambda0, h0, 50, KL + 2 * GR2RAD };

	LA model4(X1, X_land,  KL + M_PI, thetaL, 50000, 1000, (65) * GR2RAD, 4);
	LA model5(X1, X_land, KL + M_PI, thetaL, 18000, 1000, (-0.5)* GR2RAD, 5);
	LA model7(X1, X_land, KL + M_PI, thetaL, 19200, 1100, (0.6)* GR2RAD, 7);
	//LA model7(X1, X_land, KL + M_PI, thetaL, 8100, 500, (-0.5)* GR2RAD, 7);
	LA model8(X1, X_land, KL + M_PI, thetaL, 12000, 630, 0.3 * GR2RAD, 8);
	LA model9(X1, X_land, KL + M_PI, thetaL, 40000, 3000, (-0.8)* GR2RAD, 9);
	LA model10(X1, X_land, KL + M_PI, thetaL, 15000, 800, 1 * GR2RAD, 10);
	LA model11(X1, X_land, KL + M_PI, thetaL, 60000, 3000, (-0.2)* GR2RAD, 11);
	LA model12(X1, X_land, KL + M_PI, thetaL, 54000, 3000, 0.3 * GR2RAD, 12);
	LA model13(X1, X_land, KL + M_PI, thetaL, 50000, 2500, (-0.5)* GR2RAD, 13);
	LA model14(X1, X_land, KL + M_PI, thetaL, 45000, 2500, 0.3 * GR2RAD, 14);
	LA model15(X1, X_land, KL + M_PI, thetaL, 30000, 1500, 0.6 * GR2RAD, 15);
	LA model16(X1, X_land, KL + M_PI, thetaL, 25000, 1500, (-0.1)* GR2RAD, 16);
	LA model17(X1, X_land, KL + M_PI, thetaL, 80000, 4000, (-0.6) * GR2RAD, 17);
	LA model18(X1, X_land, KL + M_PI, thetaL, 25000, 1400, 0.6 * GR2RAD, 18);
	LA model19(X, X_land, KL, thetaL, 54000, 3000, (-0.2)* GR2RAD, 19);
	LA model20(X, X_land, KL, thetaL, 50000, 2500, 0.2 * GR2RAD, 20);
	LA model21(X, X_land, KL, thetaL, 30000, 1500, 0.6 * GR2RAD, 21);
	LA model22(X, X_land, KL, thetaL, 31000, 1600, 0.5 * GR2RAD, 22);
	LA model23(X, X_land, KL, thetaL, 25000, 1500, (-0.5)* GR2RAD, 23);
	LA model24(X, X_land, KL, thetaL, 20000, 1000, 0.2 * GR2RAD, 24);
	LA model25(X, X_land, KL, thetaL, 20000, 1000, (-0.5)* GR2RAD, 25);
	LA model26(X, X_land, KL, thetaL, 30000, 1500, (-0.5)* GR2RAD, 26);
	LA model27(X, X_land, KL, thetaL, 8000, 410, 0.5 * GR2RAD, 27);
	LA model28(X, X_land, KL, thetaL, 10000, 500, (-0.5)* GR2RAD, 28);
	LA model29(X, X_land, KL, thetaL, 46000, 3000, 0.3 * GR2RAD, 29);
	LA model30(X, X_land, KL, thetaL, 78000, 5000, 0.5 * GR2RAD, 30);


	vector<LA*> listLA;
	//listLA.push_back(&model);
	//listLA.push_back(&model2);
	//listLA.push_back(&model3);
	//listLA.push_back(&model4);
	//listLA.push_back(&model5);
	//listLA.push_back(&model6);
	//listLA.push_back(&model7);


	//listLA.push_back(&model8);
	//listLA.push_back(&model9);
	//listLA.push_back(&model10);
	//listLA.push_back(&model11);
	//listLA.push_back(&model12);
	//listLA.push_back(&model13);
	//listLA.push_back(&model14);
	//listLA.push_back(&model15);
	//listLA.push_back(&model16);
	//listLA.push_back(&model17);
	//listLA.push_back(&model18);
	//listLA.push_back(&model19);
	//listLA.push_back(&model20);
	//listLA.push_back(&model21);
	//listLA.push_back(&model22);
	//listLA.push_back(&model23);
	//listLA.push_back(&model24);
	//listLA.push_back(&model25);
	//listLA.push_back(&model26);
	//listLA.push_back(&model27);
	//listLA.push_back(&model28);
	//listLA.push_back(&model29);
	//listLA.push_back(&model30);
	
	// ЛА ДЛЯ ВКР

	listLA.push_back(&modelVKR1);
	listLA.push_back(&modelVKR2);
	listLA.push_back(&modelVKR3);
	listLA.push_back(&modelVKR4);
	listLA.push_back(&modelVKR5);

	vector<TEuler*> listInteg;
	for (int i = 0; i < listLA.size(); ++i)
	{
		//TEuler *X = new TEuler(0, 1000, 0.1);
		listInteg.push_back(new TEuler(0, 1000, 0.05));
	}
	std::cout << "integrators\n";

	std::thread python_plot([&]() {system("python main.py"); });
	
	python_plot.detach();
	
	//timer.add(std::chrono::microseconds(1), [&]() {system("python main.py");});
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	auto tcas = TCAS(listLA);
	int delay_tcas = 11;

	for (int i = 0; i < listInteg.size(); ++i)
	{
		for (int j = 0; j < listInteg.size(); ++j)
		{
			listLA[i]->list_tcas.push_back({ 0,0 });
		}
	}

	timer.add(std::chrono::microseconds(5), [&]() 
		{
			std::string str;
			Plot_Python p1;
			
			if (delay_tcas > 5)
			{
				tcas.run();
				delay_tcas = 0;
			}

			for (int i = 0; i < listLA.size(); ++i)
			{
				if (listLA[i]->stop_integ != 1)
				{
					listInteg[i]->integrate(*listLA[i]);
					fill_struct_py(*listLA[i], p1);
					str = std::to_string(p1.Number) + "|" + std::to_string(p1.phi) + "|" + std::to_string(p1.lbd) +
						"|" + std::to_string(p1.D) + "|" + std::to_string(p1.Az) + "|" + std::to_string(p1.h) 
						+ "|" + std::to_string(p1.delY) + "|" + std::to_string(p1.X) + "|" + std::to_string(p1.Z)
						+ "|" + std::to_string(p1.V) + "|" + std::to_string(p1.PSI) + "|" + std::to_string(p1.TCAS)
						+ "|" + std::to_string(p1.vx) + "|" + std::to_string(p1.vz);
					const char* buff = str.c_str();
					for (int j = 0; j < str.size(); ++j)
					{
						std::cout << buff[j];
					}
					std::cout << " c \n";
					mut.lock();
					sendto(_s, &buff[0], str.size(), 0,
						(sockaddr*)&_destAddr, sizeof(_destAddr));
					mut.unlock();
				}	
				
			}
			delay_tcas++;
			if (std::all_of(std::begin(listLA), std::end(listLA), [](LA* x) {return x->stop_integ == 1;}))
			{
				str = "pause";
				const char* buff = str.c_str();
				mut.lock();
				sendto(_s, &buff[0], str.size(), 0,
					(sockaddr*)&_destAddr, sizeof(_destAddr));
				mut.unlock();
				end.store(true);

			}
		//delete[] buff;
		
		/*if (asp.drop != 0)
			integratorASP.integrate(asp);*/ 
		}
	);


	//timer.add(std::chrono::microseconds(5), [&]() { });

	//timer.add(std::chrono::milliseconds(100), [&]() {SNS.run_sns(); });
	//timer.add(std::chrono::milliseconds(1000), [&]() {SNS.send_pack(); });
	//timer.add(std::chrono::milliseconds(10), [&]() {INS.send_pack(); });
	//timer.add(std::chrono::microseconds(2500), [&]() {INS.run_ins(); });

	std::vector<int> myvec;

	while (1) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		if (end.load()) 
		{
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

	//for (int i = 0; i < system.Point.size(); i++)
	//{
	//	kml_trns.KML_ops_point(i, system.Point[i]);
	//}
	//std::cout << "kml ops writen\n";

	//for (int i = 0; i < system.Point_ASP.size(); i++)
	//{
	//	kml_trns.KML_ops_point(999+i, system.Point_ASP[i]);
	//}
	//std::cout << "kml asp A writen\n";


	//KML_Transformer kml_asp;
	//kml_asp.CreateKML("resultasp");
	//for (int i = 0; i < asp.list_move.size(); i++)
	//{
	//	kml_asp.KMLNewValue(asp.list_move[i]);
	//}
	//std::cout << "kml asp line writen\n";

	// KML glissade
	KML_Transformer kml_gliss;
	kml_gliss.CreateKML("resultgliss");
	for (int i = 0; i < model.Way_glis.size(); i++)
	{
		kml_gliss.KMLNewValue(model.Way_glis[i]);
	}
	std::cout << "kml gliss writen\n";

	return 0;
}