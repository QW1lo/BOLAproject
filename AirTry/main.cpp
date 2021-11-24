#include "custom.h"
#include "Lin.h"
#include "integrator.h"
#include <iostream>


int main()
{
	double t0 = 0;
	double tk = 5;
	double h = 0.001;

	Lin::Vector vec({ 0, 0, 0 });
	std::cout << "as";
	Proverka model(vec);
	std::cout << "df";
	TRunge integrator(t0, tk, h);
	std::cout << "gh";
	integrator.integrate( &model );
	std::cout << "jk";
	std::vector<std::vector<double>> res = model.getResult();
	for (int i = 0; i < res.size(); ++i)
	{
		for (int j = 0; j < res[1].size(); ++j)
			std::cout << res[i][j];
		std::cout << std::endl;
	}
	
	std::cout << "la";
}