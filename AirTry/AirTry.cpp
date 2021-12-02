#include "model.h"
#include "Lin.h"
#include "integrator.h"
#include <iostream>
#include <vector>


int main()
{
	double t0 = 0;
	double tk = 25;
	double h = 0.1;

	// Начальный вектор состояния
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

	// Точка старта в гео
	double phi0 = 56.419363901986 * M_PI / 180;			// Начальная широта, долгота и
	double lambda0 = 37.94472361872968 * M_PI / 180;		// нулевая высота т.к. стартовая

	LA model(X, Targets, phi0, lambda0);

	TRunge integrator(t0, tk, h);

	integrator.integrate(model);

	std::vector<std::vector<double>> res = model.getResult();

}