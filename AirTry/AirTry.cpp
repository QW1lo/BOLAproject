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

	//Lin::Vector e2;
	//e2 = { 0, 1, 0 };
	//Lin::Vector v1;
	//v1 = { 1, 0, 1 };
	//v1 = v1.rotateByRodrig(v1, 1.57079632679, 2);
	//std::cout << v1;
	

	std::vector<double> test1;
	test1 = { 0, 1 };

	Lin::Vector vec;
	//Lin::Matrix M1(2, 2);
	//M1 = { 1, 2,
	//	   3, 4 };

	//Lin::Vector V1(2);
	//V1 = { 3, 3 };
	//std::cout << M1 * V1;

	vec = { 0, 14000, 0, 200, 0};
	//vec.push_back(1);
	std::cout << "as";
	LA model(vec);
	std::cout << "df";
	TRunge integrator(t0, tk, h);
	std::cout << "gh";
	integrator.integrate(model);
	std::cout << "jk";
	std::vector<std::vector<double>> res = model.getResult();
	//for (int i = 0; i < res.size(); ++i)
	//{
	//	for (int j = 0; j < res[1].size(); ++j)
	//		std::cout << res[i][j] << "  ";
	//	std::cout << std::endl;
	//}

	std::cout << "la";
}