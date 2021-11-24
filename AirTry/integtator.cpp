#include "integrator.h"
#include "model.h"
#include "Lin.h"

TIntegrator::TIntegrator(double T0, double Tk, double H)
{
	t0 = T0;
	tk = Tk;
	h = H;
}

TEuler::TEuler(double T0, double Tk, double H):TIntegrator(T0, Tk, H)
{

}

void TEuler::integrate(TModel& Model)
{	
	Lin::Vector X = Model.getInitialConditions();
	double t = t0;

	while (t <= tk)
	{
		X = X + Model.getRight(X,t) * h;
		Model.addResult(X, t);
		t += h;
	}
	
}

TRunge::TRunge(double T0, double Tk, double H) :TIntegrator(T0, Tk, H)
{

}

void TRunge::integrate(TModel& Model)
{
	Lin::Vector X = Model.getInitialConditions();
	Lin::Vector k1, k2, k3, k4;
	double t = t0;

	while (1)
	{
		Lin::Vector input;
		k1 = Model.getRight(X, t);
		input = X + k1 * h / 2;
		k2 = Model.getRight(input, t);
		input = X + k2 * h / 2;
		k3 = Model.getRight(input, t);
		input  = X + k3 * h;
		k4 = Model.getRight(input, t);
		X = X + (k1 + k2 * 2 + k3 * 2 + k4) * h/6;
		Model.addResult(X, t);
		t += h;
	}

}