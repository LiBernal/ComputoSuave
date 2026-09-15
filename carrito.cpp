#include <iostream>
#include <cmath>  // Para usar abs()

using namespace std;

int main()
{
    //entradas para validar el ejemplo
    float dist = 60, ang = -10;
    float eps=1e-6;
    //límites de la distancia
    if (dist<=0) dist=0+eps;
    if (dist>=100) dist=100-eps;
    //límites del angulo
    if (ang<=-45) ang=-45+eps; //limite inferior + eps
    if (ang>=45) ang=45-eps; //limite sup + eps

    //Conjuntos de entrada
    //Conjuntos distancia
    const int nCd=3;
    float Cd[nCd][3] = {{0, 0, 50}, //Cero (Z)
                        {0, 50, 100}, // Medio (M)
                        {50, 100, 100}}; //Lejos (L) 

    //Conjuntos angulo
    const int nCa=3;
    float Ca[nCa][3] = {{-45, -45, 0}, // Negativo (N)
                        {-45, 0, 45}, // Cero (Z)
                        {0, 45, 45}}; // Positivo (P)
    
    //Variable de salida potencia
    //Conjuntos potencia
    const int nCp=5;
    float Cp[nCp][3] = {{-100, -100, -50}, // GM (Grande Negativa)
                        {-100, -50, 0}, // MN (Media Negativa)
                        {-50, 0, 50}, // Z (Cero)
                        {0, 50, 100}, //  MP (Media Positiva)
                        {50, 100, 100}}; // GP (Grande Positiva)
    
    //Asociar las reglas con las salidas
    //Se trabaja como matriz, filas = dist, ang = columnas
    //                          aN, aZ, aP
    int reg_potPMI[nCd][nCa] = {{1, 2, 3}, //dZ
                                {2, 3, 3}, //dM
                                {3, 4, 4}}; //dL

    //                          aN, aZ, aP
    int reg_potPMD[nCd][nCa] = {{3, 2, 1}, //dZ
                                {3, 3, 2}, //dM
                                {4, 4, 3}}; //dL

    //----------------------------------------------- Aquí termina el Diseño de conjuntos
    //Pertenencia a los conjuntos de distancia
    float pCd[nCd]; //a, b, c
    for (int i=0; i<nCd; i++) {
        float a=Cd[i][0], b=Cd[i][1], c=Cd[i][2];
        if (dist<=a) pCd[i]=0;
        else if (dist<=b) pCd[i]=(dist-a)/(b-a);
        else if (dist<c) pCd[i]=(c-dist)/(c-b);
        else pCd[i]=0;
    }

    //Pertenencia a los conjuntos de angulo
    float pCa[nCa]; //a, b, c
    for (int i=0; i<nCa; i++) {
        float a=Ca[i][0], b=Ca[i][1], c=Ca[i][2];
        if (ang<=a) pCa[i]=0;
        else if (ang<=b) pCa[i]=(ang-a)/(b-a);
        else if (ang<c) pCa[i]=(c-ang)/(c-b);
        else pCa[i]=0;
    }

    //w1 = si el error es negativo (calor), la salida es enfriar
    //w2 = si el error es cero (temp. ideal), la salida es apagar
    //w3 = si el error es positivo (calor), la salida es calentar
    
    float W[nCd][nCa];
    for(int i=0; i<nCd; i++) {
        for(int j=0; j<nCa; j++) {
            W[i][j]=min(pCd[i], pCa[j]);
        }
    }

    float numerPMI=0, numerPMD=0, denom=0;
    for (int i=0; i<nCd; i++) {
        for (int j=0; j<nCa; j++){
            numerPMI += W[i][j] * Cp[reg_potPMI[i][j]][1];
            numerPMD += W[i][j] * Cp[reg_potPMD[i][j]][1];
            denom += W[i][j];
        }
    }
    float potPMI = numerPMI/denom;
    float potPMD = numerPMD/denom;
    //---------------------------------------- Aquí termina el Motor de inferencia
    cout << "Potencia del Motor Izquierdo = " << potPMI << "%" << endl;
    cout << "Potencia del Motor Derecho = " << potPMD << "%" << endl;

    return 0;
}
