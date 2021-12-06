#pragma once
#include <cmath> 

inline int* massbin(double max_value, int max_digit, int digit, double value) {
	// 20 - число значащих разрядов в ARINC_BNR
	int* arr = new int[max_digit]();
	int last_i = 0;
	float sum = 0;
	for (int i = 0; i <= digit - 1; i++)
	{
		sum = sum + (max_value / pow(2, i));
		if (sum <= value) {
			arr[i] = 1;
		}
		else {
			arr[i] = 0;
			sum = sum - (max_value / pow(2, i));
		}
		last_i = i;
	}
	if (last_i < max_digit - 1)
		for (int i = last_i; i < max_digit; ++i)
			arr[i] = 0;
	return arr;
}

// Перевод десятичного числа в двоичный float на выводе int для удобной передачи
inline int bindec(double max_value, int max_digit, int digit, double value) {
	int* arr = massbin(max_value, max_digit, digit, value);
	int sum = 0;
	for (int i = 0; i < max_digit; ++i) {
		sum = sum + (arr[i] * pow(2, max_digit - (1 + i)));
	}
	delete[] arr;
	return sum;
}

inline double* decbin(int dec, int max_digit) {
	double* bin = new double[max_digit]();
	for (int i = 0; dec > 0; i++)
	{
		bin[max_digit - (i + 1)] = (dec % 2);
		dec /= 2;
	}
	return bin;
}

inline int massdec(double max_value, int max_digit, int digit, int dec) {
	// 20 - число значащих разрядов в ARINC_BNR
	double* arr = decbin(dec, max_digit);
	int sum = 0;
	for (int i = 0; i <= digit - 1; i++) {
		sum = sum + arr[i] * (max_value / pow(2, i));
	}
	delete[] arr;
	return sum;



}
