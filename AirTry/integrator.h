//---------------------------------------------------------------------------

#ifndef integratorH
#define integratorH

// Подключение заголовочного файла библиотеки векторных и матричных операций
#include "Lin.h"
class TModel;

class TIntegrator
{
	public:
		double t0;
		double tk;
		double h;
		double t;
		TIntegrator(double T0, double Tk, double H);
		virtual void integrate(TModel& Model) = 0;		
};


class TEuler : public TIntegrator
{
	public:
		TEuler(double T0, double Tk, double H);
		void integrate(TModel& Model);
};

class TRunge : public TIntegrator
{
public:
	TRunge(double T0, double Tk, double H);
	void integrate(TModel& Model);
};

#endif