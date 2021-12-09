//#include "Bomb.h"
//
//void Bomb::get_move()
//{
//	for (int i = 0; i < 3; i++) 
//	{
//		X_la[i] = model->X[i];
//	}
//}
//
//void Bomb::get_A_T()
//{
//	double g = 9.81;
//	mut.lock();
//	get_move();
//	
//	double H = model->X[1];
//	double A = model->X[3] * pow(2 * H, 0.5) / g * (1 - exp(-0.000106 * H) * c * H / 6 * (1 + 0.000031 * H) / 5);
//	double T = pow(2 * H, 0.5) / g * (1 - exp(-0.000106 * H) * c * H / 6 * (1 + 0.000063 * H) / 5);
//	
//	print_kml();
//	check(A);
//	
//	mut.unlock();
//}

//void Bomb::print_kml()
//{
//	if model->drop == 1
//}
