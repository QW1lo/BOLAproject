#include "integrator.h"
#include "model.h"
#include "Lin.h"
#include <mutex>


TIntegrator::TIntegrator(double T0, double Tk, double H)
{
	t0 = T0;
	tk = Tk;
	h = H;
	t = t0;
}

TEuler::TEuler(double T0, double Tk, double H):TIntegrator(T0, Tk, H)
{

}

void TEuler::integrate(TModel& Model)
{	
	mut.lock();
	std::cout << t << "\n";
	Lin::Vector X = Model.getInitialConditions();
	//Model.addResult(X, t);
	//while (t <= tk)
	//{
	X = X + Model.getRight(X,t) * h;
	t += h;
	Model.addResult(X, t);
	//}
	mut.unlock();
	
}

TRunge::TRunge(double T0, double Tk, double H) :TIntegrator(T0, Tk, H)
{

}

void TRunge::integrate(TModel& Model)
{
	mut.lock();
	Lin::Vector X = Model.getInitialConditions();
	Lin::Vector k1, k2, k3, k4;
	
	//Model.addResult(X, t);
	//while ()
	//{	
		
		std::cout << t << "\n";
		Lin::Vector input;
		k1 = Model.getRight(X, t);
		input = X + k1 * h / 2;
		k2 = Model.getRight(input, t + h / 2);
		input = X + k2 * h / 2;
		k3 = Model.getRight(input, t + h / 2);
		input  = X + k3 * h;
		k4 = Model.getRight(input, t + h);
		X = X + (k1 + k2 * 2 + k3 * 2 + k4) * h/6;
		t += h;
		Model.addResult(X, t);
		mut.unlock();
	//}

}