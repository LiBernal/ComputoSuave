#include <iostream>
#include <cmath>  // Para usar abs()

using namespace std;

int main()
{
    //temperatura deseada y actual
    float td=22, ta=19;
    //error
    float e=td-ta;
    //límites
    if (e<=-10) e=-9.999999;
    if (e>=10) e=9.999999;
    //conjuntos
    //variable para definir error
    //Conjuntos error
    const int nCe=3;
    float Ce[nCe][3] = {{-10, -10, 0}, //Negativo (N)(calor)
                        {-10, 0, 10}, //Cero (Z)(ideal)
                        {0, 10, 10}}; //Positivo (P)(Frio)

    //Conjuntos potencia de enfriamiento
    const int nCp=3;
    float Cp[nCp][3] = {{-100, -100, 0}, // Enfriar (ENF)
                        {-100, 0, 100}, // Apagar (OFF)
                        {0, 100, 100}}; // Calentar (CAL)
    
    int reg_pot[3]={0,1,2}; //Asociar las reglas con las salidas
    //----------------------------------------------- Aquí termina el Diseño de conjuntos
    //miu = pertenencia al conjunto error
    float pCe[3]; //0, 1, 2 (mN, mZ, mP)

    for (int i=0; i<nCe; i++) {
        float a=Ce[i][0], b=Ce[i][1], c=Ce[i][2];
        if (e<=a) pCe[i]=0;
        else if (e<=b) pCe[i]=(e-a)/(b-a);
        else if (e<c) pCe[i]=(c-e)/(c-b);
        else pCe[i]=0;
    }

    //w1 = si el error es negativo (calor), la salida es enfriar
    //w2 = si el error es cero (temp. ideal), la salida es apagar
    //w3 = si el error es positivo (calor), la salida es calentar
    float W[nCe];
    for(int i=0; i<nCe; i++) {
        W[i]=pCe[i];
        //W[0]=pCe[0]
        //grado de cumplimiento de R1 = grado pertenencia a conjunto N
    }

    float numer=0, denom=0;
    for (int i=0; i<nCe; i++) {
        numer += W[i]*Cp[reg_pot[i]][1];
        denom += W[i];
    }
    float pot=numer/denom;
    //---------------------------------------- Aquí termina el Motor de inferencia
    if (pot<0) {
        cout << "Potencia de Enfriamiento = " << abs(pot) <<"%" << endl;
    } else if (pot>0) {
        cout << "Potencia de Calefaccion = " << pot <<"%" << endl;
    } else {
        cout << "Temperatura ideal = " << endl;
    }
    return 0;
}
