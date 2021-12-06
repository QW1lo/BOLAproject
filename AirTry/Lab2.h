#pragma once
#include <iostream>
//#include "Timer.cpp"
#include "atom.h"
#include "protocol.h"
#include "bin.h"
//#include <mutex>
#include <random>
#include "model.h"

extern SOCKET _s;
extern sockaddr_in _destAddr;
extern std::mutex mut;

class Sat_NS {
private:
    double H, HDOP, VDOP, Track_angle, Cur_Phi, Cur_Phi_p, Cur_Lambda, Cur_Lambda_p, Delay, UTC_h, UTC_l, V_h;
    int Year, Mounth, Day = 0;
    char* buffer;
    int buff_count;
    bool startup = 0;
    int count_nav = 0;

public:

    LA* model;

    Sat_NS(LA* model0, double H0, double HDOP0, double VDOP0, double Track_angle0, double Cur_Phi0,
        double Cur_Phi_p0, double Cur_Lambda0, double Cur_Lambda_p0, double Delay0, double UTC_h0,
        double UTC_l0, double V_h0, int Year0, int Mounth0, int Day0) {

        model = model0;
        H = H0;
        HDOP = HDOP0;
        VDOP = VDOP0;
        Track_angle = Track_angle0;
        Cur_Phi = Cur_Phi0;
        Cur_Phi_p = Cur_Phi_p0;
        Cur_Lambda = Cur_Lambda0;
        Cur_Lambda_p = Cur_Lambda_p0;
        Delay = Delay0;
        UTC_h = UTC_h0;
        UTC_l = UTC_l0;
        V_h = V_h0;
        Year = Year0;
        Mounth = Mounth0;
        Day = Day0;
    }

    void test_control();
    void prepare();
    void navigation();

    void test_send();
    void send_pack();

    template <class T>
    void fill_buff(T pack);

    void run_sns();
};

class In_NS {
private:
    double Phi, Lambda, H, Psi, Theta, Gamma, V_ns, V_ev, V_h_inertial, A_x, A_z, A_y;
    char* buffer;
    char buff2send[4];
    int buff_count;
    int count_nav = 0;
    bool startup = 0;

public:

    LA* model;

    In_NS(LA* model0, double Phi0, double Lambda0, double H0, double Psi0,
        double Theta0, double Gamma0, double V_ns0, double V_ev0,
        double V_h_inertial0, double A_x0, double A_y0, double A_z0) {

        model = model0;
        Phi = Phi0;
        Lambda = Lambda0;
        H = H0;
        Psi = Psi0;
        Theta = Theta0;
        Gamma = Gamma0;
        V_ns = V_ns0;
        V_ev = V_ev0;
        V_h_inertial = V_h_inertial0;
        A_x = A_x0;
        A_z = A_z0;
        A_y = A_y0;
    }

    void test_control();

    void prepare();

    void navigation();

    template <class T>
    void fill_buff(T pack);

    void send_pack();

    void run_ins();
};