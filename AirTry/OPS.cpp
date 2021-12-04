#include <iostream>
#include "Lin.h"
#include "model.h"

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
		R = model->norm2svyaz(R, -model->gamma, model->theta, model->X[4]); //Поворот

		Range = R.length();

		Theta = M_PI/2 - atan2(R[0], R[1]);
		Gamma = atan2(R[2], R[0]);

		limit_angles();
		print_angles();

	}
};

int main()
{
	// Начальый весктор состояния
	Lin::Vector X;
	X = { 0, 0, 0, 0.001, 0};
	
	// Массив целей в гео координатах
	std::vector<Lin::Vector> Targets;
	Lin::Vector target(3);
	target = { 57.419363901986 * M_PI / 180, 37.94472361872968 * M_PI / 180, 12000 };
	Targets.push_back(target);
	target = { 57.419363901986 * M_PI / 180, 38.94472361872968 * M_PI / 180, 12000 };
	Targets.push_back(target);
	target = { 57.419363901986 * M_PI / 180, 36.94472361872968 * M_PI / 180, 12000 };		//
	Targets.push_back(target);

	// Цель в гео координатах 
	Lin::Vector tar;
	tar = {0, 0, 0};

	// Точка старта в гео
	double phi0 = 56.419363901986 * M_PI / 180;			// Начальная широта, долгота и
	double lambda0 = 37.94472361872968 * M_PI / 180;		// нулевая высота т.к. стартовая
	
	LA model(X, Targets, tar,  phi0, lambda0);

	OPS system(&model, 45, -45, 0, -90, 20000);
	
	system.get_angles();

/*double phi = 0;
double a = 0;
while (1) {
	phi += 0.00032;
	a += 1;
	X_t[0] = a * cos(phi);
	X_t[2] = a * sin(phi);
	system.move_t(X_t);

	system.get_angles();
}*/
}