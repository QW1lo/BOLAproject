//---------------------------------------------------------------------------
#include <math.h>
#pragma warning(disable : 4996)
#ifndef modelH
#define modelH
#include "Lin.h"
//---------------------------------------------------------------------------
extern double GR2RAD;
extern double RAD2GR;

//---------------------------------------------------------------------------
class TModel
{
public:
    // Текущий вектор состояния
    Lin::Vector X;

    // Хранилище результатов
    std::vector<std::vector<double>> Result;


    // Общий конструктор моделей - инициализация параметров по умолчанию
    TModel(Lin::Vector& X0) { X = X0; }
    // Абстрактная перегружаемая функция правых частей ДУ (X - вектор состояния, t - независимый аргумент)
    virtual Lin::Vector getRight(const Lin::Vector& v, double t) = 0;
    // Получение начальных условий
    inline Lin::Vector getInitialConditions() const { return X; }
    // Порядок системы - по размерности вектора начальных условий
    inline int getOrder() const { return X.size(); }

    // Получение матрицы результатов
    inline std::vector<std::vector<double>> getResult() const { return Result; }

    // Запись результатов (в этом методе в наследнике допустимо организовать запись в файл 
    // вместо или вместе с наполнением матрицы результатов)

    virtual void addResult(const Lin::Vector& v, double t)
    {
        X = v;
        std::vector<double> tmp;

        tmp.push_back(t);
        for (int i = 0; i < v.size(); ++i)
            tmp.push_back(v[i]);

        Result.push_back(tmp);
        
        for (int i = 0; i < tmp.size(); ++i)
            std::cout << tmp[i] << " ";
        std::cout << std::endl;
        tmp.clear();
    }

};

class Proverka : public TModel
{

public:
    Proverka(Lin::Vector& X0) :TModel(X0) {};
    Lin::Vector getRight(const Lin::Vector& v, double t)
    {
        Lin::Vector tmp(v.size());
        tmp = v;

        tmp[0] = 2 * t;
        tmp[1] = 2 * t;
        tmp[2] = 2 * t;
        return tmp;
    }
};


class LA : public TModel
{
public:
    FILE* output;
    LA(Lin::Vector& X0) :TModel(X0) { output = fopen("LAoutput.txt", "w"); };

    Lin::Vector getRight(const Lin::Vector& v, double t)
    {
        //double V = 180;
        double nxa = 0;
        double g = 9.81;
        double gamma = 10 * RAD2GR;
        double psi = 15 * RAD2GR;
        double theta = 0 * RAD2GR;
        Lin::Vector tmp(v.size());

        //tmp[0] = V * cos(theta) * cos(psi);
        //tmp[1] = V * sin(theta);
        //tmp[2] = -V * sin(theta) * sin(psi);
        tmp[0] = v[3] * cos(theta) * cos(v[4]);          // xg'
        tmp[1] = v[3] * sin(theta);                      // yg'
        tmp[2] = -v[3] * cos(theta) * sin(v[4]);         // zg'
        tmp[3] = g * nxa;                             // V'
        tmp[4] = -g / v[3] * tan(gamma);                 // PSI' = wy


        return tmp;
    }

    void addResult(const Lin::Vector& v, double t)
    {
        X = v;
        std::vector<double> tmp;

        tmp.push_back(t);
        for (int i = 0; i < v.size(); ++i)
            tmp.push_back(v[i]);

        //Result.push_back(tmp);

        //for (int i = 0; i < tmp.size(); ++i)
        //    std::cout << tmp[i] << " ";
        //std::cout << std::endl;

        fprintf(output, "%lf  %lf  %lf  %lf\n", tmp[0], tmp[1], tmp[2], tmp[3]);
        
        //tmp.clear();

    }

};

#endif
 