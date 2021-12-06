//#include <iostream>
//#include "Lin.h"
//#include "model.h"
//
//#include <string>
//#include "kml/dom.h"
//#include "kmlTransformer.h"
//#include "integrator.h"
////#include "Timer.cpp"
////#include <atomic>
////#include <mutex>
//#include "atom.h"
//#include "OPS.h"

// libkml types are in the kmldom namespace
//using kmldom::CoordinatesPtr;
//using kmldom::KmlPtr;
//using kmldom::KmlFactory;
//using kmldom::PlacemarkPtr;
//using kmldom::PointPtr;
//using std::vector;

//class OPS {
//private:
//	Lin::Vector X_la;
//	Lin::Vector X_t;
//	
//	double Gamma, Theta; 
//	double Range;
//	double GammaMax, GammaMin, ThetaMax, ThetaMin, MaxRange;
//
//	LA* model;
//
//
//public:
//
//	std::vector<Lin::Vector> Point;
//
//	OPS(LA* model0, double GammaMax0, double GammaMin0, double ThetaMax0, double ThetaMin0, double MaxRange0) {
//
//		model = model0;
//		for (int i = 0; i < 3; i++) {
//			X_la.push_back(model->X[i]);
//			X_t.push_back(model->target[i]);
//		
//		}
//
//		
//		Gamma = 0;
//		Theta = 0;
//		Range = 0;
//		GammaMax = GammaMax0;
//		GammaMin = GammaMin0;
//		ThetaMax = ThetaMax0;
//		ThetaMin = ThetaMin0;
//		MaxRange = MaxRange0;
//	}
//	
//	// Нужна для отладки
//	void move_t(Lin::Vector X_t_cur) {
//
//		for (int i = 0; i < 3; i++) {
//
//			X_t[i] = X_t_cur[i];
//		}
//	}
//	// Нужна для отладки
//	void print_kml() {
//		
//		if (Gamma != 0.0 && Theta != 0) {
//			Lin::Vector d;
//			Lin::Vector geo;
//			d = VSK_to_svyaz();
//			mut.lock();
//			d = model->svyaz_to_norm(d, model->gamma, model->theta, model->X[4]);
//			d = d + X_la;	
//			geo = model->TSK_to_Geo(d, 0) * 180 / M_PI;
//			mut.unlock();
//			Point.push_back(geo);
//		}
//
//	}
//
//	void move_LA() {
//		mut.lock();
//		for (int i = 0; i < 3; i++) {
//
//			X_la[i] = model->X[i];
//		}
//		mut.unlock();
//	}
//
//	void limit_angles() {
//
//		if (Theta > ThetaMax * M_PI / 180) {
//			Theta = 0;
//			Gamma = 0;
//			Range = MaxRange;
//		}
//		if (Theta < ThetaMin * M_PI / 180) {
//
//			Theta = 0;
//			Gamma = 0;
//			Range = MaxRange;
//		}
//
//		if (Gamma > GammaMax * M_PI / 180){
//			Theta = 0;
//			Gamma = 0;
//			Range = MaxRange;
//		}
//
//		if (Gamma < GammaMin * M_PI / 180) {
//			Theta = 0;
//			Gamma = 0;
//			Range = MaxRange;
//		}
//		
//		if (Range >= MaxRange) {
//			Theta = 0;
//			Gamma = 0;
//			Range = MaxRange;
//		}
//
//	}
//
//	void get_angles() {
//
//		move_LA();
//
//		Lin::Vector R(3);  // Вектор цели относительно ЛА
//		for (int i = 0; i < 3; i++) {
//
//			R[i] = X_t[i] - X_la[i];
//		}
//
//		//model->X[4] = -M_PI / 4;
//		//model->gamma = -90 * M_PI / 180;
//		//model->theta = 10 * M_PI / 180;
//		mut.lock();
//		R = model->norm2svyaz(R, model->gamma, model->theta, model->X[4]); //Поворот
//		mut.unlock();
//		Range = R.length();
//
//		Theta = M_PI/2 - atan2(R[0], R[1]);
//		Gamma = atan2(R[2], R[0]);
//
//		limit_angles();
//		print_kml();
//
//	}
//
//	Lin::Vector VSK_to_svyaz() {
//
//		Lin::Vector D;
//		D = { Range, 0 , 0 };
//		Lin::Matrix M(3, 3);
//		M = { cos(Gamma) * cos(Theta),	-cos(Gamma) * sin(Theta),	sin(Gamma),
//				     sin(Theta),			cos(Theta),					0,
//				-sin(Gamma) * cos(Theta), sin(Gamma) * sin(Theta),	cos(Gamma)};
//		
//		return M * D;
//	}
//	
//};

//int main()
//{	
//
//	end.store(false);
//
//	// Точка старта в гео
//	double phi0 = 61.0 * M_PI / 180;			// Начальная широта, долгота и
//	double lambda0 = 62.3231936777456 * M_PI / 180;			// нулевая высота т.к. стартовая
//	double h0 = 0;
//
//	// Начальынй ВС
//	Lin::Vector X;
//	X = { phi0, lambda0, h0, 0.001, 0};
//
//	// Массив ппм
//	KML_Transformer kml_trns;
//
//	vector<Lin::Vector> vec_coord;
//	vec_coord = parser("pyt.kml");
//
//	// Характеристики ОПС
//	double gam_max = 45;
//	double gam_min = -45;
//	double th_max = 0;
//	double th_min = -90;
//	double rng_max = 20000;
//
//	// Цель в гео координатах
//	Lin::Vector tar;
//	tar = vec_coord[0];
//
//	LA model(X, vec_coord, tar);
//	std::cout << "model\n";
//
//	
//	OPS system(&model, gam_max, gam_min, th_max, th_min, rng_max);
//	std::cout << "ops\n";
//	TRunge integrator(0, 1000, 1);
//	std::cout << "integrator\n";
//
//	Timer timer;
//	timer.add(std::chrono::microseconds(500), [&]() {system.get_angles(); });
//	timer.add(std::chrono::microseconds(5), [&]() {integrator.integrate(model); });
//	//std::thread thread1([&]() {integrator.integrate(model); });
//	//thread1.join();
//
//	while (1) {
//		if (end.load()) {
//			break;
//		}
//	}
//	
//	std::cout << "done\n";
//	
//	kml_trns.CreateKML("result");
//	for (int i = 0; i < model.Way.size(); i++)
//	{
//		kml_trns.KMLNewValue(model.Way[i]);
//	}
//	std::cout << "kml la writen\n";
//
//
//	for (int i = 0; i < system.Point.size(); i++)
//	{
//		kml_trns.KML_ops_point(i, system.Point[i]);
//	}
//	std::cout << "kml ops writen\n";
//	
//}