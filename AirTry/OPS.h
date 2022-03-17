#pragma once
#include <iostream>
#include "Lin.h"
#include "model.h"
#include "Bomb.h"

#include <string>
#include "kml/dom.h"
#include "kmlTransformer.h"
#include "integrator.h"
#include "Timer.h"
//#include <atomic>
//#include <mutex>
#include "atom.h"



class OPS 
{
private:
	Bomb *asp;
	Lin::Vector X_la;	// в норм ск
	Lin::Vector X_t;	

	int count_print = 0;
	double Gamma, Theta;
	double Range;
	double GammaMax, GammaMin, ThetaMax, ThetaMin, MaxRange;
	double A = 0, T = 0;		// Относ и время падения АБ
	
	LA* model;
	

public:

	std::vector<Lin::Vector> Point;
	std::vector<Lin::Vector> Point_ASP;

	OPS(LA* model0, Bomb* asp0, double GammaMax0, double GammaMin0, double ThetaMax0, double ThetaMin0, double MaxRange0) {

		model = model0;
		for (int i = 0; i < 3; i++) {
			X_la.push_back(model->X[i]);
			X_t.push_back(model->target[i]);
		}
		asp = asp0;

		Gamma = 0;
		Theta = 0;
		Range = 0;
		GammaMax = GammaMax0;
		GammaMin = GammaMin0;
		ThetaMax = ThetaMax0;
		ThetaMin = ThetaMin0;
		MaxRange = MaxRange0;
	}

	// Нужна для отладки
	void move_t(Lin::Vector X_t_cur) {

		for (int i = 0; i < 3; i++) {

			X_t[i] = X_t_cur[i];
		}
	}
	
	void print_kml() {

		if (Gamma != 0.0 && Theta != 0 ) 
		{
			if (count_print % 10 == 0) 
			{
				Lin::Vector d;
				Lin::Vector geo;
				d = VSK_to_svyaz();
				d = model->svyaz_to_norm(d, model->gamma, model->theta, model->X[4]);
				d = d + X_la;
				geo = model->TSK_to_Geo(d, 0) * 180 / M_PI;
				Point.push_back(geo);
			}
			count_print++;
		}
		if (asp->drop == 1)
		{

			Lin::Vector d1;
			d1 = { X_la[0], 0, X_la[2] };
			Lin::Vector geo_drop_point;
			Lin::Vector geo_land_point;
			
			geo_drop_point = model->TSK_to_Geo(d1, 0) * 180 / M_PI;
			Point_ASP.push_back(geo_drop_point);

			d1 = model->norm2svyaz(d1, 0, 0, model->X[4]);
			d1[0] += A;
			d1 = model->svyaz_to_norm(d1, 0, 0, model->X[4]);
			geo_land_point = model->TSK_to_Geo(d1, 0) * 180 / M_PI;
			Point_ASP.push_back(geo_land_point);

			asp->drop++;
		}

	}

	void move_LA() 
	{
		for (int i = 0; i < 3; i++) {

			X_la[i] = model->X[i];
		}
	}

	void limit_angles() {

		if (Theta > ThetaMax * M_PI / 180) {
			Theta = 0;
			Gamma = 0;
			Range = MaxRange;
		}
		if (Theta < ThetaMin * M_PI / 180) {

			Theta = 0;
			Gamma = 0;
			Range = MaxRange;
		}

		if (Gamma > GammaMax * M_PI / 180) {
			Theta = 0;
			Gamma = 0;
			Range = MaxRange;
		}

		if (Gamma < GammaMin * M_PI / 180) {
			Theta = 0;
			Gamma = 0;
			Range = MaxRange;
		}

		if (Range >= MaxRange) {
			Theta = 0;
			Gamma = 0;
			Range = MaxRange;
		}

	}

	void get_angles() {
		mut.lock();
		move_LA();

		Lin::Vector R(3);  // Вектор цели относительно ЛА
		
		for (int i = 0; i < 3; i++) {

			R[i] = X_t[i] - X_la[i];
		}

		//model->X[4] = -M_PI / 4;
		//model->gamma = -90 * M_PI / 180;
		//model->theta = 10 * M_PI / 180;
		
		R = model->norm2svyaz(R, model->gamma, model->theta, model->X[4]); //Поворот

		Range = R.length();

		Theta = M_PI / 2 - atan2(R[0], R[1]);   // Elevation
		Gamma = atan2(R[2], R[0]);			    // Az
		
		limit_angles();

		A_T_ASP();
		if (Range < 19000)
			check(R[0]);
		
		print_kml();
		mut.unlock();

	}

	Lin::Vector VSK_to_svyaz() {

		Lin::Vector D;
		D = { Range, 0 , 0 };
		Lin::Matrix M(3, 3);
		M = { cos(Gamma) * cos(Theta),	-cos(Gamma) * sin(Theta),	sin(Gamma),
					 sin(Theta),			cos(Theta),					0,
				-sin(Gamma) * cos(Theta), sin(Gamma) * sin(Theta),	cos(Gamma) };

		return M * D;
	}

	void A_T_ASP()
	{
		double g = 9.81;

		double H = model->X[1];
		A = model->X[3] * pow(2 * H / g, 0.5)  * (1 - exp(-0.000106 * H) * asp->c * H / 6. * (1 + 0.000031 * H / 5.));
		//std::cout << A << "\n";
		T = pow(2 * H / g, 0.5)  * (1 + exp(-0.000106 * H) * asp->c * H / 6. * (1 + 0.000063 * H) / 5.);

	}

	void check(double x_range)
	{
		if (abs(x_range - A) < 50 && asp->drop == 0)
		{
			//std::cout << "START ASP!";
			asp->get_move(model->X, model->theta);
			asp->drop = 1;
		}
	}


};
