#pragma once
#include <iostream>
#include "Lin.h"
#include "model.h"
#include <vector>

class TCAS
{
private:
public:
	std::vector<LA*> listLA;
	Lin::Vector Xi;
	Lin::Vector Xtarget;

	TCAS(std::vector<LA*> listLA0)
	{
		listLA = listLA0;
	};

	void addPPMs(LA* LA, int ind)
	{
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

	}

	void run()
	{
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
				
				Lin::Vector delX = (Xi - Xtarget);
				double D = delX.length();

				double r1 = listLA[i]->X[3] * 60;
				double r2 = listLA[j]->X[3] * 60;

				if (sqrt(delX[0] * delX[0] + delX[2] * delX[2]) < (r1 + r2) && abs(delX[1]) < 500)
				{
					listLA[i]->TCAS = 1;
					listLA[j]->TCAS = 1;
				}


				r1 = listLA[i]->X[3] * 35;
				r2 = listLA[j]->X[3] * 35;

				if (sqrt(delX[0] * delX[0] + delX[2] * delX[2]) < (r1 + r2) && abs(delX[1]) < 200)
				{
					if (listLA[i]->X[0] - listLA[j]->X[0] > 0)
					{
						listLA[i]->TCAS = 3;
						listLA[j]->TCAS = 2;
							if (listLA[i]->X[2] > listLA[j]->X[2])
								addPPMs(listLA[i], 1);
							else
								addPPMs(listLA[i], -1);
					}
					//else
					//	addPPMs(listLA[j], j);
					break;
				}
				

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