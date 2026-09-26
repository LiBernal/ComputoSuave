#ifndef DIFUSO_H
#define DIFUSO_H

#include <iostream>
#include <vector>
#include <map>
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
    int vE1, vE2, vS1, vS2;
};

class Variable {
public:
    float LI,LS;
    vector<Conjunto> conjuntos;
    void agregarConj(string nombre,float a,float b, float c) {
        conjuntos.push_back({nombre,a,b,c});
    }
};

class Difuso {
public:
    vector<Regla> reglas;
    map<string,Variable>varEnt;
    map<string,Variable>varSal;
    void agregarVarEnt(string,float,float);
    void agregarVarSal(string,float,float);
    struct Res {float salida1, salida2;};
    Res calcular(float,float);
    string vE1,vE2,vS1,vS2;
    void ordenReglas(string,string,string,string);
    void agregarRegla(int,int,int,int);
    void agregarRegla(string,string,string,string);
};

#endif // DIFUSO_H
