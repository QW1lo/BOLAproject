//---------------------------------------------------------------------------
#pragma once
#include <math.h>
#include <array>
#pragma warning(disable : 4996)
#ifndef modelH
#define modelH
#include "Lin.h"
#include "atom.h"

//---------------------------------------------------------------------------
extern double GR2RAD;
extern double RAD2GR;

//---------------------------------------------------------------------------
class TModel
{
public:
    int start = 0;
    // Текущий вектор состояния
    Lin::Vector X;
    // Хранилище результатов
    std::vector<std::vector<double>> Result;


    // Общий конструктор моделей - инициализация параметров по умолчанию
    TModel(Lin::Vector& X0) { X = X0; }
    // Абстрактная перегружаемая функция правых частей ДУ (X - вектор состояния, t - независимый аргумент)
    virtual Lin::Vector getRight(const Lin::Vector& v, double t) = 0;
    // Получение текущего вектора состояния
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
    std::vector<Lin::Vector> list_ppm;
    double last_psi = 0;
    std::vector<int> list_rotation;
    int count_targ = 0;
    double m = 80000;   // масса ВС
    
    int count = 1;      

    int R = 6371000;										// Радиус земли, м
    double omega = 0.00007292115;							// Угловая скорость Земли, рад

    double phi0 = 0;			                            // Начальная широта, долгота и
    double lambda0 = 0;		                                // нулевая высота т.к. стартовая
    double h0 = 0;											// точка на поверхности Земли

public:
    FILE* output;

    std::vector<Lin::Vector> Way;                           // Для КМЛ путь в ГСК
    

    Lin::Vector target;                                     // Вектор В ТСК цели

    Lin::Vector ppm;                                        // Вектор в ТСК текущего ппм

    double gamma = 0;
    double theta = 0;

    LA(Lin::Vector& X0, std::vector<Lin::Vector>& Init_ppms, Lin::Vector target0) :TModel(X0)
    {
        output = fopen("LAoutput.txt", "w");

        phi0 = X[0];
        lambda0 = X[1];
        target = Geo_TSK(target0 * M_PI / 180, 0);
        target[1] = 0;
        //target[2] = 1000;

        for (int i = 0; i < 3; i++) {
            X[i] = 0;
        }

        for (int i = 0; i < Init_ppms.size(); i++) {
            Lin::Vector tmp;
            tmp = Geo_TSK(Init_ppms[i] * M_PI / 180, 0);
            list_ppm.push_back(tmp);
            list_rotation.push_back(0);
        }
    };

    Lin::Vector Rotate(char axis, Lin::Vector vec, double angle) {

        Lin::Matrix R(3, 3);	// Матрица поворота
        if (axis == 'x') {
            R = { 1     ,     0     ,     0     ,
                      0     , cos(angle),-sin(angle),
                      0     , sin(angle), cos(angle) };
        }
        if (axis == 'y') {
            R = { cos(angle),     0     , sin(angle),
                      0     ,     1     ,     0     ,
                 -sin(angle),     0     , cos(angle) };
        }
        if (axis == 'z') {
            R = { cos(angle),-sin(angle),     0     ,
                  sin(angle), cos(angle),     0     ,
                      0     ,     0     ,     1 };
        }
        return vec * R;
    }

    Lin::Vector Shift(char axis, Lin::Vector vec, double val) {

        if (axis == 'x') {
            vec[0] = vec[0] - val;
        }
        if (axis == 'y') {
            vec[1] = vec[1] - val;
        }
        if (axis == 'z') {
            vec[2] = vec[2] - val;
        }
        return vec;
    }

    Lin::Vector TSK_to_ISK(Lin::Vector vec, float time) {

        Lin::Vector tmp;  // Промежуточный результат
        tmp = { 0.0, 0.0, 0.0 };

        vec = Rotate('z', vec, phi0 - M_PI / 2);				// Поворот МГП до экваториальной плоскости

        vec = Shift('x', vec, R * cos(phi0));					// Сдвиг с.к. на ось вращения Земли

        vec = Rotate('y', vec, M_PI - (lambda0 + omega * time));// Поворот Xt до Xi

        vec = Shift('y', vec, -R * sin(phi0));					// Совмещение центров двух с.к.

        tmp[0] = vec[0];
        tmp[1] = -vec[2];
        tmp[2] = vec[1];

        return tmp;
    }

    Lin::Vector ISK_to_Geo(Lin::Vector vec, float time) {

        Lin::Vector tmp;  // Промежуточный результат
        tmp = { 0, 0, 0 };

        double r = sqrt(pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));

        tmp[0] = atan2(vec[2], sqrt(pow(vec[0], 2) + pow(vec[1], 2)));	// Широта
        tmp[1] = atan2(vec[1], vec[0]);					                // Долгота
        tmp[2] = r - R;													// Высота

        return tmp;
    }

    // todo перевод не в географическую СК а геодезическую
    Lin::Vector TSK_to_Geo(Lin::Vector vec, float time) {

        vec = TSK_to_ISK(vec, time);
        vec = ISK_to_Geo(vec, time);

        return vec;
    }

    Lin::Vector Geo_to_ISK(Lin::Vector vec, float time) {

        Lin::Vector tmp;  // Промежуточный результат
        tmp = { 0, 0, 0 };

        tmp[0] = (R + vec[2]) * cos(vec[0]) * cos(vec[1] + omega * time);
        tmp[1] = (R + vec[2]) * cos(vec[0]) * sin(vec[1] + omega * time);
        tmp[2] = (R + vec[2]) * sin(vec[0]);

        return tmp;
    }

    Lin::Vector ISK_to_TSK(Lin::Vector vec, float time) {

        Lin::Vector tmp;	// Промежуточный результат
        tmp = { 0.0, 0.0, 0.0 };

        tmp[0] = vec[0];
        tmp[1] = vec[2];
        tmp[2] = -vec[1];

        tmp = Shift('y', tmp, R * sin(phi0));

        tmp = Rotate('y', tmp, -M_PI + (lambda0 + omega * time));

        tmp = Shift('x', tmp, -R * cos(phi0));

        tmp = Rotate('z', tmp, M_PI / 2 - phi0);

        return tmp;
    }

    Lin::Vector Geo_TSK(Lin::Vector vec, float time) {
       
        vec = Geo_to_ISK(vec, time);
        vec = ISK_to_TSK(vec, time);

        return vec;
    }

    Lin::Vector getRight(const Lin::Vector& v, double t)
    {
        // todo: костыли с каунтами - персмотреть алгоритм определения поворотов
        //Lin::Vector target(3);
        /*if (start != 2)
            return Lin::Vector(5);*/
        
       ppm = list_ppm[count_targ];

        if (abs(v[0] - ppm[0]) < 150 && abs(v[2] - ppm[2]) < 70)
        {
            Lin::Vector geo;
            Lin::Vector tsk;
            tsk = { v[0],  0 , v[2] };
            geo = TSK_to_Geo(tsk, 0) * 180 / M_PI;
            Way.push_back(geo);

            count_targ++;
            if (count_targ == list_ppm.size())
            {
                count_targ = 0;
                //end.store(true);
                for (int i = 0; i < list_rotation.size(); ++i)
                    list_rotation[i] = 0;
            }
            ppm = list_ppm[count_targ];
            count = 1;           
        }
        
        double g = 9.81;

        gamma = 0;
        //double psi = 15 * GR2RAD;
        theta = 0 * GR2RAD;
        double P = 120000;  // Тяга двигателей
        //double nxa = 0;//sin(theta);
        double nxa;
        
        if (v[3] < 300)
            nxa = P / (m * g);
        else
            nxa = 0;

        if (v[3] > 50 && v[1] < 6000)
            theta = 20 * GR2RAD;
        else theta = 0;

        
        Lin::Vector tmp(v.size());
        Lin::Vector v_sv;
        Lin::Vector target_sv;


        v_sv = { v[0], v[1], v[2] };
        
        target_sv = norm2svyaz(ppm, gamma, theta, v[4]);
        v_sv = norm2svyaz(v_sv, gamma, theta, v[4]);
        gamma = 0;

        centerrad(v, ppm, -g / v[3] * tan(15 * GR2RAD), &list_rotation[count_targ], (v_sv[2] - target_sv[2]));

        // Логика совершения маневров 
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
        tmp[3] = g * (nxa - sin(theta));                 // V' в траекторной СК
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

    Lin::Vector svyaz_to_norm(Lin::Vector& v, double gamma, double theta, double psi)
    {
        Lin::Matrix M(3, 3);

        M = { cos(psi) * cos(theta),                                sin(theta),             -sin(psi) * cos(theta),
        sin(psi) * sin(gamma) - cos(psi) * sin(theta) * cos(gamma), cos(theta) * cos(gamma), cos(psi) * sin(gamma) + sin(psi) * sin(theta) * cos(gamma),
        sin(psi) * cos(gamma) + cos(psi) * sin(theta) * sin(gamma), -cos(theta) * sin(gamma), cos(psi) * cos(gamma) - sin(psi) * sin(theta) * sin(gamma) };

        M = M.T();


        return (M * v);
    }

    int centerrad(Lin::Vector v, Lin::Vector& targ, double dPSI, int* rotation, double error)
    {
        int sign = 1;
        if (error > 0)
            sign *= -1;

        if (*rotation == 1)
            return 0;
        *rotation = 1;
        double Ra = 200. * 200 / (9.81 * tan(15 * GR2RAD));
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
        
        if ((X[4] - last_psi) > 0)
            gamma = -15 * GR2RAD;
        if ((X[4] - last_psi) < 0)
            gamma = 15 * GR2RAD;
        if (X[4] == last_psi)
            gamma = 0 * GR2RAD;
        
        last_psi = X[4];

        std::vector<double> tmp;

        tmp.push_back(t);
        for (int i = 0; i < v.size(); ++i)
            tmp.push_back(v[i]);

        fprintf(output, "%lf  %lf  %lf  %lf  %lf  %lf  %lf  %lf\n", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], gamma, theta, tmp[5]);
        
    }

};

#endif
 