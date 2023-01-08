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
	double mod_tau_min = 20000;
	TCAS(std::vector<LA*> listLA0)
	{
		listLA = listLA0;
	};

	void addPPMs(LA* LA, int ind)
	{
		// ������ ��������
		/*
		// ��� ������ ����� � ��� �������� ��������� ���� �������(������� ��� ������ ����)?
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
		int N = LA_target->N - 1;
		if (LA_self->list_tcas[N][1] == 1)
			return 1;
	
		// ������ � [0] ���� �� ������������: 1 - ����, 0 - ���.
		// � [1] ������ "1" ���� �������  ������������ ����� ��. 0 ��� - ������

		// ������������ ���� ������ ������, ���� ������� �� ���������� ��� ������?
		// ���� �� �������� �� ���������� �� �� � ������� ��� �������� ���� �� ���, � ������� ��� �������� ���� �� ��� ������ ����� ���-��

		// ���-�� ������ ���� ������� �������� �������������� ���������� �������

		// ���� �������� �������� ����� ��� �������� ����������
		// � ����� ������ �������� ���� ������������� �� ���� �� ����� ���


		double sin1 = LA_self->vxyz[0] * delX[2] - delX[0] * LA_self->vxyz[2];
		double cos1 = LA_self->vxyz[0] * delX[0] + LA_self->vxyz[2] * delX[2];
		double angle = -atan2(sin1, cos1) * 180. / M_PI;			// ���� ����� �������� �������� � ���������� �� ������� ��

		double sin_v = LA_self->vxyz[0] * LA_target->vxyz[2] - LA_target->vxyz[0] * LA_self->vxyz[2];
		double cos_v = LA_self->vxyz[0] * LA_target->vxyz[0] + LA_self->vxyz[2] * LA_target->vxyz[2];
		double ang_v = -atan2(sin_v, cos_v) * 180. / M_PI;			// ���� ����� ��������� �������� 

		//  ����� ����
		if (angle < 10 && angle > -10)
		{
			LA_self->list_tcas[N][0] = 0;
			LA_self->list_tcas[N][1] = 1;
			
			if (ang_v < -5 && ang_v > -85)
			{
				LA_self->list_tcas[N][0] = 3; //�����
				LA_self->TCAS = 3;
				LA_self->count_t = 0;
			}
			else
			{
				LA_self->list_tcas[N][0] = 4; //������
				LA_self->TCAS = 3;
				LA_self->count_t = 0;
			}
			return 0;
		}

		// ������ ������� (1��)
		if (angle < -10 && angle > -90)
		{
			LA_self->list_tcas[N][0] = 0;
			LA_self->list_tcas[N][1] = 1;

			LA_self->list_tcas[N][0] = 3; //�����
			LA_self->TCAS = 3;
			LA_self->count_t = 0;

			return 0;
		}

		// ����� ������� (2��)
		if (angle > 10 && angle < 90)
		{
			if (ang_v < -25 && ang_v > -180)
			{
				LA_self->list_tcas[N][0] = 1;
				LA_self->list_tcas[N][1] = 1;
				return 0;
			}
			else
			{
				LA_self->list_tcas[N][0] = 4;
				LA_self->TCAS = 3;
				LA_self->count_t = 0;
				return 0;
			}
			return -1;
		}

		// ����� ����� (3��)
		if (angle > 90 && angle < 180)
		{
			LA_self->list_tcas[N][0] = 1;
			LA_self->list_tcas[N][1] = 1;
			return 0;
		}

		// ������ ����� (4��)
		if (angle < -90 && angle > -180)
		{
			LA_self->list_tcas[N][0] = 1;
			LA_self->list_tcas[N][1] = 1;
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
			listLA[i]->conflict = 0;
			listLA[i]->TCAS = 0;
			if (listLA[i]->stop_integ == 1)
				continue;

			Xi = { listLA[i]->X[0], listLA[i]->X[1], listLA[i]->X[2] };
			for (int j = 0; j < listLA.size(); ++j)
			{
				if (i == j || listLA[j]->stop_integ == 1)
					continue;

				Xtarget = { listLA[j]->X[0], listLA[j]->X[1], listLA[j]->X[2] };

				Lin::Vector delX1 = (Xtarget - Xi);
				if (elipson.collide_la(listLA[i], listLA[j], 60., 1220. * 60. / 35, 1220. * 60. / 35.) && abs(delX1[1]) < 260)
				{
					listLA[i]->TCAS = 1;
				}
			}

			
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
				double mod_tau = abs((DMOD * DMOD - D * D) / (D * V_rad));
				if (mod_tau < mod_tau_min)
					mod_tau_min = mod_tau;
				
				
					
				if (elipson.collide_la(listLA[i], listLA[j], 35., 1220., 1220.) && abs(delX[1]) < 185)
				{
					listLA[i]->conflict++;
					
					if (listLA[i]->TCAS != 3)
						listLA[i]->TCAS = 2;
					
					//������ ����������� � ��������
					solve_benefit(listLA[i], listLA[j], delX);
					
					// ���� ���� ������������ �� ������ � �� ������������ ������ 23 ������
					if (listLA[i]->list_tcas[j][0] == 1 && mod_tau > 23)
						continue;
					if (mod_tau < 23)
					{
						mod_tau += 0.00001;
					}
					double sin1 = listLA[i]->vxyz[0] * delX[2] - delX[0] * listLA[i]->vxyz[2];
					double cos1 = listLA[i]->vxyz[0] * delX[0] + listLA[i]->vxyz[2] * delX[2];
					double angle = -atan2(sin1, cos1) * 180. / M_PI;			// ���� ����� �������� �������� � ���������� �� ������� ��

					if (listLA[i]->list_tcas[j][0] == 3)
					{
						listLA[i]->mode = 10;
						listLA[i]->TCAS = 3;
						listLA[i]->count_t = 0;
					}
					if (listLA[i]->list_tcas[j][0] == 4)
					{
						listLA[i]->mode = 11;
						listLA[i]->TCAS = 3;
						listLA[i]->count_t = 0;
					}

					//����� ����������� ������
				//	if (angle > -10 && angle < 10)
				//	{
				//		double sin_v = listLA[i]->vxyz[0] * listLA[j]->vxyz[2] - listLA[j]->vxyz[0] * listLA[i]->vxyz[2];
				//		double cos_v = listLA[i]->vxyz[0] * listLA[j]->vxyz[0] + listLA[i]->vxyz[2] * listLA[j]->vxyz[2];
				//		double ang_v = -atan2(sin_v, cos_v) * 180. / M_PI;			// ���� ����� ��������� �������� 

				//		if (ang_v < -5 && ang_v > -85)
				//		{
				//			listLA[i]->mode = 10;
				//			listLA[i]->TCAS = 3;
				//			listLA[i]->count_t = 0;
				//		}
				//		else
				//		{
				//			listLA[i]->mode = 11;
				//			listLA[i]->TCAS = 3;
				//			listLA[i]->count_t = 0;
				//		}
				//	}

				//	if (angle > - 95 && angle < -10)
				//	{

				//		listLA[i]->mode = 10;
				//		listLA[i]->TCAS = 3;
				//		listLA[i]->count_t = 0;
				//	}
				//	if (angle < 90 && angle > 10)
				//	{

				//		listLA[i]->mode = 11;
				//		listLA[i]->TCAS = 3;
				//		listLA[i]->count_t = 0;
				//	}
					if (abs(mod_tau) > 60 && (listLA[i]->mode == 10 || listLA[i]->mode == 11))
					{
						listLA[i]->TCAS = 2;
						listLA[i]->count_t = 0;
						listLA[i]->mode = 15;
					}
				}
			}
			if ((listLA[i]->TCAS == 1 || listLA[i]->TCAS == 0) && listLA[i]->mode != 5 &&
				(listLA[i]->TCAS != 21 || listLA[i]->TCAS != 20))
			{
				listLA[i]->mode = 5;
			}

			// if mode = �������
			// ��������� ����������� ���������� ��������� �� ����� ��� ��� TCAS = 0
			// ���� ������������ -> ������������ ��������������
			// ��� ������ ������� ��� ����� ����� ������� ��������� ������

			//if (listLA[i]->conflict > 1 && listLA[i]->count_vert==0)
			//{
			//	// ����� �������� h0 + ������� ����� ��������� h0 ������� ����� � h ������� � �� � h0
			//	// � ��������� ��� ������� � ����� �������� ���� i ����� �������
			//	listLA[i]->mode = 20;
			//	//listLA[i]->h0 = listLA[i]->h0 + 350;
			//	listLA[i]->count_vert++;
			//}

			
		}
		for (int i = 0; i < listLA.size(); ++i)
		{
			if (listLA[i]->mode == 10 || listLA[i]->mode == 11)
			{
				for (int j = 0; j < listLA.size(); ++j)
				{
					// ������� ������ ��� tCAS != 0 
					if (i == j || listLA[j]->stop_integ == 1 || listLA[j]->TCAS > 1)
						continue;
					if (elipson.collide_maneuver(listLA[i], listLA[j], 35., 1220., 1220., listLA[i]->mode) && 
						abs(listLA[i]->X[1] - listLA[j]->X[1]) < 180)
					{
						int one = 1;
						listLA[i]->mode = 20;
						if (abs(listLA[i]->h0 - listLA[j]->h0) < 200)
						{
							if (listLA[i]->N == 1)
								one = -1;
							listLA[i]->h0 = listLA[i]->h0 + 350 * one;
						}
					}


				}
			}
		}
	};
};