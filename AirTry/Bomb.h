#pragma once
#include "Lin.h"
#include "model.h"
#include "atom.h"

class Bomb : public TModel
{
private:
	FILE* output;
	std::vector<Lin::Vector> list_move;
	
	LA* model;
	double S_mid;				// S midel

	// Таблицы для рассчета Cx(M)
	int table_size = 12;
	double M_table [12] = { 0, 0.4, 0.55, 0.65, 0.75, 0.8, 0.9, 1, 1.05, 1.1, 1.15, 1.25 };
	double Cx_table[12] = { 0.580, 0.580, 0.593, 0.648, 0.752, 0.831, 1.004, 1.178, 1.262, 1.325, 1.367, 1.403 };

public:
	
	int drop = 0;			// Флаг сброса бомбы
	double m;				// mass
	double c = 8.3e-5;		// Баллистический к-нт АСП

	Bomb(LA* model0, Lin::Vector X0, double m0, double c0, double S_mid0) : TModel(X0)
	{
		model = model0;
		output = fopen("outASP.txt", "w");
		m = m0;
		c = c0;
		S_mid = S_mid0;
	};
	
	Lin::Vector getRight(const Lin::Vector& v, double t);
	
	double ro(double H)
	{
		return (1.225 * pow((1 - 2.256 * pow(10, -5) * H), 4.256));
	}
	
	double T(double H) 
	{
		return (288.15 - 6.5 * pow(10, -3) * H);
	}
	
	double accel(double H) 
	{
		return 20.05 * pow(T(H), 0.5);
	}

	void get_move(Lin::Vector X_la, double theta);
	void addResult(const Lin::Vector& v, double t);

};