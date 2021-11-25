#include <iostream>
#include <array>
#include <vector>
#include <math.h>
#include "Lin.h"


using namespace Lin;

double GR2RAD = M_PI / 180.;
double RAD2GR = 180. / M_PI;

//////////////////			 class: Vector				//////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//Vector::Vector() {}
//
//Vector::Vector(const int size_vec)
//{
//	this->data[]
//	v = std::vector<double>(size_v);
//	size_v = size_vec;
//	for (int i = 0; i < size_v; ++i)
//	{
//		v[i] = 0;
//	}
//}

Vector::Vector(const std::vector<double>& values)
{
	this->clear();
	for (int i = 0; i < values.size(); ++i)
		this->push_back(values[i]);
}

/*Vector::Vector(double val, ...)
{
	double* p = &val;
	while (*p)
	{
		size_v++;
		p++;
	}
	p = &val;
	v = new double[size_v];
	for (int i = 0; i < size_v; ++i)
	{
		v[i] = *p;
		p++;
	}
}*/

//double Vector::operator[] (const int index1) const
//{
//	return v[index1];
//}
//
//double Vector::operator() (const int index) const
//{
//	return v[index];
//}


Vector& Vector::operator= (const std::initializer_list<double> values)
{
	this->clear();
	this->resize(values.size());
	int count = 0;
	for (auto& element : values)
	{
		(*this)[count] = element;
		++count;
	}
	return *this;
#ifdef _DEBUG

#endif
}

Vector& Vector::operator=(const Vector& values)
{
	this->clear();
	for (int i = 0; i < values.size(); ++i)
		this->push_back(values[i]);
	return *this;
}

Vector Vector::operator+ (const Vector& vector) const
{
	if (this->size() == vector.size())
	{
		std::vector<double> tmp;
		for (int i = 0; i < vector.size(); ++i)
		{
			tmp.push_back((*this)[i] + vector[i]);
		}
			
		return Vector(tmp);
	}
#ifdef _DEBUG
	else
		throw " size sum Vector";
#endif
}

Vector Vector::operator- (const Vector& vector) const
{
	if (this->size() == vector.size())
	{
		std::vector<double> tmp;
		for (int i = 0; i < vector.size(); ++i)
		{
			tmp.push_back((*this)[i] - vector[i]);
		}

		return Vector(tmp);
	}
#ifdef _DEBUG
	else
	throw "size sub Vector";
#endif
}

Vector Vector::operator-() const
{
	// TODO:
	std::vector<double> tmp;
	for (int i = 0; i < this->size(); ++i)
	{
		tmp.push_back(-(*this)[i]);
	}
	return Vector(tmp);
}


// Скалярное произведение векторов
// Scalar multiplication vectors
// todo проверка размерности
double Vector::operator* (const Vector& vector) const
{
	if (this->size() == vector.size())
	{
		double sum = 0;
		for (int i = 0; i < this->size(); ++i)
		{
			sum += (*this)[i] * vector[i];
		}
		return sum;
	}
#ifdef _DEBUG
	else
	throw "size mul Vector";
#endif
}

// Умножение вектора на число
// Mult vector by a number
Vector Vector::operator* (const double& value) const
{
	std::vector<double> tmp;
	for (int i = 0; i < this->size(); ++i)
	{
		tmp.push_back((*this)[i] * value);
	}
	return Vector(tmp);
}

// Умножение на матрицу
// Todo
Vector Vector::operator* (const Matrix& values) const
{
	if (this->size() == values.row())
	{
		double sum = 0;
		std::vector<double> tmp;
		for (int i = 0; i < this->size(); ++i)
		{
			sum = 0;
			for (int j = 0; j < values.row(); ++j)
			{
				sum += (*this)[j] * values(j, i);
			}
			tmp.push_back(sum);
		}
		return Vector(tmp);
	}
	else
		throw "size mul Vector by Matrix";
}

Vector Vector::operator/ (const double& value) const
{
	std::vector<double> tmp;
	for (int i = 0; i < this->size(); ++i)
	{
		tmp.push_back((*this)[i] / value);
	}
	return Vector(tmp);
}

// Векторное произведение векторов
Vector Vector::cross(const Vector& values) const
{
	if (this->size() == values.size() && this->size() == 3)
	{
		std::vector<double> tmp = { 1, 1, 1 };
		for (int i = 0; i < this->size(); ++i)
		{
			tmp.push_back((*this)[i]);
		}
		for (int i = 0; i < this->size(); ++i)
		{
			tmp.push_back(values[i]);
		}
		Matrix M(this->size(), this->size());
		M = tmp;
		tmp.clear();
		for (int i = 0; i < this->size(); ++i)
		{
			tmp.push_back(pow(-1, i) * M.minor(0,i).det());
		}
		return Vector(tmp);
	}
#ifdef _DEBUG
	else
	{
		throw " size cross Vector";
	}
#endif

	
}

// Длина вектора
double Vector::length() const
{
	double len = 0;
	for (int i = 0; i < this->size(); ++i)
	{
		len += (*this)[i] * (*this)[i];
	}
	return pow(len, 1. / 2);
}

// Нормирование вектора
Vector Vector::norm() const
{
	std::vector<double> tmp(3);
	double len = this->length();
	for (int i = 0; i < this->size(); ++i)
	{
		tmp[i] = ((*this)[i] / len);
	}
	Vector ret_V(tmp);
	//ret_V.v = tmp;
	return ret_V;
}

// TODO проверка vec и себя на размер
Vector Vector::rotateByRodrig(const Vector vec, const double phi)
{
	Vector tmp(vec.size());
	tmp = vec;
	tmp = tmp.norm();
	return tmp * (tmp * *this) * (1 - cos(phi)) + tmp.cross(*this) * sin(phi) + *this * cos(phi);
}

// todo проверка себя на размер
//Vector Vector::rotateByQuaternion(Quaternion Q)
//{
//	Quaternion tmp = Q * *this * Q.conj();
//	return Vector({ tmp(1), tmp(2), tmp(3) });
//}

//int Vector::size() const
//{
//	return size_v;
//};


//////////////////			 class: Matrix				//////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Matrix::Matrix(const int row, const int col)
{

	if (row >= 0 && col >= 0)
	{
		m_row = row;
		m_col = col;
		std::vector<double> tmp;
		for (int i = 0; i < row; ++i)
		{
			v.push_back(tmp);
			for (int j = 0; j < col; ++j)
			{
				v[i].push_back(0.);
			}
			tmp.clear();
		}
	}
#ifdef _DEBUG
	else
	{
	throw "Create Matrix: col or row =< 0";
	}
#endif
}
	
	
Matrix& Matrix::operator= (std::vector<double> values)
{
	if (values.size() == m_row * m_col)
	{
		for (int i = 0; i < m_row; ++i)
		{

			for (int j = 0; j < m_col; ++j)
			{
				v[i][j] = values[i * m_col + j];
			}
		}
		return *this;
	}
#ifdef _DEBUG
	else
	{
		throw "size initializer_list";
	}
#endif
	
}

// Публичный метод обращения к элементам массива через ()
double Matrix::operator() (const int index1, const int index2) const
{
	return v[index1][index2];
}

// Сумма матриц
Matrix Matrix::operator+ (const Matrix& M) const
{
	if (m_row == M.m_row && m_col == M.m_col)
	{
		std::vector<double> tmp;
		for (int i = 0; i < m_row; ++i)
		{
			for (int j = 0; j < m_col; ++j)
			{
				tmp.push_back(this->v[i][j] + M.v[i][j]);
			}
		}
		Matrix M_return(m_row, m_col);
		M_return = tmp;
		return M_return;
	}
#ifdef _DEBUG
	else
	{
		throw "size sum Matrix" ;
	};
#endif
}

// Разность матриц
Matrix Matrix::operator- (const Matrix & M) const
{
	if (m_row == M.m_row && m_col == M.m_col)
	{
		std::vector<double> tmp;
		for (int i = 0; i < m_row; ++i)
		{
			for (int j = 0; j < m_col; ++j)
			{
				tmp.push_back(this->v[i][j] - M.v[i][j]);
			}
		}
		Matrix M_return(m_row, m_col);
		M_return = tmp;
		return M_return;
	}
#ifdef _DEBUG
	else
	{
		throw "size sub Matrix" ;
	};
#endif
}

// Деление на число
Matrix Matrix::operator/ (double value) const
{
	std::vector<double> tmp;
	for (int i = 0; i < m_row; ++i)
	{
		for (int j = 0; j < m_col; ++j)
		{
			tmp.push_back(this->v[i][j] / value);
		}
	}
	Matrix M_return(m_row, m_col);
	M_return = tmp;
	return M_return;
}

// Умножение на число
Matrix Matrix::operator* (double value) const
{
	std::vector<double> tmp;
	for (int i = 0; i < m_row; ++i)
	{
		for (int j = 0; j < m_col; ++j)
		{
			tmp.push_back(this->v[i][j] * value);
		}
	}
	Matrix M_return(m_row, m_col);
	M_return = tmp;
	return M_return;
}

// Умножение на матрицу
Matrix Matrix::operator* (const Matrix & M) const
{
	if (m_col == M.m_row)
	{
		double sum = 0;
		std::vector<double> tmp;
		//std::vector<double> endtmp;
		for (int i = 0; i < m_row; ++i)
		{
			for (int j = 0; j < M.m_col; ++j)
			{
				for (int k = 0; k < m_col; ++k)
				{
					sum += v[i][k] * M.v[k][j];
						
				}
				tmp.push_back(sum);
				sum = 0;
			}
		}
		Matrix M_return(m_row, M.m_col);
		M_return = tmp;
		return M_return;
	}
#ifdef _DEBUG
	else
	{
		throw "size mul Matrix by Matrix";

	};
#endif
}

// Умножение на вектор
Vector Matrix::operator* (const Vector& V) const
{
	if (m_col == V.size())
	{
		std::vector<double> tmp;
		double sum = 0;
		for (int i = 0; i < m_row; ++i)
		{
			sum = 0;
			for (int j = 0; j < m_col; ++j)
			{
				sum += v[i][j] * V[j];
			}
			tmp.push_back(sum);
		}

		return Vector(tmp);
	}
#ifdef _DEBUG
	else
	{
		throw "Error size sub Matrix by Vector";
	};
#endif
}

// Определитель матрицы
double Matrix::det() const
{
#ifdef _DEBUG
	if (m_row != m_col)
	{
		throw "Det from not square matrix";
	}
#endif
	if (m_col == 2)
		return v[0][0] * v[1][1] - v[0][1] * v[1][0];
	else
	{
		double sum = 0;
		for (int j = 0; j < m_col; ++j)
		{
			sum += pow(-1, j) * v[0][j] * this->minor(0, j).det();
		}
		return sum;
	}
}

// Минор матрицы
Matrix Matrix::minor(int i, int j) const
{
	std::vector<double> tmp;
	for (int k = 0; k < m_row; ++k)
	{
		if (k != i)
		{
			for (int l = 0; l < m_row; ++l)
			{
				if (l != j)
				{
					tmp.push_back(v[k][l]);
				}
			}
		}
	}
	Matrix M_return(m_row - 1, m_col - 1);
	M_return = tmp;
	return M_return;
}

// Транспонирование матрицы
Matrix Matrix::T() const
{
	std::vector<double> tmp;
	for (int i = 0; i < m_col; ++i)
	{
		for (int j = 0; j < m_row; ++j)
			tmp.push_back(v[j][i]);
	}
	Matrix M_return(m_col, m_row);
	M_return = tmp;
	return M_return;
}


// Вычисление обратной матрицы
Matrix Matrix::inv() const
{
	// source: https://www.programmersforum.ru/showthread.php?t=51386
	
	int i, k, j;
	Matrix obr(m_row, m_col);
	Matrix M(m_row, m_col);
	M.v = this->v;

	for (k = 0; k < m_row; ++k)
	{
		for (i = 0; i < m_row; ++i)
		{
			for (j = 0; j < m_row; ++j)
			{
				if (i == k && j == k)
					obr.v[i][j] = 1 / M.v[i][j];
				if (i == k && j != k)
					obr.v[i][j] = -M.v[i][j] / M.v[k][k];
				if (i != k && j == k)
					obr.v[i][j] = M.v[i][k] / M.v[k][k];
				if (i != k && j != k)
					obr.v[i][j] = M.v[i][j] - M.v[k][j] * M.v[i][k] / M.v[k][k];
			}
		}
		for (i = 0; i < m_row; i++)
			for (j = 0; j < m_row; j++) M.v[i][j] = obr.v[i][j];
	}
	return M;
}


int Matrix::row() const
{
	return m_row;
}

int Matrix::col() const
{
	return m_col;
}


///////////////			   class: Quaternion               ///////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//Quaternion::Quaternion(const std::vector<double>& values)
//{
//
//	if (values.size() == 4)
//	{
//		q0 = values[0];
//		q = { values[1], values[2], values[3] };
//		
//	}
//#ifdef _DEBUG
//	else
//		throw "Quaternion size != 4 \n";
//#endif
//}

//Quaternion::Quaternion()
//{
//	q0 = 0;
//	q = { 0, 0, 0 };
//}
//
//Quaternion::Quaternion(const double phi, const std::vector<double>& values)
//{
//	if (values.size() == 3)
//	{
//		q0 = cos(phi / 2.);
//		q = { values[0], values[1], values[2] };
//		q = q.norm();
//		q = q * sin(phi / 2.);
//	}
//#ifdef _DEBUG
//	else
//		throw "Quaternion size != 4 \n";
//#endif
//}
//
//Quaternion::Quaternion(const double phi, const Vector& values)
//{
//	if (values.size() == 3)
//	{
//		q0 = cos(phi / 2.);
//		q = values;
//		q = q.norm();
//		q = q * sin(phi / 2.);
//	}
//#ifdef _DEBUG
//	else
//		throw "Quaternion size != 4 \n";
//#endif
//}
//
//Quaternion::Quaternion(const Quaternion& rvalue) { *this = rvalue; }
//
//Quaternion::Quaternion(const double q0, const double q1, const double q2, const double q3)
//{
//	this->q0 = q0;
//	q(0) = q1;
//	q(1) = q2;
//	q(2) = q3;
//}
//
//Quaternion& Quaternion::operator=(std::vector<double>& values)
//{
//	
//	if(values.size() == 4)
//	{
//		q0 = values[0];
//		for (int i = 1; i < 4; ++i)
//		{
//			q(i) = values[i];
//		}
//		return *this;
//	}
//#ifdef _DEBUG
//	else
//		throw "operator= Quaternion size != 4 \n";
//#endif
//
//}
//
//double Quaternion::operator[](const int index) const
//{
//	if (index == 0)
//	{
//		double r_q[1] = { q0 };
//		return r_q[0];
//	}
//		
//	else
//		return q(index - 1);
//}
//
//double Quaternion::operator()(const int index) const
//{
//	if (index == 0)
//	{
//		double r_q[1] = { q0 };
//		return r_q[0];
//	}
//
//	else
//		return q(index - 1);
//}
//
//Quaternion Quaternion::operator+(const Quaternion& other) const
//{
//	return Quaternion({q0 + other.q0, q(0) + other.q(0), q(1) + other.q(1) , q(2) + other.q(2) });
//}
//
//Quaternion Quaternion::operator-(const Quaternion& other) const
//{
//	return Quaternion({ q0 - other.q0, q(0) - other.q(0), q(1) - other.q(1) , q(2) - other.q(2) });
//}
//
//Quaternion Quaternion::operator*(const Quaternion& other) const
//{
//	double a = q0 * other.q0 - q * other.q;
//	Vector vec(3);
//	vec = other.q * q0 + q * other.q0 + q.cross(other.q);
//	return Quaternion({ a, vec(0), vec(1), vec(2) });
//}
//
//Quaternion Quaternion::operator*(const Vector& values) const
//{
//	Quaternion other = Quaternion({ 0, values(0), values(1), values(2) });
//	return *this * other;
//}
//
//Quaternion Quaternion::operator*(const double value) const
//{
//	return Quaternion({q0 * value, q(0) * value, q(1) * value, q(2) * value });
//}
//
//Quaternion Quaternion::operator/(const double value) const
//{
//	return Quaternion({ q0 / value, q(0) / value, q(1) / value, q(2) / value });
//}
//
//double Quaternion::length() const
//{
//	double len = q0*q0;
//	for (int i = 0; i < 3; ++i)
//	{
//		len += q(i) * q(i);
//	}
//	return pow(len, 1. / 2);
//}
//
//// Вычисление сопряженного кватарниона
//Quaternion Quaternion::conj() const
//{
//	return Quaternion(q0, -q(0), -q(1), -q(2));
//}
//
//// Нормирование кватарниона
//Quaternion Quaternion::norm() const
//{
//	double n = this->length();
//	return Quaternion(q0 / n, q(0) / n, q(1) / n, q(2) / n);
//}
//
//// Обратный кватернион
//Quaternion Quaternion::inv() const
//{
//	Quaternion Q_conj = this->conj();
//	double n = this->length();
//	return Q_conj / n;
//}
