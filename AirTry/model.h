//---------------------------------------------------------------------------

#pragma warning(disable : 4996)
#ifndef modelH
#define modelH

//---------------------------------------------------------------------------

//#include "model.h"
#include "Lin.h"

//---------------------------------------------------------------------------
class TModel
{
public:
    // ������� ������ ���������
    Lin::Vector X;

    // ��������� �����������
    std::vector<std::vector<double>> Result;


    // ����� ����������� ������� - ������������� ���������� �� ���������
    TModel(Lin::Vector& X0) { X = X0; }
    // ����������� ������������� ������� ������ ������ �� (X - ������ ���������, t - ����������� ��������)
    virtual Lin::Vector getRight(const Lin::Vector& v, double t) = 0;
    // ��������� ��������� �������
    inline Lin::Vector getInitialConditions() const { return X; }
    // ������� ������� - �� ����������� ������� ��������� �������
    inline int getOrder() const { return X.size(); }

    // ��������� ������� �����������
    inline std::vector<std::vector<double>> getResult() const { return Result; }

    // ������ ����������� (� ���� ������ � ���������� ��������� ������������ ������ � ���� 
    // ������ ��� ������ � ����������� ������� �����������)

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
        Lin::Vector tmp(v.size());
        for (int i = 0; i < v.size(); ++i)
            tmp[i] = v[i];

       /* tmp[0] = 2 * t;
        tmp[1] = 2 * t;
        tmp[2] = 2 * t;*/
        tmp[0] = t * t - 2*v[0];

        return tmp;
    }

    void addResult(const Lin::Vector& v, double t)
    {
        X = v;
        std::vector<double> tmp;

        tmp.push_back(t);
        for (int i = 0; i < v.size(); ++i)
            tmp.push_back(v[i]);

        Result.push_back(tmp);

        //for (int i = 0; i < tmp.size(); ++i)
        //    std::cout << tmp[i] << " ";
        //std::cout << std::endl;

        fprintf(output, "%lf  %lf\n", tmp[0], tmp[1]);
        
        //tmp.clear();

    }

};

#endif
 