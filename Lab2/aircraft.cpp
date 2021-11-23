#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

class Aircraft {
private:

	float m;	// Масса самолета, кг
	float S;
	float V;	// Горизонтальная скорость, м/с
	float Vy;	// Вертикальная скорость, м/с
	float H; ;	// Высота Ла, м
	float T;	// Температура за бортом, К
	float alpha;// Угол атаки, рад
	float gamma;// Угол крена. рад
	int E;	// Режим работы двигателя. Взлет - 1, номинал - 2, 0.9 номинала - 3, ... , 6

	float H_ro = 11600;		// Константа высоты для плотности
	float H_p = 11000;		// 
	float Tmca = 217;		// Константа температуры
public:

	Aircraft(float m0, float S0, float V0, float Vy0, float H0,
		float T0, float alpha0, float gamma0, int E0) {

		m = m0;
		S = S0;
		V = V0;
		Vy = Vy0;
		H = H0;
		T = T0;
		alpha = alpha0;
		gamma = gamma0;
		E = E0;
	}

	float Cy() {

		float result = 0.0;
		float a_deg = 180 / M_PI * alpha;

		if (a_deg >= 2.7 && a_deg < 10.0)
			result = 0.121 * (a_deg - 2.7);

		else if (a_deg >= 10.0 && a_deg < 18.0)
			result = 1.0 - 0.0075 * (-14.0 + a_deg) * (-14.0 + a_deg);

		return result;
	}

	float Cx() {

		float result = 0.0;
		float a_deg = 180 / M_PI * alpha;

		if (a_deg >= 2.7 && a_deg < 7.5)
			result = 0.025 + 0.00085 * (a_deg - 2.7) * (a_deg - 2.7);

		if (a_deg >= 7.5 && a_deg < 16.0)
			result = -0.003 + 0.0018 * (a_deg - 2.7) * (a_deg - 2.7);

		if (a_deg >= 16.0)
			result = 0.014 + 0.000128 * (a_deg - 2.7) * (a_deg - 2.7) * (a_deg - 2.7);

		return result;
	}

	float ro() {

		float deltaH = H - H_ro;
		float deltaT = T - Tmca;

		return 0.34 * (1 - 0.11 * deltaH / 1000) * (1.0 - deltaT / Tmca);
	}

	float P() {
		float deltaH = H - H_p;
		float deltaT = T - Tmca;

		deltaH = deltaH / 1000;

		float result = 0.0;
		// Взлетный
		if (E == 11)
			result = (96 - 15.0 * deltaH) * (1.0 - deltaT / Tmca);
		// Номинальный
		if (E == 2)
			result = (87 - 11.5 * deltaH) * (1.0 - deltaT / Tmca);
		// 0.9 номинала
		if (E == 3)
			result = (83 - 10.2 * deltaH) * (1.0 - deltaT / Tmca);
		// 0.8 номинала
		if (E == 4)
			result = (76 - 8.0 * deltaH) * (1.0 - deltaT / Tmca);
		// 0.7 номинала
		if (E == 5)
			result = (69 - 7.5 * deltaH) * (1.0 - deltaT / Tmca);
		//0.6 номинала
		if (E == 6)
			result = (59 - 4.6 * deltaH) * (1.0 - deltaT / Tmca);

		result = result * 1000;		// Перевод из кН в Н
		return result;
	}

	float Fy() {
		float result = 0.0;

		result = Cy() * S * ro() * V * V / 2;

		result = result * cos(gamma);

		return result;

	}

	float Fx() {
		float result = 0.0;

		result = Cx() * S * ro() * V * V / 2;

		return result;
	}

};

//int main() {
//	Aircraft TU_154(98000, 201, 500, 0, 11300, 217, 4 * M_PI / 180, 0.0, 3);
//	float Y = TU_154.Fy();
//	float G = 98000 * 9.81;
//	float X = TU_154.Fx();
//	float P = TU_154.P();
//
//}