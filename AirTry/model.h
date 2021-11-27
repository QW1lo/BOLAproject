//---------------------------------------------------------------------------
#include <math.h>
#include <array>
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
private:
    std::array<Lin::Vector, 3> list_targets;
    std::array<int, 3> list_rotation;
    int count_targ = 0;
    double gamma = 0;
    double theta = 0;
    int count = 1;

public:
    FILE* output;
    LA(Lin::Vector& X0) :TModel(X0) 
    { 
        output = fopen("LAoutput.txt", "w"); 
        Lin::Vector target1(3);
        target1 = { -2000, 14000, -5000};
        Lin::Vector target2(3);
        target2 = { 700, 14000, 1000};
        Lin::Vector target3(3);
        target3 = { 2000, 14000, -300};
        list_targets[0] = target1;
        list_targets[1] = target2;
        list_targets[2] = target3;
        std::cout << list_rotation[0];
    };

    Lin::Vector getRight(const Lin::Vector& v, double t)
    {
        // todo: костыли с каунтами - персмотреть алгоритм определения поворотов
        Lin::Vector target(3);
        target = list_targets[count_targ];

        if (abs(v[0] - target[0]) < 10 && abs(v[2] - target[2]) < 30)
        {
            count_targ++;
            if (count_targ == list_targets.size())
            {
                count_targ = 0;
                for (int i = 0; i < list_rotation.size(); ++i)
                    list_rotation[i] = 0;

            }
            target = list_targets[count_targ];
            count = 1;
                
        }

        double nxa = 0;
        double g = 9.81;

        gamma = 0;
        //double psi = 15 * GR2RAD;
        theta = 0 * GR2RAD;
        Lin::Vector tmp(v.size());
        Lin::Vector v_sv;
        Lin::Vector target_sv;


        v_sv = { v[0], v[1], v[2] };
        
        target_sv = norm2svyaz(target, gamma, theta, v[4]);
        v_sv = norm2svyaz(v_sv, gamma, theta, v[4]);
        gamma = 0;

        centerrad(v, target, -g / v[3] * tan(15 * GR2RAD), &list_rotation[count_targ], (v_sv[2] - target_sv[2]));

        // Логика совершения маневров todo
        if (list_rotation[count_targ] == 1)
        {
            if (abs(v_sv[2] - target_sv[2]) > 10)
            {
                if ((v_sv[2] - target_sv[2]) < 0)
                {
                    //centerrad(v, target, -g / v[3] * tan(15 * GR2RAD), &list_rotation[count_targ]);
                    gamma = 15 * GR2RAD;

                }
                else
                {
                    //centerrad(v, target, -g / v[3] * tan(-15 * GR2RAD), &list_rotation[count_targ]);
                    gamma = -15 * GR2RAD;
                }

            }
            else 
            {
                gamma = 0;

            }
        }

        tmp[0] = v[3] * cos(theta) * cos(v[4]);          // xg'
        tmp[1] = v[3] * sin(theta);                      // yg'
        tmp[2] = -v[3] * cos(theta) * sin(v[4]);         // zg'
        tmp[3] = g * nxa;                                // V' в траекторной СК
        tmp[4] = -g / v[3] * tan(gamma);                 // PSI' = wy

        return tmp;
    }

    Lin::Vector norm2svyaz(Lin::Vector& v, double gamma, double theta, double psi)
    {
        Lin::Matrix M(3, 3);
        
        M = { cos(psi) * cos(theta),                                sin(theta),             -sin(psi) * cos(theta),
        sin(psi) * sin(gamma) - cos(psi) * sin(theta) * cos(gamma), cos(theta) * cos(gamma), cos(psi) * sin(gamma) + sin(psi) * sin(theta) * cos(gamma),
        sin(psi) * cos(gamma) + cos(psi) * sin(theta) * sin(gamma), -cos(theta) * sin(gamma), cos(psi) * cos(gamma) - sin(psi) * sin(theta) * sin(gamma) };
        
        return M * v;
    }

    int centerrad(Lin::Vector v, Lin::Vector& targ, double dPSI, int* rotation, double error)
    {
        int sign = 1;
        if (error > 0)
            sign *= -1;

        if (*rotation == 1)
            return 0;
        *rotation = 1;
        double Ra = 200 * 200 / (9.81 * tan(15 * GR2RAD));
        double dt = 0.1;
        dPSI *= sign;
        double d = dPSI * dt;
        Lin::Vector res(3);
        Lin::Vector tc(3);
        res[0] = v[3] * cos(theta) * cos(v[4]);
        res[2] = -v[3] * cos(theta) * sin(v[4]);

        double len = res.length();

        if (dPSI < 0)
            res = res.rotateByRodrig(res, -90 * GR2RAD, 2);
        else
            res = res.rotateByRodrig(res, 90 * GR2RAD, 2);
        res = res * Ra;


        tc[0] = v[0] - res[0];
        tc[2] = v[2] - res[2];
        

        if (pow((targ[0] - tc[0]), 2) + pow((targ[2] - tc[2]), 2) < Ra * Ra)
            *rotation = -1;
        if (*rotation == 1)
            std::cout << "";
        return 1;
    }

    void addResult(const Lin::Vector& v, double t)
    {
        X = v;
        std::vector<double> tmp;

        tmp.push_back(t);
        for (int i = 0; i < v.size(); ++i)
            tmp.push_back(v[i]);

        fprintf(output, "%lf  %lf  %lf  %lf\n", tmp[0], tmp[1], tmp[2], tmp[3]);
        
        //tmp.clear();

    }

};

#endif
 