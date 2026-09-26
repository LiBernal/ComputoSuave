#include "difuso.h"

void Difuso::agregarCd(string nombre, float a, float b, float c) {
    Cd.push_back({nombre,a,b,c});
}

void Difuso::agregarCa(string nombre, float a, float b, float c) {
    Ca.push_back({nombre,a,b,c});
}

void Difuso::agregarCp(string nombre, float a, float b, float c) {
    Cp.push_back({nombre,a,b,c});
}

int indConj(string nombre, vector<Conjunto> conjunto) {
    for (int i=0; i<conjunto.size(); i++) {
        if (nombre==conjunto[i].nombre) {
            return i;
        }
    }
    cout << "Error con el conjunto: " << nombre << endl;
    exit(1);
}

void Difuso::distLimites(float distLI, float distLS) {
    this->distLI=distLI;
    this->distLS=distLS;
}

void Difuso::angLimites(float angLI, float angLS) {
    this->angLI=angLI;
    this->angLS=angLS;
}

void Difuso::agregarRegla(int dist, int ang, int pmi, int pmd) {
    reglas.push_back({dist,ang,pmi,pmd});
}

void Difuso::agregarRegla(string dist, string ang, string pmi, string pmd) {
    int iCd=indConj(dist,Cd);
    int iCa=indConj(ang,Ca);
    int iCpmi=indConj(pmi,Cp);
    int iCpmd=indConj(pmd,Cp);
    reglas.push_back({iCd,iCa,iCpmi,iCpmd});
}

Difuso::Res Difuso::calcular(float dist, float ang) {
    const float EPSILON = 1e-4;
    if (dist<=distLI) dist=distLI+EPSILON;
    if (dist>=distLS) dist=distLS-EPSILON;
    if (ang<=angLI) ang=angLI+EPSILON;
    if (ang>=angLS) ang=angLS-EPSILON;

    const int nCd=3;
    float pCd[nCd];
    for (int i=0; i<nCd; i++) {
        pCd[i]=Cd[i].pert(dist);
    }
    const int nCa=3;
    float pCa[nCa];
    for (int i=0; i<nCa; i++) {
        pCa[i]=Ca[i].pert(ang);
    }
    float W[nCd*nCa];
    for (int i=0; i<nCd*nCa; i++) {
        W[i]=min(pCd[reglas[i].dist],pCa[reglas[i].ang]);
    }
    float numerPMI=0,numerPMD=0,denom=0;
    for (int i=0; i<nCd*nCa; i++) {
        numerPMI+=W[i]*Cp[reglas[i].pmi].b;
        numerPMD+=W[i]*Cp[reglas[i].pmd].b;
        denom+=W[i];
    }
    float potMI=numerPMI/denom;
    float potMD=numerPMD/denom;
    return {potMI,potMD};
}
