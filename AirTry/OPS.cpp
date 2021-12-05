#include <iostream>
#include "Lin.h"
#include "model.h"

#include <string>
#include "kml/dom.h"
#include "kmlTransformer.h"
#include "integrator.h"

// libkml types are in the kmldom namespace
using kmldom::CoordinatesPtr;
using kmldom::KmlPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using std::vector;

class OPS {
private:
	Lin::Vector X_la;
	Lin::Vector X_t;
	
	double Gamma, Theta; 
	double Range;
	double GammaMax, GammaMin, ThetaMax, ThetaMin, MaxRange;

	LA* model;


public:

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
	void print_angles() {
		std::cout << " Gamma=" << Gamma * 180 / M_PI << " Theta =" << Theta * 180 / M_PI << " Range" << Range << "\n";
	}

	void move_LA() {

		for (int i = 0; i < 3; i++) {

			X_la[i] = model->X[i];
		}
	}

	void limit_angles() {

		if (Theta > ThetaMax * M_PI / 180) {
			Theta = 0;
			Gamma = 0;
		}
		if (Theta < ThetaMin * M_PI / 180) {

			Theta = 0;
			Gamma = 0;
		}

		if (Gamma > GammaMax * M_PI / 180){
			Theta = 0;
			Gamma = 0;
		}

		if (Gamma < GammaMin * M_PI / 180) {
			Theta = 0;
			Gamma = 0;
		}
		
		if (Range >= MaxRange) {
			Theta = 0;
			Gamma = 0;
		}

	}

	void get_angles() {

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

		Theta = M_PI/2 - atan2(R[0], R[1]);
		Gamma = atan2(R[2], R[0]);

		limit_angles();
		print_angles();

	}
};

int main()
{	
	// Точка старта в гео
	double phi0 = 61.0 * M_PI / 180;			// Начальная широта, долгота и
	double lambda0 = 62.3231936777456 * M_PI / 180;			// нулевая высота т.к. стартовая
	double h0 = 0;

	// Начальынй ВС
	Lin::Vector X;
	X = { phi0, lambda0, h0, 0.001, 0};

	// Массив ппм
	KML_Transformer kml_trns;
	kml_trns.CreateKML("result");
	vector<Lin::Vector> vec_coord;
	vec_coord = parser("pyt.kml");

	// Характеристики ОПС
	double gam_max = 45;
	double gam_min = -45;
	double th_max = 0;
	double th_min = -90;
	double rng_max = 20000;

	// Цель в гео координатах
	Lin::Vector tar;
	tar = vec_coord[vec_coord.size()-1];

	LA model(X, vec_coord, tar);
	std::cout << "model\n";
	
	OPS system(&model, gam_max, gam_min, th_max, th_min, rng_max);
	std::cout << "ops\n";
	TRunge integrator(0, 1000, 1);
	std::cout << "integrator\n";
	integrator.integrate(model);
	std::cout << "done\n";
}