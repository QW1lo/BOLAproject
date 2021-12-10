#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <math.h>


typedef std::vector<double> BaseVector;
typedef std::vector<std::vector<double>> BaseMatrix;

namespace Lin {
	extern double GR2RAD;
	extern double RAD2GR;

	inline double lin_interpol(double x0, double y0, double x1, double y1, double x)
	{
		return (((x - x0) * (y0 - y1) / (x0 - x1) + y0));
	}

	inline double linear_search(double x, int n, double x_arr[], double y_arr[])
	{
		// Используется бинарный поиск по массиву векторов, находится номера векторов, между которыми "лежит" 
		// необходимый нам вектор на заданный момент времени. 
		size_t i_left = 0, i_right = n - 1;
		size_t mid;

		while (((i_right - i_left) != 1))
		{
			mid = (i_left + i_right) / 2.0;
			if (x_arr[mid] <= x)
			{
				i_left = mid;
			}
			if (x_arr[mid] > x)
			{
				i_right = mid;
			}
		}

		return lin_interpol(x_arr[i_left], y_arr[i_left], x_arr[i_right], y_arr[i_right], x);
	}

	inline double lagrange(double x, int n, double x_arr[], double y_arr[])
	{
		//Пусть точки отсортированы по возрастанию координаты x

		if (n > 5)
			return (x <= x_arr[n / 2]) ? lagrange(x, (n + 1) / 2, x_arr, y_arr) :
			lagrange(x, (n + 1) / 2, &x_arr[n / 2], &y_arr[n / 2]);

		double sum = 0;
		for (int i = 0; i < n; ++i) {

			double l = 1;
			for (int j = 0; j < n; ++j)
				if (j != i)
					l *= (x - x_arr[j]) / (x_arr[i] - x_arr[j]);
			sum += y_arr[i] * l;
		}

		return sum;
	}

	class Matrix;
	class Quaternion;

	class Vector: public BaseVector
	{
	protected:
		//std::vector<double> v;
		//int size_v;

	public:
		//TODO базовый конструктор, v - vector, Vector::vector;
		Vector() : BaseVector() {};
		Vector(const int size_vec) : BaseVector(size_vec) {};
		Vector(const std::vector<double>& values);

		//Vector(double val, ...);
		//double operator[] (const int index1) const;
		//double operator() (const int index) const;

		friend std::ostream& operator<< (std::ostream& os, const Vector& vector)
		{
			os << "(";
			for (int i = 0; i < vector.size() - 1; ++i)
			{
				os << vector[i] << ", ";

			}
			os << vector[vector.size() - 1] << ")" << std::endl;
			return os;
		}
		Vector& operator= (const std::initializer_list<double> values);
		Vector& operator= (const Vector& values);
		Vector operator+ (const Vector& vector) const;
		Vector operator- (const Vector& vector) const;
		Vector operator- () const;

		double operator* (const Vector& vector) const;
		Vector operator* (const double& value) const;
		Vector operator* (const Matrix& values) const;
		Vector operator/ (const double& value) const;
		Vector cross(const Vector& values) const;

		double length() const;
		Vector norm() const;

		Vector rotateByRodrig(const Vector& vec, const double phi, int O = 1);
		Vector rotateByQuaternion(Quaternion Q);

		//int size() const;
	};


	class Matrix
	{
	protected:
		std::vector<std::vector<double>> v;
		int m_row = NULL;
		int m_col = NULL;

	public:
		Matrix(const int row, const int col);
		Matrix& operator= (std::vector<double> values);

		friend std::ostream& operator<< (std::ostream& os, const Matrix& M)
		{
			for (int i = 0; i < M.m_row; ++i)
			{
				os << "(";
				for (int j = 0; j < M.m_col - 1; ++j)
				{
					os << M.v[i][j] << ", ";

				}
				os << M.v[i][M.m_col - 1] << ")" << std::endl;
			}
			return os;
		};

		double operator() (const int index1, const int index2) const;
		Matrix operator+ (const Matrix& M) const;
		Matrix operator- (const Matrix& M) const;
		Matrix operator/ (double value) const;

		Matrix operator* (double value) const;
		Matrix operator* (const Matrix& M) const;
		Vector operator* (const Vector& V) const;

		double det() const;
		Matrix minor(int i, int j) const;
		//int nonzero_row(int k);
		Matrix T() const;
		Matrix inv() const;

		int row() const;
		int col() const;
	};


	class Quaternion
	{
	protected:
		double q0;
		Vector q{ {0,0,0} };

	public:
		Quaternion(const std::vector<double>& values);
		Quaternion();
		Quaternion(const double phi, const std::vector<double>& values);
		Quaternion(const double phi, const Vector& values);
		Quaternion(const Quaternion& rvalue);
		Quaternion(const double q0, const double q1, const double q2, const double q3 );

		Quaternion& operator= (std::vector<double>& values);
		double operator[] (const int index) const;
		double operator() (const int index) const;

		// TODO
		//friend std::ostream& operator<< (std::ostream& os, const Quaternion& Q)
		//{
		//	os << Q.q0 << " + ";
		//	os << Q.q(0) << "i + ";
		//	os << Q.q(1) << "j + ";
		//	os << Q.q(2) << "k\n";
		//	return os;
		//}


		Quaternion operator+ (const Quaternion& other) const;
		Quaternion operator-(const Quaternion& other) const;
		Quaternion operator* (const Quaternion& other) const;
		Quaternion operator* (const Vector& values) const;
		Quaternion operator* (const double value) const;

		Quaternion operator/ (const double value) const;

		double length() const;
		Quaternion conj() const;
		Quaternion norm() const;
		Quaternion inv() const;


	};
}