#include "difuso.h"

void Difuso::agregarVarEnt(string nombre, float LI, float LS) {
    Variable variable;
    variable.LI=LI;
    variable.LS=LS;
    varEnt.insert({nombre,variable});
}

void Difuso::agregarVarSal(string nombre, float LI, float LS) {
    Variable variable;
    variable.LI=LI;
    variable.LS=LS;
    varSal.insert({nombre,variable});
}

void Difuso::agregarRegla(int vE1, int vE2, int vS1, int vS2) {
    reglas.push_back({vE1,vE2,vS1,vS2});
}

int indConj(string nombre, vector<Conjunto> conjunto) {
    for (int i=0; i<(int)conjunto.size(); i++) {
        if (nombre==conjunto[i].nombre) {
            return i;
        }
    }
    cout << "Error con el conjunto: " << nombre << endl;
    exit(1);
}

void Difuso::ordenReglas(string vE1, string vE2, string vS1, string vS2) {
    this->vE1=vE1;
    this->vE2=vE2;
    this->vS1=vS1;
    this->vS2=vS2;
}

void Difuso::agregarRegla(string e1, string e2, string s1, string s2) {
    int iCE1=indConj(e1,varEnt[vE1].conjuntos);
    int iCE2=indConj(e2,varEnt[vE2].conjuntos);
    int iCS1=indConj(s1,varSal[vS1].conjuntos);
    int iCS2=indConj(s2,varSal[vS2].conjuntos);
    reglas.push_back({iCE1,iCE2,iCS1,iCS2});
}

Difuso::Res Difuso::calcular(float e1, float e2) {
    const float EPSILON = 1e-4;
    if (e1<=varEnt[vE1].LI) e1=varEnt[vE1].LI+EPSILON;
    if (e1>=varEnt[vE1].LS) e1=varEnt[vE1].LS-EPSILON;
    if (e2<=varEnt[vE2].LI) e2=varEnt[vE2].LI+EPSILON;
    if (e2>=varEnt[vE2].LS) e2=varEnt[vE2].LS-EPSILON;
    const int nCE1=(int)varEnt[vE1].conjuntos.size();
    const int nCE2=(int)varEnt[vE2].conjuntos.size();
    float* pCE1 = new float[nCE1];
    for (int i=0; i<nCE1; i++) {
        pCE1[i] = varEnt[vE1].conjuntos[i].pert(e1);
    }
    float* pCE2 = new float[nCE2];
    for (int i=0; i<nCE2; i++) {
        pCE2[i] = varEnt[vE2].conjuntos[i].pert(e2);
    }
    float* W = new float[nCE1*nCE2];
    for (int i=0; i<nCE1*nCE2; i++) {
        W[i] = min(pCE1[reglas[i].vE1],pCE2[reglas[i].vE2]);
    }
    float numerVS1=0,numerVS2=0,denom=0;
    for (int i=0; i<nCE1*nCE2; i++) {
        numerVS1 += W[i]*varSal[vS1].conjuntos[reglas[i].vS1].b;
        numerVS2 += W[i]*varSal[vS2].conjuntos[reglas[i].vS2].b;
        denom += W[i];
    }
    float salida1=numerVS1/denom;
    float salida2=numerVS2/denom;
    delete[] pCE1;
    delete[] pCE2;
    delete[] W;
    return {salida1,salida2};
}
