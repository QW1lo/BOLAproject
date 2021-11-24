#pragma once
#include <iostream>
#include <vector>

typedef std::vector<double> BaseVector;
typedef std::vector<std::vector<double>> BaseMatrix;

namespace Lin {
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

		Vector rotateByRodrig(Vector vec, const double phi);
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