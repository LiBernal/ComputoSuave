#ifndef DIFUSO_H
#define DIFUSO_H

#include <iostream>
#include <vector>
using namespace std;

class Conjunto{
public:
    string nombre;
    float a,b,c;
    float p=0;
    float pert(float valor) {
        if (valor<=a) p=0;
        else if (valor<=b) p=(valor-a)/(b-a);
        else if (valor<c) p=(c-valor)/(c-b);
        else p=0;
        return p;
    }
};

class Regla {
public:
    int dist, ang, pmi, pmd;
};

class Difuso {
public:
    float distLI,distLS;
    float angLI,angLS;
    vector<Conjunto> Cd;
    vector<Conjunto> Ca;
    vector<Conjunto> Cp;
    vector<Regla> reglas;
    void distLimites(float,float);
    void angLimites(float,float);
    struct Res {float potMI, potMD;};
    Res calcular(float,float);
    void agregarCd(string,float,float,float);
    void agregarCa(string,float,float,float);
    void agregarCp(string,float,float,float);
    void agregarRegla(int,int,int,int);
    void agregarRegla(string,string,string,string);
};

#endif // DIFUSO_H
