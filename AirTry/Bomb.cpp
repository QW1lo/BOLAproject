#include "Bomb.h"

Lin::Vector Bomb::getRight(const Lin::Vector& v, double t)
{
	Lin::Vector tmp(v.size());
	if (drop != 0)
	{
		double theta = v[5];
		/*if (theta < -M_PI_2)
			theta = -M_PI - v[5];*/
		
		double g = 9.81;
		
		
		double M = v[3] / accel(v[1]);					// Мах
	 	double q = ro(v[1]) * pow(v[3], 2) / 2.;		// Скоростной напор
		
		double Cx = Lin::linear_search(M, table_size, M_table, Cx_table);  // линейная интерполяция по таблицам для нахлождения CX
		double X_f = Cx * q * S_mid;						// X -  сила лоб сопротивления

		tmp[0] = v[3] * cos(theta) * cos(v[4]);          // xg'
		tmp[1] = v[3] * sin(theta);                      // yg'
		tmp[2] = -v[3] * cos(theta) * sin(v[4]);         // zg'
		tmp[3] = -(X_f + m*g * sin(theta)) / m;            // V' в траекторной СК
		tmp[4] = 0;										// PSI' = wy
		tmp[5] = -g / v[3] * cos(theta);				    // theta'
	}
	//std::cout << tmp;
	return tmp;
}

void Bomb::get_move(Lin::Vector X_la, double theta)
{
	X = { X_la[0], X_la[1], X_la[2], X_la[3], X_la[4], theta};
}

void Bomb::addResult(const Lin::Vector& v, double t)
{
	if (count_move % 10 == 0)
	{
		Lin::Vector geo;
		Lin::Vector tsk;
		tsk = { v[0],  0 , v[2] };
		geo = model->TSK_to_Geo(tsk, 0) * 180 / M_PI;
		list_move.push_back(geo);
	}
	count_move++;
	// todo запись в кмл
	X = v;
	if (drop != 0)
		fprintf(output, "%lf  %lf  %lf  %lf  %lf  %lf  %lf\n", t, X[0], X[1], X[2], X[3], X[4], X[5]);
	
	if (X[1] < 0)
		end.store(true);
}

