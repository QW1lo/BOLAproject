//---------------------------------------------------------------------------

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

    void addResult(const Lin::Vector& v, double t)
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
    Proverka(Lin::Vector& X) :TModel(X) {};
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


class LA : TModel
{
public:

    Lin::Vector getRight(const Lin::Vector& v, double t)
    {
        Lin::Vector tmp(v.size());
        for (int i = 0; i < v.size(); ++i)
            tmp[i] = v[i];

        tmp[0] = 2 * t;
        tmp[1] = 2 * t;
        tmp[2] = 2 * t;


        return tmp;
    }
};

#endif
 