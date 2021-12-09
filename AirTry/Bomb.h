#pragma once
#include "Lin.h"
#include "model.h"


class Bomb
{
private:

	LA* model;
	
	Lin::Vector X_t;
	Lin::Vector X_la;
	
public:
	int drop = 0;		// Флаг сброса бомбы
	double m;			// mass
	double c = 8.3e-5;		// Баллистический к-нт АСП

	Bomb() {};

	Bomb(LA* model0, double m0) 
	{
		model = model0;
		m = m0;
		X_t = model->target;
	};
	
	void get_move();
	void get_A_T();
	void check(double A);


	void print_kml();

};