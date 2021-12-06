#pragma once
#include <iostream>
#include "Lin.h"
#include "model.h"

#include <string>
#include "kml/dom.h"
#include "kmlTransformer.h"
#include "integrator.h"
#include "Timer.h"
//#include <atomic>
//#include <mutex>
#include "atom.h"


class OPS {
private:
	Lin::Vector X_la;
	Lin::Vector X_t;

	double Gamma, Theta;
	double Range;
	double GammaMax, GammaMin, ThetaMax, ThetaMin, MaxRange;

	LA* model;


public:

	std::vector<Lin::Vector> Point;

	OPS(LA* model0, double GammaMax0, double GammaMin0, double ThetaMax0, double ThetaMin0, double MaxRange0) {

		model = model0;
		for (int i = 0; i < 3; i++) {
			X_la.push_back(model->X[i]);
			X_t.push_back(model->target[i]);

		}


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
	// Нужна для отладки
	void print_kml() {

		if (Gamma != 0.0 && Theta != 0) {
			Lin::Vector d;
			Lin::Vector geo;
			d = VSK_to_svyaz();
			d = model->svyaz_to_norm(d, model->gamma, model->theta, model->X[4]);
			d = d + X_la;
			geo = model->TSK_to_Geo(d, 0) * 180 / M_PI;
			Point.push_back(geo);
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

		Theta = M_PI / 2 - atan2(R[0], R[1]);
		Gamma = atan2(R[2], R[0]);

		limit_angles();
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

};
