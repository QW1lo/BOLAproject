#include <iostream>

class OPS {
private:
	double X_la[3];
	double X_t[3];
	double orientation[3]; // Psi Theta Gamma ������������������ ��������� ���� ������ ����
	double Gamma, Theta; // ���� ��
	int GammaMax, GammaMin, ThetaMax, ThetaMin;


public:

	OPS(double X_la0[3], double X_t0[3], double orientation0[3], int GammaMax0, int GammaMin0, int ThetaMax0, int ThetaMin0) {

		for (int i = 0; i < 3; i++) {
			X_la[i] = X_la0[i];
			X_t[i] = X_t0[i];
			orientation[i] = orientation0[i];
		}

		Gamma = 0;
		Theta = 0;
		GammaMax = GammaMax0;
		GammaMin = GammaMin0;
		ThetaMax = ThetaMax0;
		ThetaMin = ThetaMin0;
	}
	// ����� ��� �������
	void move_t(double X_t_cur[3]) {

		for (int i = 0; i < 3; i++) {

			X_t[i] = X_t_cur[i];
		}
	}

	void limit_angles() {

		if (Theta > ThetaMax) {

			Theta = ThetaMax;

		}
		if (Theta < ThetaMin) {

			Theta = ThetaMin;
		}

		/*if (Gamma >= GammaMax) { Gamma = GammaMax; }
		if (Gamma <= -GammaMax) { Gamma = -GammaMax; }*/
	}

	void print_angles() {
		std::cout << " Gamma=" << Gamma << " Theta =" << Theta << "\n";
	}

	void get_angles() {

		double R[3];  // ������ ���� ������������ ��
		for (int i = 0; i < 3; i++) {

			R[i] = X_t[i] - X_la[i];
		}
		R[2] = -R[2];

		Gamma = atan2(R[1], R[0]) * 180 / 3.1415926535;
		Theta = atan2(sqrt(R[0] * R[0] + R[1] * R[1]), R[2]) * 180 / 3.1415926535;

		if (R[0] < 0) {
			Theta = -Theta;
		}

		limit_angles();
		print_angles();

	}
};

//int main()
//{
//	double X_la[3] = { 0, 0, 10 };
//	double X_t[3] = { 0, 0, 0 };
//	double angles[3] = { 0, 0, 0 };
//	OPS system(X_la, X_t, angles, 45, -45, 90, 0);
//	system.get_angles();
//
//	double phi = 0;
//	double a = 10;
//	while (1) {
//		//phi += 0.0002;
//		a -= 0.001;
//		X_t[0] = a * cos(phi);
//		X_t[1] = a * sin(phi);
//		system.move_t(X_t);
//		std::cout << "a=" << a;
//		system.get_angles();
//
//	}
//}