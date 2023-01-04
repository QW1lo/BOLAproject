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
    
    double last_psi = 0;
    
    int count_targ = 0;
    double m = 40000;   // масса ВС
    
    int count = 1;      

    int R = 6371000;										// Радиус земли, м
    double omega = 0.00007292115;							// Угловая скорость Земли, рад

    double phi0 = 0;			                            // Начальная широта, долгота и
    double lambda0 = 0;		                                // нулевая высота т.к. стартовая
    double h0 = 0;											// точка на поверхности Земли
                                              // Режим полета 0 - ППМ, 1 - посадка по глиссаде
    double K_land = 0;                                      // Курс ВПП
    double theta_land = 0;                                  // Угол наклона глиссады
    
    

public:
    std::vector<int> list_rotation{ 1};
    std::vector<int> list_tcas{ 5 };
    std::vector<Lin::Vector> list_ppm;
    FILE* output;
    int N;
    std::vector<Lin::Vector> Way;                           // Для КМЛ путь в ГСК
    int stop_integ = 0;
    std::vector<Lin::Vector> Way_glis;
    int mode = 0;
    Lin::Vector target;                                     // Вектор В ТСК цели
    int count_t = 0;
    Lin::Vector ppm;                                        // Вектор в ТСК текущего ппм

    double gamma = 0;
    double theta = 0;
    int TCAS;

    Lin::Vector dvxz{ 2 };

    Lin::Vector del_glissade{ 3 };                               // невязки координат и глиссады
    Lin::Vector vxyz{3};
    double T0 = 0;

    // todo конструктор для программы с ВПП, начало ТСК на ВПП
    // todo Управление по отклонению с глиссады

    LA(Lin::Vector& X0, std::vector<Lin::Vector>& Init_ppms, Lin::Vector target0) :TModel(X0)
    {
        // X0 - ВС формата {phi0, lbd0, h0, V0, PSI0}
        output = fopen("LAoutput.txt", "w");

        phi0 = 56.1448638889 * M_PI / 180;
        lambda0 = 34.9926805556 * M_PI / 180;
        h0 = X0[2];
        target = Geo_TSK(target0 * M_PI / 180, 0);
        target[1] = 0;
        //target[2] = 1000;

        //for (int i = 0; i < 3; i++) {
        //    X[i] = 0;
        //}
        Lin::Vector Xstart;
        Xstart = { X[0], X[1], X[2] };
        Xstart = Geo_TSK(Xstart, 0);
        
        X[0] = Xstart[0];
        X[1] = X0[2];
        X[2] = Xstart[2];


        for (int i = 0; i < Init_ppms.size(); i++) {
            Lin::Vector tmp;
            tmp = Geo_TSK(Init_ppms[i] * M_PI / 180, 0);
            list_ppm.push_back(tmp);
            list_rotation.push_back(0);
        }
        mode = 0;
    };

    LA(Lin::Vector& X0, Lin::Vector& Land, double K_land0, double theta_land0, double D_La0, double H_La0, double beta_La0, int number) :TModel(X0)
    {
        // beta_la - угол отклонения от глиссады ЛА в азимутальной плоскости, D_la - Дистанция до ЛА от ВПП, K_land - Курс ВПП
        // theta_land- наклон глиссады
        // X0 - ВС формата {phi0, lbd0, h0, V0, PSI0}
        output = fopen("LAoutput.txt", "w");
        //gps_output = fopen("gps.txt", "w");

        phi0 = Land[0];
        lambda0 = Land[1];
        h0 = H_La0;

        K_land = K_land0;
        theta_land = theta_land0;
        // Определение нач координат ЛА в ТСК 
        start_coord_LA(K_land, D_La0, H_La0, beta_La0);
        mode = 1;
        N = number;
    };


    LA(Lin::Vector& X0, std::vector<Lin::Vector>& Init_ppms, double H0, int number) :TModel(X0)
    {
        // X0 - ВС формата {phi0, lbd0, h0, V0, PSI0}
        output = fopen("LAoutput.txt", "w");

        // За [0, 0] берется аэродром орловка
        phi0 = 56.1448638889 * M_PI / 180;
        lambda0 = 34.9926805556 * M_PI / 180;
        h0 = H0;

        Lin::Vector Xstart;
        Xstart = { Init_ppms[0][0] * M_PI / 180, Init_ppms[0][1] * M_PI / 180, Init_ppms[0][2] };
        Xstart = Geo_TSK(Xstart, 0);

        X[0] = Xstart[0];
        X[1] = Xstart[1];
        X[2] = Xstart[2];


        for (int i = 0; i < Init_ppms.size(); i++) {
            Lin::Vector tmp(3);
            tmp = { Init_ppms[i][0] * M_PI / 180, Init_ppms[i][1] * M_PI / 180, Init_ppms[i][2]};
            tmp = Geo_TSK(tmp, 0);
            list_ppm.push_back(tmp);
            list_rotation.push_back(0);
        }
        mode = 5;
        N = number;
        count_targ++;
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
        
        // начало разбега ВС после выставки СНС и БИНС
        /*if (start != 2)
            return Lin::Vector(5);*/
        Lin::Vector tmp(v.size());

        double g = 9.81;

        gamma = 0;
        //double psi = 15 * GR2RAD;
        theta = 0 * GR2RAD;
        double P = 120000;  // Тяга двигателей
        //double nxa = 0;//sin(theta);
        double nxa;
        nxa = 0;
        

        if (mode == 0 || mode == 2 || mode == 5)
        {
            ppm = list_ppm[count_targ];

            if (abs(v[0] - ppm[0]) < 150 && abs(v[2] - ppm[2]) < 70)
            {
                Lin::Vector geo;
                Lin::Vector tsk;
                tsk = { v[0],  0 , v[2] };
                geo = TSK_to_Geo(tsk, 0) * 180 / M_PI;
                Way.push_back(geo);

                count_targ++;

                if (mode == 2 && count_targ == list_ppm.size())
                {
                    count_targ = 0;
                    mode = 1;
                }

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

            if (mode == 0 || mode == 2)
            {
                if (v[3] > 50 && v[1] < ppm[1])
                    theta = 20 * GR2RAD;
                else theta = 0;

                if (abs(X[1] - ppm[1]) > 0.5)
                {
                    if ((X[1] - ppm[1]) < 0)
                    {
                        theta = 15 * GR2RAD;

                    }
                    else
                    {
                        theta = -15 * GR2RAD;
                    }

                }

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
                            gamma = 45 * GR2RAD;

                        }
                        else
                        {
                            //centerrad(v, target, -g / v[3] * tan(-15 * GR2RAD), &list_rotation[count_targ]);
                            gamma = -45 * GR2RAD;
                        }

                    }
                    else
                    {
                        gamma = 0;
                    }
                }
                if (mode == 2)
                {
                    Lin::Vector ppmgl;
                    ppmgl = glissade();
                    v_sv = { v[0], v[1], v[2] };
                    target_sv = norm2svyaz(ppm, 0, 0, v[4]);
                    v_sv = norm2svyaz(v_sv, 0, 0, v[4]);
                    del_glissade = target_sv - v_sv;
                }
            }
        }
        if (mode == 1 || mode == 5)
        {
            Lin::Vector v_sv;
            Lin::Vector target_sv;
            //
            if (mode == 1)
                ppm = glissade();
            else
                ppm = airway();
            //
            v_sv = { v[0], v[1], v[2] };
            //target_sv = ppm - v_sv;

            //double kurse = atan2(v[2], v[0]);
            //if (abs(K_land + M_PI - v[4]) > 0.002)
            //{
            //    if ((K_land + M_PI - v[4]) > 0)
            //    {
            //        gamma = -4 * GR2RAD;
            //    }
            //    else
            //    {
            //        gamma = 4 * GR2RAD;
            //    }
            //}
            //else
            //{
            //    gamma = 0;
            //    
            //}

            if (abs(X[1] - ppm[1]) > 0.5)
            {
                if ((X[1] - ppm[1]) < 0)
                {
                    theta = 15 * GR2RAD;
            
                }
                else
                {
                    theta = -15 * GR2RAD;
                }
            
            }
            else
            {
                theta = -3 * GR2RAD;
            }
            
            target_sv = norm2svyaz(ppm, 0, 0, v[4]);
            v_sv = norm2svyaz(v_sv, 0, 0, v[4]);
            gamma = 0;
            
            //todo логика управления на глиссаду
            double delZ = v_sv[2] - target_sv[2];
            
            if (abs(delZ) > 1)
            {
                //if ((delZ) < 0)
                //{
                //    if (delZ < 0)
                //    gamma = 45 * GR2RAD;
                //}
                //else
                //{
                //    gamma = -45 * GR2RAD;
                //}

                if ((delZ) < 0)
                {
                    if (delZ < -190)
                        gamma = 38 * GR2RAD;
                    else
                        gamma = -delZ * 0.2 * GR2RAD;
                    
                    if (gamma < 5 * GR2RAD)
                        gamma = 5 * GR2RAD;
                }
                else
                {
                    if (delZ > 190)
                        gamma = -38 * GR2RAD;
                    else
                        gamma = -delZ * 0.2 * GR2RAD;
                    
                    if (gamma > -5 * GR2RAD)
                        gamma = -5 * GR2RAD;
                }
            
            }
            else
            {
                gamma = 0;
            }

            
            del_glissade = target_sv - v_sv;
            std::cout  << N << "  " << del_glissade[2] << "  " << del_glissade[1] << "\n";
            

        }
        // vlevo
        if (mode == 10)
        {
            if (count_t == 0)
            {
                T0 = t;
                count_t++;
            }
            gamma = -38 * GR2RAD;
            if (t - T0 > 45)
            {
                mode = 5;
          
            }
        }

        // vpravo
        if (mode == 11)
        {
            if (count_t == 0)
            {
                T0 = t;
                count_t++;
            }
            gamma = 38 * GR2RAD;
            if (t - T0 > 45)
            {
                mode = 5;

            }
        }

        if (mode == 15)
        {
            theta = 0;
            gamma = 0;
        }

        if (v[3] < 100)
            nxa = P / (m * g);
        else
            //nxa = 0;
            nxa = sin(theta);

        tmp[0] = v[3] * cos(theta) * cos(v[4]);          // xg' (1.5)
        tmp[1] = v[3] * sin(theta);                      // yg'
        tmp[2] = -v[3] * cos(theta) * sin(v[4]);         // zg'
        tmp[3] = g * (nxa - sin(theta));                 // V' в траекторной СК
        tmp[4] = -g / v[3] * tan(gamma);                 // PSI' = wy (5.8-5.9)
        
        dvxz[0] = tmp[0] ;
        dvxz[1] = tmp[2] ;

        vxyz[0] = tmp[0];
        vxyz[1] = tmp[1];
        vxyz[2] = tmp[2];

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
        double Ra = v[3] * v[3] / (9.81 * tan(45 * GR2RAD));
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

        // TODO if mode

        fprintf(output, "%lf  %lf  %lf  %lf  %lf  %lf  %lf  %lf  %lf  %lf\n", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], gamma, theta, tmp[5] * RAD2GR, del_glissade[2], del_glissade[1]);
        if (X[1] < 0.6)
        {
            stop_integ = 1;
            //end.store(true);
            
        }
            

        Lin::Vector geo;
        Lin::Vector tsk;
        tsk = { v[0],  0 , v[2] };
        geo = TSK_to_Geo(tsk, 0) * 180 / M_PI;
        geo[2] = v[1];
        Way.push_back(geo);
        
    }

    // beta_la - угол отклонения от глиссады ЛА в азимутальной плоскости, D_la - Дистанция до ЛА от ВПП, K_land - Курс ВПП
    void start_coord_LA(double K_land, double D_La, double H_La, double beta_La)
    {
        double gamma = K_land - beta_La;
        
        X[0] = D_La * cos(gamma);
        X[1] = H_La;
        X[2] = D_La * sin(gamma);
    }
    // theta - угол наклона глиссады
    // K_land - курс ВПП
    Lin::Vector glissade()
    {
        Lin::Vector X_la;
        Lin::Vector Glissade;
        X_la = { X[0], X[1], X[2] };
        double R = X_la.length();
        Glissade = { R, 0 ,0 };
        Glissade = Rotate('z', Glissade, -theta_land);
        Glissade = Rotate('y', Glissade, K_land);

        double h;
        if (theta_land == 0)
            h = h0;
        else
            h = Glissade[1];
        
        Lin::Vector geo;
        Lin::Vector tsk;
        tsk = { Glissade[0],  0 , Glissade[2] };

        geo = TSK_to_Geo(tsk, 0) * 180 / M_PI;
        geo[2] = h;
        Way_glis.push_back(geo);
        
        Glissade[0] = R-1000;
        Glissade[1] = 0;
        Glissade[2] = 0;
        Glissade = Rotate('z', Glissade, -theta_land);
        Glissade = Rotate('y', Glissade, K_land);
        Glissade[1] = h;

        return Glissade;     
    }

    Lin::Vector airway()
    {
        // По аналогии с глиссадой, предполагается, что самолет будет пытаться лететь в точку, находящуюся между ним и ППМ на расстоянии
        // от себя в 1км
        Lin::Vector X_la;
        Lin::Vector way;
        Lin::Vector way2;
        Lin::Vector D1;
        Lin::Vector ppm1;
        Lin::Vector ppm2;
        int i = 1;
        int j = 0;
        
        if (count_targ == 1)
        {
            i = 0;
            j = 1;
        }

        X_la = { X[0], 0, X[2] };

        D1 = {ppm[0], 0, ppm[2]};
        way = D1-X_la;
        
        way = way.norm();
        way = way * 1000;
        way = X_la + way;
        way[1] = h0;


       
        
        ppm1 = { list_ppm[i][0], 0, list_ppm[i][2] };
        ppm2 = { list_ppm[j][0], 0, list_ppm[j][2] };
        Lin::Vector way22;
        way22 = X_la - ppm2;
        Lin::Vector dor;
        
        dor = ppm1 - ppm2;
        //double angle = acos(dor * way2 / (dor.length() * way2.length()));
        //double angle1 = atan2(dor.cross(way2).length(), dor * way2);
        double sin1 = dor[0] * way22[2] - way22[0] * dor[2];
        double cos1 = dor[0] * way22[0] + dor[2] * way22[2];
        double angle = -atan2(sin1, cos1);
        Lin::Vector tmp;
        tmp = way22.norm();
        tmp = tmp * 1000;
        way2 = way22 - tmp;

        way2 = Rotate('y', way2, angle);
        way2[1] = h0;
        way2 = way2 + ppm2;
        
        //double zway = way[2] * cos(angle) - way[0] * sin(angle);
        //double xway = way[2] * sin(angle) + way[0] * cos(angle);
        //double xway_n = xway / sqrt(xway * xway + zway * zway);
        //double  zway_n = zway / sqrt(xway * xway + zway * zway);
        //xway -= xway_n * 800;
        //zway -= zway_n * 800;
        //way2[0] = xway + ppm2[0];
        //way2[1] = h0;
        //way2[2] = zway + ppm2[2];
        // D1 - D2 =vektor dorogi
        // angle(doroga, way2)
        // way 2 povernut na angle и вычесть из него 1к



        return way2;
    }
};

#endif
 