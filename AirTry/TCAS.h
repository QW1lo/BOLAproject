#pragma once
#include <iostream>
#include "Lin.h"
#include "model.h"
#include "Ellipse.h"
#include <vector>

class TCAS
{
private:
public:
	std::vector<LA*> listLA;
	Lin::Vector Xi;
	Lin::Vector Xtarget;
	EllipseCollisionTest elipson{ 10 };
	double D_min = 20000;
	TCAS(std::vector<LA*> listLA0)
	{
		listLA = listLA0;
	};

	void addPPMs(LA* LA, int ind)
	{
		// Старые повороты
		/*
		// Кто наверх летит а кто остается выполнять свой маршрут(глисада или второй круг)?
		LA->mode = 2;
		LA->list_ppm.clear();
		Lin::Vector ppm(3);
		Lin::Vector ppm1(3);
		ppm = { LA->X[0] + 44000 , LA->X[1] + 200 * ind, LA->X[2] + 30000 * ind };
		
		LA->list_ppm.push_back(ppm);
		
		ppm1 = { 9000.  , 1500, 38000. * ind  };
		ppm = ppm + ppm1;
		LA->list_ppm.push_back(ppm);

		ppm1 = { -9000. , 1000, 18000. * ind };
		ppm = ppm + ppm1;
		LA->list_ppm.push_back(ppm);

		ppm1 = { 10000. , 0, -38000. * ind };
		ppm = ppm + ppm1;
		LA->list_ppm.push_back(ppm);
		
		LA->list_rotation.clear();
		LA->list_rotation.resize(4);
		LA->TCAS = 3;
		*/
		if (LA->N == 1)
		{
			LA->mode = 10;
			LA->TCAS = 3;
		}
		
	}

	int solve_benefit(LA* LA_self, LA* LA_target, Lin::Vector delX)
	{
		if (LA_self->list_tcas[LA_target->N][1] == 1)
			return 1;
	
		// Ставим в [0] есть ли преимущество: 1 - есть, 0 - нет.
		// В [1] ставим "1" если считали  преимущество между ЛА. 0 там - дефолт

		// Преимущество если помеха справа, если впереди то уклоняются оба вправо?
		// Если мы обгоняем то уклоняемся мы не в сторону его движения если на нас, в сторону его движения если он под прямым углом где-то

		// Где-то должна быть функция проверки рациональности совершения маневра

		// Сюда закинуть проверку углов для подсчета привелегий
		// А снизу просто выбирать куда маневрировать по сути НЕ МНОГО ЭТО


		double sin1 = LA_self->vxyz[0] * delX[2] - delX[0] * LA_self->vxyz[2];
		double cos1 = LA_self->vxyz[0] * delX[0] + LA_self->vxyz[2] * delX[2];
		double angle = -atan2(sin1, cos1) * 180. / M_PI;			// угол между вектором скорости и дельностью до другого ла

		double sin_v = LA_self->vxyz[0] * LA_target->vxyz[2] - LA_target->vxyz[0] * LA_self->vxyz[2];
		double cos_v = LA_self->vxyz[0] * LA_target->vxyz[0] + LA_self->vxyz[2] * LA_target->vxyz[2];
		double ang_v = -atan2(sin_v, cos_v) * 180. / M_PI;			// угол между векторами скорости 

		//  перед нами
		if (angle < 10 && angle > -10)
		{
			LA_self->list_tcas[LA_target->N][0] = 0;
			LA_self->list_tcas[LA_target->N][1] = 1;
			return 0;
		}

		// Справа впереди (1пп)
		if (angle < -10 && angle > -90)
		{
			LA_self->list_tcas[LA_target->N][0] = 0;
			LA_self->list_tcas[LA_target->N][1] = 1;
			return 0;
		}

		// Слева впереди (2пп)
		if (angle > 10 && angle < 90)
		{
			if (ang_v < -25 && ang_v > -180)
			{
				LA_self->list_tcas[LA_target->N][0] = 1;
				LA_self->list_tcas[LA_target->N][1] = 1;
				return 0;
			}
			else
			{
				LA_self->list_tcas[LA_target->N][0] = 0;
				LA_self->list_tcas[LA_target->N][1] = 1;
				return 0;
			}
			return -1;
		}

		// Слева сзади (3пп)
		if (angle > 90 && angle < 180)
		{
			LA_self->list_tcas[LA_target->N][0] = 1;
			LA_self->list_tcas[LA_target->N][1] = 1;
			return 0;
		}

		// справа сзади (4пп)
		if (angle < -90 && angle > -180)
		{
			LA_self->list_tcas[LA_target->N][0] = 1;
			LA_self->list_tcas[LA_target->N][1] = 1;
			return 0;
		}

		return -1;
	}

	void run()
	{
		double DMOD = 1220;
		double trash = 0;
		for (int i = 0; i < listLA.size(); ++i)
		{
			listLA[i]->TCAS = 0;
			if (listLA[i]->stop_integ == 1)
				continue;
			

			
			Xi = { listLA[i]->X[0], listLA[i]->X[1], listLA[i]->X[2] };
			//if (Xi.length() < 3000)
			//	continue;
			
			for (int j = 0; j < listLA.size(); ++j)
			{
				if (i == j || listLA[j]->stop_integ == 1)
					continue;

				Xtarget = { listLA[j]->X[0], listLA[j]->X[1], listLA[j]->X[2] };
				
				Lin::Vector delX = (Xtarget - Xi);
				Lin::Vector eD = delX.norm();
				Lin::Vector Vdot = listLA[j]->vxyz - listLA[i]->vxyz;
				
				double D = delX.length();
				if (D < D_min)
					D_min = D;
				double V_rad;
				V_rad = (listLA[j]->vxyz - listLA[i]->vxyz) * eD;
				double t_cpa = (delX[0] * Vdot[0] + delX[2] * Vdot[2]) / (Vdot[0] * Vdot[0] + Vdot[2] * Vdot[2]);
				double mod_tau = (DMOD * DMOD - D * D) / (D * V_rad);
				
				if (elipson.collide_la(listLA[i], listLA[j], 60., 1220. * 60./35, 1220. * 60./35.) && abs(delX[1]) < 500)
				{
					listLA[i]->TCAS = 1;
				}
					
				if (elipson.collide_la(listLA[i], listLA[j], 35., 1220., 1220.) && abs(delX[1]) < 300)
				{
					solve_benefit(listLA[i], listLA[j], delX);
					double sin1 = listLA[i]->vxyz[0] * delX[2] - delX[0] * listLA[i]->vxyz[2];
					double cos1 = listLA[i]->vxyz[0] * delX[0] + listLA[i]->vxyz[2] * delX[2];
					double angle = -atan2(sin1, cos1) * 180. / M_PI;			// угол между вектором скорости и дельностью до другого ла

					listLA[i]->TCAS = 2;
					
					auto check = [listLA = this->listLA, i, j]()
					{
						for (int k = 0; k < listLA[j]->list_tcas.size(); k++)
						{
							if (listLA[j]->list_tcas[k][0] == listLA[i]->N)
								return 1;
						}
						return 0;
					};
					if (check() == 1)
						break;
					//Старое
					//if (listLA[i]->X[0] - listLA[j]->X[0] < 0)
					//{
					//	//listLA[i]->TCAS = 3;
					//	if (listLA[i]->X[2] > listLA[j]->X[2])
					//		addPPMs(listLA[i], 1);
					//	else
					//		addPPMs(listLA[i], -1);
					//}
					//break;

					if (angle > -10 && angle < 10)
					{
						double sin_v = listLA[i]->vxyz[0] * listLA[j]->vxyz[2] - listLA[j]->vxyz[0] * listLA[i]->vxyz[2];
						double cos_v = listLA[i]->vxyz[0] * listLA[j]->vxyz[0] + listLA[i]->vxyz[2] * listLA[j]->vxyz[2];
						double ang_v = -atan2(sin_v, cos_v) * 180. / M_PI;			// угол между векторами скорости 

						if (ang_v < -5 && ang_v > -85)
						{
							listLA[i]->mode = 10;
							listLA[i]->TCAS = 3;
							listLA[i]->count_t = 0;
							listLA[i]->list_tcas[0][0] = listLA[j]->N;
						}
						else
						{
							listLA[i]->mode = 11;
							listLA[i]->TCAS = 3;
							listLA[i]->count_t = 0;
							listLA[i]->list_tcas[0][0] = listLA[j]->N;
						}
					}

					if (angle > - 90 && angle < -10)
					{
						listLA[i]->list_tcas.push_back({ listLA[j]->N, 0 });
						//listLA[i]->TCAS = 3;
						if (listLA[i]->X[2] > listLA[j]->X[2])
							addPPMs(listLA[i], 1);
						else
							addPPMs(listLA[i], -1);
					}
					if (abs(mod_tau) > 40)
					{
						listLA[i]->TCAS = 2;
						listLA[i]->count_t = 0;
						listLA[i]->mode = 15;
					}
				}
				
				if (listLA[i]->TCAS == 1 && listLA[i]->mode != 5)
				{
					listLA[i]->mode = 5;
					//listLA[i]->list_tcas[0] = 0;
				}
					
				
				/*
				double r1 = listLA[i]->X[3] * 60;
				double r2 = listLA[j]->X[3] * 60;

				if (sqrt(delX[0] * delX[0] + delX[2] * delX[2]) < (r1 + r2) && abs(delX[1]) < 500)
				{
					listLA[i]->TCAS = 1;
				}


				r1 = listLA[i]->X[3] * 35;
				r2 = listLA[j]->X[3] * 35;

				if (sqrt(delX[0] * delX[0] + delX[2] * delX[2]) < (r1 + r2) && abs(delX[1]) < 200)
				{
					
					listLA[i]->TCAS = 2;
					if (listLA[i]->X[0] - listLA[j]->X[0] > 0)
					{
						listLA[i]->TCAS = 3;
							if (listLA[i]->X[2] > listLA[j]->X[2])
								addPPMs(listLA[i], 1);
							else
								addPPMs(listLA[i], -1);
					}
					//else
					//	addPPMs(listLA[j], j);
					break;
				}
				*/

				// TODO Если курс встречный набор высоты и отворот, курс можно посчитать по разности векторов высчитав угол как арктангенс
				//if (D < 2200 && abs(delX[1]) < 200)
				//{
				//	if (listLA[i]->X[0] - listLA[j]->X[0] > 0)
				//	{
				//		listLA[i]->TCAS = 2;
				//		if (listLA[i]->X[2] > listLA[j]->X[2])
				//			addPPMs(listLA[i], 1);
				//		else
				//			addPPMs(listLA[i], -1);
				//	}
				//	//else
				//	//	addPPMs(listLA[j], j);
				//	break;
				//}
				//listLA[i]->TCAS = 0;
			}
		}
	};
};