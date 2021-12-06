#include "Lab2.h"

std::mt19937 generator;

void Sat_NS::test_control() {
    std::cout << "test control start\n";
    mut.lock();

    ARINC_FEATURES pack;                        // 
    pack.bits.adress = 273;                     // 
    pack.bits.R_data = 0;                       // 
    pack.bits.type_sns = 3;                     //
    pack.bits.almanach_gps = 1;                 //
    pack.bits.almanach_glo = 1;                 //
    pack.bits.time_f = 0;                       //
    pack.bits.empty = 0;                        // Формирование
    pack.bits.mode = 0;                         // слова 
    pack.bits.diff = 0;                         // состояния
    pack.bits.submode = 0;                      //
    pack.bits.empty1 = 0;                       //
    pack.bits.reject = 1;                       //
    pack.bits.signal_step = 0;                  //
    pack.bits.SK = 0;                           //
    pack.bits.empty2 = 0;                       //
    pack.bits.SM = 0;                           //
    pack.bits.P = 0;                            //

    //delete[] buffer;
    buffer = new char[sizeof(pack)];            // 
    memset(buffer, 0, sizeof(pack));            // Заполнение буфера
    memcpy(buffer, &pack, sizeof(pack));        //

    mut.unlock();

    auto start = std::chrono::system_clock::now();                  // 
    auto end = std::chrono::system_clock::now();                    // 
    std::chrono::duration<double> elapsed_seconds = end - start;    // Имитация 
    while (elapsed_seconds.count() < 2) {                          // 20 сек теста
        end = std::chrono::system_clock::now();                     //
        elapsed_seconds = end - start;                              //
    };                                                              //

    mut.lock();
    pack.bits.mode = 1;         // Тест  
    pack.bits.submode = 1;      // контроль   
    pack.bits.reject = 0;       // пройден
    pack.bits.P = 1;            // + функция бита четности

    //delete[] buffer;
    buffer = new char[sizeof(pack)];            // 
    memset(buffer, 0, sizeof(pack));            // Заполнение буфера
    memcpy(buffer, &pack, sizeof(pack));        //

    mut.unlock();
    std::cout << "test control end\n";
}

void Sat_NS::prepare() {
    std::cout << "prepare start\n";

    auto start = std::chrono::system_clock::now();                  // 
    auto end = std::chrono::system_clock::now();                    //
    std::chrono::duration<double> elapsed_seconds = end - start;    // Имитация 
    while (elapsed_seconds.count() < 2) {                         // 120 сек теста
        end = std::chrono::system_clock::now();                     //
        elapsed_seconds = end - start;                              //
    };                                                              //

    std::cout << "prepare end\n";
}

void Sat_NS::navigation() {
    if (count_nav == 0)
    {
        std::cout << "navigation start\n";
        delete[] buffer;
        buffer = new char[14 * sizeof(ARINC_BNR)];
        count_nav++;
    }

    std::normal_distribution<double> delta(0.0, 0.01);

    mut.lock();
    Lin::Vector pos;
    pos = { model->X[0], model->X[1], model->X[2] };
    pos = model->TSK_to_Geo(pos, 0);

    double h = pos[2] + delta(generator);                            //
    double hDOP = HDOP + delta(generator);                      //
    double vDOP = VDOP + delta(generator);                      // 
    double track_angle = Track_angle + delta(generator);        // 
    double cur_Phi = pos[0] + delta(generator);                // 
    double cur_Phi_p = Cur_Phi_p + delta(generator);            // Создание 
    double cur_Lambda = pos[1] + delta(generator);          // зашумленных
    double cur_Lambda_p = Cur_Lambda_p + delta(generator);      // измерений
    double delay = Delay + delta(generator);                    //
    double uTC_h = UTC_h + delta(generator);                    //
    double uTC_l = UTC_l + delta(generator);                    //  
    double v_h = V_h + delta(generator);                        //


    //delete buffer;
    buff_count = 0;
    //delete[] buffer;
            // 14 - кол-во слов состояний

    ARINC_BNR pack_1;
    pack_1.bits.adress = 76;        // Формирование 1-го слова
    pack_1.bits.value = bindec(table[pack_1.bits.adress].max_value, 20, table[pack_1.bits.adress].height, h);
    pack_1.bits.sign = 1;           //
    pack_1.bits.SM = 3;             //
    pack_1.bits.P = 1;              // + функция бита четности

    fill_buff<ARINC_BNR>(pack_1);

    ARINC_BNR pack_2;               // Формирование 2-го слова
    pack_2.bits.adress = 101;       //
    pack_2.bits.value = bindec(table[pack_2.bits.adress].max_value, 20, table[pack_2.bits.adress].height, hDOP);
    pack_2.bits.sign = 1;           //
    pack_2.bits.SM = 3;             //
    pack_2.bits.P = 1;              // + функция бита четности

    fill_buff<ARINC_BNR>(pack_2);

    ARINC_BNR pack_3;               // Формирование 3-го слова
    pack_3.bits.adress = 102;       //
    pack_3.bits.value = bindec(table[pack_3.bits.adress].max_value, 20, table[pack_3.bits.adress].height, vDOP);
    pack_3.bits.sign = 1;           //
    pack_3.bits.SM = 3;             //
    pack_3.bits.P = 1;              // + функция бита четности

    fill_buff<ARINC_BNR>(pack_3);

    ARINC_BNR pack_4;               // Формирование 4-го слова
    pack_4.bits.adress = 103;       //
    pack_4.bits.value = bindec(table[pack_4.bits.adress].max_value, 20, table[pack_4.bits.adress].height, track_angle);
    pack_4.bits.sign = 1;           //
    pack_4.bits.SM = 3;             //
    pack_4.bits.P = 1;              // + функция бита четности

    fill_buff<ARINC_BNR>(pack_4);

    ARINC_BNR pack_5;               // Формирование 5-го слова
    pack_5.bits.adress = 110;       //
    pack_5.bits.value = bindec(table[pack_5.bits.adress].max_value, 20, table[pack_5.bits.adress].height, cur_Phi);
    pack_5.bits.sign = 1;           //
    pack_5.bits.SM = 3;             //
    pack_5.bits.P = 1;              // + функция бита четности

    fill_buff<ARINC_BNR>(pack_5);

    ARINC_BNR pack_6;               // Формирование 6-го слова
    pack_6.bits.adress = 120;       //
    pack_6.bits.value = bindec(table[pack_6.bits.adress].max_value, 20, table[pack_6.bits.adress].height, cur_Phi_p);
    pack_6.bits.sign = 1;           //
    pack_6.bits.SM = 3;             //
    pack_6.bits.P = 1;              // + функция бита четности

    fill_buff<ARINC_BNR>(pack_6);

    ARINC_BNR pack_7;               // Формирование 7-го слова
    pack_7.bits.adress = 111;       //
    pack_7.bits.value = bindec(table[pack_7.bits.adress].max_value, 20, table[pack_7.bits.adress].height, cur_Lambda);
    pack_7.bits.sign = 1;           //
    pack_7.bits.SM = 3;             //
    pack_7.bits.P = 1;              // + функция бита четности

    fill_buff<ARINC_BNR>(pack_7);

    ARINC_BNR pack_8;               // Формирование 8-го слова
    pack_8.bits.adress = 121;       //
    pack_8.bits.value = bindec(table[pack_8.bits.adress].max_value, 20, table[pack_8.bits.adress].height, cur_Lambda_p);
    pack_8.bits.sign = 1;           //
    pack_8.bits.SM = 3;             //
    pack_8.bits.P = 1;              // + функция бита четности

    fill_buff<ARINC_BNR>(pack_8);

    ARINC_BNR pack_9;               // Формирование 9-го слова
    pack_9.bits.adress = 113;       //
    pack_9.bits.value = bindec(table[pack_9.bits.adress].max_value, 20, table[pack_9.bits.adress].height, delay);
    pack_9.bits.sign = 1;           //
    pack_9.bits.SM = 3;             //
    pack_9.bits.P = 1;              // + функция бита четности

    fill_buff<ARINC_BNR>(pack_9);

    ARINC_BNR pack_10;              // Формирование 10-го слова
    pack_10.bits.adress = 150;      //
    pack_10.bits.value = bindec(table[pack_10.bits.adress].max_value, 20, table[pack_10.bits.adress].height, uTC_h);
    pack_10.bits.sign = 1;          //
    pack_10.bits.SM = 3;            //
    pack_10.bits.P = 1;             // + функция бита четности

    fill_buff<ARINC_BNR>(pack_10);

    ARINC_BNR pack_11;              // Формирование 11-го слова
    pack_11.bits.adress = 140;      //
    pack_11.bits.value = bindec(table[pack_11.bits.adress].max_value, 20, table[pack_11.bits.adress].height, uTC_l);
    pack_11.bits.sign = 1;          //
    pack_11.bits.SM = 3;            //
    pack_11.bits.P = 1;             // + функция бита четности

    fill_buff<ARINC_BNR>(pack_11);

    ARINC_BNR pack_12;              // Формирование 12-го слова
    pack_12.bits.adress = 165;      //
    pack_12.bits.value = bindec(table[pack_12.bits.adress].max_value, 20, table[pack_12.bits.adress].height, v_h);
    pack_12.bits.sign = 1;          //
    pack_12.bits.SM = 3;            //
    pack_12.bits.P = 1;             // + функция бита четности

    fill_buff<ARINC_BNR>(pack_12);

    ARINC_DATE pack_13;                             // 
    pack_13.bits.adress = 260;                      //
    pack_13.bits.empty1 = 0;                        //
    pack_13.bits.Y_u = Year - Year / 10 * 10;       // Формирование 
    pack_13.bits.Y_d = Year / 10;                   // слова
    pack_13.bits.M_u = Mounth - Mounth / 10 * 10;   // дата
    pack_13.bits.M_d = Mounth / 10;                 //
    pack_13.bits.D_u = Day - Day / 10 * 10;         //
    pack_13.bits.D_d = Day / 10;                    //
    pack_13.bits.empty2 = 1;                        //
    pack_13.bits.SM = 0;                            //
    pack_13.bits.P = 1;                             // + функция бита четности

    fill_buff<ARINC_DATE>(pack_13);

    ARINC_FEATURES pack_14;
    pack_14.bits.adress = 273 - 256;                     // 
    pack_14.bits.R_data = 0;                       // 
    pack_14.bits.type_sns = 3;                     //
    pack_14.bits.almanach_gps = 1;                 //
    pack_14.bits.almanach_glo = 1;                 //
    pack_14.bits.time_f = 0;                       // Формирование
    pack_14.bits.empty = 0;                        // слова 
    pack_14.bits.mode = 1;                         // признаки
    pack_14.bits.diff = 0;                         //
    pack_14.bits.submode = 1;                      //   
    pack_14.bits.empty1 = 0;                       //
    pack_14.bits.reject = 0;                       // 
    pack_14.bits.signal_step = 0;                  //
    pack_14.bits.SK = 0;                           //
    pack_14.bits.empty2 = 0;                       //
    pack_14.bits.SM = 0;                           //
    pack_14.bits.P = 1;                            // + функция бита четности

    fill_buff<ARINC_FEATURES>(pack_14);

    mut.unlock();

}
void Sat_NS::test_send()
{
    mut.lock();
    ARINC_DISCRETE pack;                        // 
    pack.bits.adress = 270;                     //
    pack.bits.SDI = 1;                          //
    pack.bits.preparation = 0;                  //
    pack.bits.control = 0;                      //
    pack.bits.navigation = 1;                   //
    pack.bits.gyro = 0;                         //
    pack.bits.empty1 = 0;                       //
    pack.bits.re_start = 0;                     //
    pack.bits.scale = 0;                        // 
    pack.bits.heat = 1;                         //
    pack.bits.therm = 0;                        //
    pack.bits.init_data = 0;                    // 
    pack.bits.rec_H = 1;                        //
    pack.bits.INS = 0;                          //
    pack.bits.acceleration = 0;                 //
    pack.bits.ready = 0;                        //
    pack.bits.empty2 = 0;                       //
    pack.bits.SM = 2;                           //
    pack.bits.P = 1;                            //

    //delete[] buffer;
    buffer = new char[sizeof(pack)];            // 
    memset(buffer, 0, sizeof(pack));            // Заполнение буфера
    memcpy(buffer, &pack, sizeof(pack));        //
    mut.unlock();
}
template <class T>
void Sat_NS::fill_buff(T pack)
{
    char* buff;
    buff = new char[sizeof(pack)];					  // 
    memset(buff, 0, sizeof(pack));                    // Заполнение буфера
    memcpy(buff, &pack, sizeof(pack));				  //

    for (int i = 0; i < 4; ++i)
    {
        buffer[4 * buff_count + i] = buff[i];
    }
    buff_count++;
    delete[] buff;
}

void Sat_NS::send_pack() {
    mut.lock();
    if (not startup)
    {
        sendto(_s, &buffer[0], sizeof(buffer), 0,           //
            (sockaddr*)&_destAddr, sizeof(_destAddr));		// 
                                                            //
    }
    else
    {
        char buff[4];
        int count_word = 0;
        for (int i = 0; i < 14; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                buff[j] = buffer[count_word * 4 + j];
            }

            sendto(_s, &buff[0], sizeof(buff), 0,
                (sockaddr*)&_destAddr, sizeof(_destAddr));
            count_word++;
        }

    }

    mut.unlock();
    std::cout << "send " << std::endl;
}

void  Sat_NS::run_sns()
{
    if (not startup)
    {
        test_control();
        prepare();
        startup = 1;
        model->start++;
    }
    navigation();
}




void In_NS::test_control() {
    std::cout << "test control start\n";
    mut.lock();

    ARINC_DISCRETE pack;                        // 
    pack.bits.adress = 270;                     //
    pack.bits.SDI = 1;                          //
    pack.bits.preparation = 0;                  //
    pack.bits.control = 0;                      //
    pack.bits.navigation = 1;                   //
    pack.bits.gyro = 0;                         //
    pack.bits.empty1 = 0;                       //
    pack.bits.re_start = 0;                     //
    pack.bits.scale = 0;                        // 
    pack.bits.heat = 1;                         //
    pack.bits.therm = 0;                        //
    pack.bits.init_data = 0;                    // 
    pack.bits.rec_H = 1;                        //
    pack.bits.INS = 0;                          //
    pack.bits.acceleration = 0;                 //
    pack.bits.ready = 0;                        //
    pack.bits.empty2 = 0;                       //
    pack.bits.SM = 2;                           //
    pack.bits.P = 1;                            //

    //delete[] buffer;
    buffer = new char[sizeof(pack)];            // 
    memset(buffer, 0, sizeof(pack));            // Заполнение буфера
    memcpy(buffer, &pack, sizeof(pack));        //

    mut.unlock();

    auto start = std::chrono::system_clock::now();                  // 
    auto end = std::chrono::system_clock::now();
    double time_test = 2;// 
    std::chrono::duration<double> elapsed_seconds = end - start;    // Имитация 
    while (elapsed_seconds.count() < time_test) {
        end = std::chrono::system_clock::now();                     //
        elapsed_seconds = end - start;                              //
    };                                                              //

    mut.lock();
    pack.bits.init_data = 0;    // тест контроль
    pack.bits.INS = 1;          //  пройден
    pack.bits.P = 1;            // + функция бита четности

    delete[] buffer;
    buffer = new char[sizeof(pack)];            // 
    memset(buffer, 0, sizeof(pack));            // Заполнение буфера
    memcpy(buffer, &pack, sizeof(pack));        //

    mut.unlock();
    std::cout << "test control end\n";
}

void In_NS::prepare() {
    mut.lock();
    std::cout << "prepare start\n";

    ARINC_DISCRETE pack;                        // 
    pack.bits.adress = 270;                     //
    pack.bits.SDI = 1;                          //
    pack.bits.preparation = 0;                  //
    pack.bits.control = 0;                      //
    pack.bits.navigation = 1;                   //
    pack.bits.gyro = 0;                         //
    pack.bits.empty1 = 0;                       //
    pack.bits.re_start = 0;                     //
    pack.bits.scale = 1;                        // 
    pack.bits.heat = 1;                         //
    pack.bits.therm = 0;                        //
    pack.bits.init_data = 0;                    // 
    pack.bits.rec_H = 1;                        //
    pack.bits.INS = 1;                          //
    pack.bits.acceleration = 0;                 //
    pack.bits.ready = 0;                        //
    pack.bits.empty2 = 0;                       //
    pack.bits.SM = 0;                           //
    pack.bits.P = 1;                            //

    mut.unlock();

    auto start = std::chrono::system_clock::now();                  // 
    auto end = std::chrono::system_clock::now();                    //
    int change = 1;                                                 //
    double time_pr = 5;											// 120 сек теста
    std::chrono::duration<double> elapsed_seconds = end - start;    // Имитация 
    while (elapsed_seconds.count() < (time_pr)) {                   //    
        end = std::chrono::system_clock::now();                     // со шкалой подготовки
        elapsed_seconds = end - start;                              //
        if (elapsed_seconds.count() / (time_pr / 6.) - change > 1)
        {														    //
            change += 1;                                            //
            mut.lock();                                           //
            pack.bits.scale = change;                               //
            mut.unlock();                                         //
        };                                                          //        
    };                                                              //

    mut.lock();
    pack.bits.scale = 7;
    std::cout << "prepare end\n";
    mut.unlock();
}

void In_NS::navigation() {
    if (count_nav == 0)
    {
        std::cout << "navigation start\n";
        delete[] buffer;
        buffer = new char[13 * sizeof(ARINC_BNR)];
        count_nav++;
    }

    mut.lock();
    Lin::Vector pos;
    pos = { model->X[0], model->X[1], model->X[2] };
    pos = model->TSK_to_Geo(pos, 0);

    std::normal_distribution<double> delta(0.0, 0.01);

    double phi = pos[0] + delta(generator);
    double lambda = pos[1] + delta(generator);
    double h = pos[2] + delta(generator);
    double psi = model->X[4] + delta(generator);
    double theta = model->theta + delta(generator);
    double gamma = model->gamma + delta(generator);
    double v_ns = V_ns + delta(generator);
    double v_ev = V_ev + delta(generator);
    double v_h_inertial = V_h_inertial + delta(generator);
    double a_x = A_x + delta(generator);
    double a_z = A_z + delta(generator);
    double a_y = A_y + delta(generator);


    buff_count = 0;
    // 13 - кол-во слов состояний

    ARINC_BNR pack_1;
    pack_1.bits.adress = 310 - 256; // Формирование 1-го слова
    pack_1.bits.value = bindec(table[pack_1.bits.adress].max_value, 20, table[pack_1.bits.adress].height, phi); // преобраование в двоичный формат
    pack_1.bits.sign = 1; //
    pack_1.bits.SM = 3; //
    pack_1.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_1);

    ARINC_BNR pack_2;
    pack_2.bits.adress = 311 - 256; // Формирование 2-го слова
    pack_2.bits.value = bindec(table[pack_2.bits.adress].max_value, 20, table[pack_2.bits.adress].height, lambda); // преобраование в двоичный формат
    pack_2.bits.sign = 1; //
    pack_2.bits.SM = 3; //
    pack_2.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_2);

    ARINC_BNR pack_3;
    pack_3.bits.adress = 361 - 256; // Формирование 3-го слова
    pack_3.bits.value = bindec(table[pack_3.bits.adress].max_value, 20, table[pack_3.bits.adress].height, h);
    pack_3.bits.sign = 1; //
    pack_3.bits.SM = 3; //
    pack_3.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_3);

    ARINC_BNR pack_4;
    pack_4.bits.adress = 314 - 256; // Формирование 4-го слова
    pack_4.bits.value = bindec(table[pack_4.bits.adress].max_value, 20, table[pack_4.bits.adress].height, psi);
    pack_4.bits.sign = 1; //
    pack_4.bits.SM = 3; //
    pack_4.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_4);

    ARINC_BNR pack_5;
    pack_5.bits.adress = 324 - 256; // Формирование 5-го слова
    pack_5.bits.value = bindec(table[pack_5.bits.adress].max_value, 20, table[pack_5.bits.adress].height, theta);
    pack_5.bits.sign = 1; //
    pack_5.bits.SM = 3; //
    pack_5.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_5);

    ARINC_BNR pack_6;
    pack_6.bits.adress = 325 - 256; // Формирование 6-го слова
    pack_6.bits.value = bindec(table[pack_6.bits.adress].max_value, 20, table[pack_6.bits.adress].height, gamma);
    pack_6.bits.sign = 1; //
    pack_6.bits.SM = 3; //
    pack_6.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_6);

    ARINC_BNR pack_7;
    pack_7.bits.adress = 366 - 256; // Формирование 7-го слова
    pack_7.bits.value = bindec(table[pack_7.bits.adress].max_value, 20, table[pack_7.bits.adress].height, v_ns);
    pack_7.bits.sign = 1; //
    pack_7.bits.SM = 3; //
    pack_7.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_7);

    ARINC_BNR pack_8;
    pack_8.bits.adress = 367 - 256; // Формирование 8-го слова
    pack_8.bits.value = bindec(table[pack_8.bits.adress].max_value, 20, table[pack_8.bits.adress].height, v_ev);
    pack_8.bits.sign = 1; //
    pack_8.bits.SM = 3; //
    pack_8.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_8);

    ARINC_BNR pack_9;
    pack_9.bits.adress = 365 - 256; // Формирование 9-го слова
    pack_9.bits.value = bindec(table[pack_9.bits.adress].max_value, 20, table[pack_9.bits.adress].height, v_h_inertial);
    pack_9.bits.sign = 1; //
    pack_9.bits.SM = 3; //
    pack_9.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_9);

    ARINC_BNR pack_10;
    pack_10.bits.adress = 331 - 256; // Формирование 10-го слова
    pack_10.bits.value = bindec(table[pack_10.bits.adress].max_value, 20, table[pack_10.bits.adress].height, a_x);
    pack_10.bits.sign = 1; //
    pack_10.bits.SM = 3; //
    pack_10.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_10);

    ARINC_BNR pack_11;
    pack_11.bits.adress = 332 - 256; // Формирование 11-го слова
    pack_11.bits.value = bindec(table[pack_11.bits.adress].max_value, 20, table[pack_11.bits.adress].height, a_z);
    pack_11.bits.sign = 1; //
    pack_11.bits.SM = 3; //
    pack_11.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_11);

    ARINC_BNR pack_12;
    pack_12.bits.adress = 333 - 256; // Формирование 12-го слова
    pack_12.bits.value = bindec(table[pack_12.bits.adress].max_value, 20, table[pack_12.bits.adress].height, a_y);; // + сделать функцию Q
    pack_12.bits.sign = 1; //
    pack_12.bits.SM = 3; //
    pack_12.bits.P = 1; // + функция бита четности

    fill_buff<ARINC_BNR>(pack_12);

    ARINC_DISCRETE pack_13; // Проверять
    pack_13.bits.adress = 270 - 256; //
    pack_13.bits.SDI = 1; //
    pack_13.bits.preparation = 0; //
    pack_13.bits.control = 0; //
    pack_13.bits.navigation = 1; //
    pack_13.bits.gyro = 0; //
    pack_13.bits.empty1 = 0; //
    pack_13.bits.re_start = 0; //
    pack_13.bits.scale = 0; //
    pack_13.bits.heat = 1; //
    pack_13.bits.therm = 0; //
    pack_13.bits.init_data = 0; //
    pack_13.bits.rec_H = 1; //
    pack_13.bits.INS = 1; //
    pack_13.bits.acceleration = 0; //
    pack_13.bits.ready = 0; //
    pack_13.bits.empty2 = 0; //
    pack_13.bits.SM = 0; //
    pack_13.bits.P = 1; //

    fill_buff<ARINC_DISCRETE>(pack_13);

    mut.unlock();

}
template <class T>
void In_NS::fill_buff(T pack)
{
    char* buff;
    buff = new char[sizeof(pack)];					  // 
    memset(buff, 0, sizeof(pack));                    // Заполнение буфера
    memcpy(buff, &pack, sizeof(pack));				  //

    for (int i = 0; i < 4; ++i)
    {
        buffer[4 * buff_count + i] = buff[i];
    }
    buff_count++;
    delete[] buff;
}

void In_NS::send_pack() {
    mut.lock();
    if (not startup)
    {
        sendto(_s, &buffer[0], sizeof(buffer), 0,           //
            (sockaddr*)&_destAddr, sizeof(_destAddr));		// Переделать
                                                            //
    }
    else
    {
        int count_word = 0;
        for (int i = 0; i < 13; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                buff2send[j] = buffer[count_word * 4 + j];
            }

            sendto(_s, &buff2send[0], sizeof(buff2send), 0,
                (sockaddr*)&_destAddr, sizeof(_destAddr));
            count_word++;
        }
        //delete[] buff;

    }

    mut.unlock();
    std::cout << "send " << std::endl;
}

void In_NS::run_ins() {
    if (not startup)
    {
        test_control();
        prepare();
        startup = 1;
        model->start++;
    }
    navigation();
}



//int main()
//{
//	setlocale(LC_ALL, "rus");
//    Sat_NS SNS(10, 10.0, 10.0, 5.0, 55, 3, 35, 4, 13.3, 6.0, 2.0, 1.0, 21, 10, 18); // Класс СНС
//    In_NS INS(33, 55, 130, 15.3, 3.5, 6.3245, 400, 200, 6400, 0, 0, 0);
//
//    char buff[10 * 1014];
//
//    // Шаг 1 - иницилизация библиотеки Winsocks
//    if (WSAStartup(0x202, (WSADATA*)&buff[0]))
//    {
//        printf("WSAStartup error: %d\n",
//            WSAGetLastError());
//        return -1;
//    }
//
//    // Шаг 2 - открытие сокета
//    _s = socket(AF_INET, SOCK_DGRAM, 0);
//    if (_s == INVALID_SOCKET)
//    {
//        printf("socket() error: %d\n", WSAGetLastError());
//        WSACleanup();
//        return -1;
//    }
//
//    // Шаг 3 - обмен сообщений с сервером
//    HOSTENT* hst;
//    _destAddr;
//    _destAddr.sin_family = AF_INET;
//    _destAddr.sin_port = htons(PORT);
//
//    // определение IP-адреса узла
//    if (inet_addr(SERVERADDR))
//        _destAddr.sin_addr.s_addr = inet_addr(SERVERADDR);
//
//    else
//    {
//        if (hst = gethostbyname(SERVERADDR))
//            _destAddr.sin_addr.s_addr = ((unsigned long**)hst->h_addr_list)[0][0];
//
//        else
//        {
//            printf("Unknown host: %d\n", WSAGetLastError());
//            closesocket(_s);
//            WSACleanup();
//			return -1;
//        }
//    }
//
//    Timer timer;
//    timer.add(std::chrono::milliseconds(100), [&]() {SNS.run_sns(); });
//    timer.add(std::chrono::milliseconds(1000), [&]() {SNS.send_pack(); });
//    timer.add(std::chrono::milliseconds(10), [&]() {INS.send_pack(); });
//    timer.add(std::chrono::microseconds(2500), [&]() {INS.run_ins(); });
//    
//	while (true) { std::this_thread::sleep_for(std::chrono::seconds(3600)); };
//};