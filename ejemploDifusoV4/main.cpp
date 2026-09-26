#include "difuso.h"

int main()
{
    float dist=60,ang=-10;

    Difuso difuso;

    difuso.agregarVarEnt("dist",0.0,100.0);
    difuso.agregarVarEnt("ang",-45.0,45.0);
    difuso.agregarVarSal("potMI",-100.0,100.0);
    difuso.agregarVarSal("potMD",-100.0,100.0);

    difuso.varEnt["dist"].agregarConj("cero",0,0,50);       // Cero (Z)
    difuso.varEnt["dist"].agregarConj("media",0,50,100);    // Media (M)
    difuso.varEnt["dist"].agregarConj("lejos",50,100,100);  // Lejos (L)

    difuso.varEnt["ang"].agregarConj("negativo",-45,-45,0); // Negativo (N)
    difuso.varEnt["ang"].agregarConj("cero",-45,0,45);      // Cero (Z)
    difuso.varEnt["ang"].agregarConj("positivo",0,45,45);   // Positivo (P)

    difuso.varSal["potMI"].agregarConj("GN",-100,-100,-50);  // 0 - Grande Negativa (GN)
    difuso.varSal["potMI"].agregarConj("MN",-100,-50,0);     // 1 - Media Negativa (MN)
    difuso.varSal["potMI"].agregarConj("Z",-50,0,50);        // 2 - Cero (Z)
    difuso.varSal["potMI"].agregarConj("MP",0,50,100);       // 3 - Media Positiva (MP)
    difuso.varSal["potMI"].agregarConj("GP",50,100,100);     // 4 - Grande Positiva (GP)

    difuso.varSal["potMD"].agregarConj("GN",-100,-100,-50);  // 0 - Grande Negativa (GN)
    difuso.varSal["potMD"].agregarConj("MN",-100,-50,0);     // 1 - Media Negativa (MN)
    difuso.varSal["potMD"].agregarConj("Z",-50,0,50);        // 2 - Cero (Z)
    difuso.varSal["potMD"].agregarConj("MP",0,50,100);       // 3 - Media Positiva (MP)
    difuso.varSal["potMD"].agregarConj("GP",50,100,100);     // 4 - Grande Positiva (GP)

    difuso.ordenReglas("dist","ang","potMI","potMD");

    difuso.agregarRegla("cero","negativo","MN","MP");
    difuso.agregarRegla("cero","cero","Z","Z");
    difuso.agregarRegla("cero","positivo","MP","MN");
    difuso.agregarRegla("media","negativo","Z","MP");
    difuso.agregarRegla("media","cero","MP","MP");
    difuso.agregarRegla("media","positivo","MP","Z");
    difuso.agregarRegla("lejos","negativo","MP","GP");
    difuso.agregarRegla("lejos","cero","GP","GP");
    difuso.agregarRegla("lejos","positivo","GP","MP");

    Difuso::Res res=difuso.calcular(dist,ang);
    //--------------------------------------------------
    cout << "Potencia Motor Izquierdo = " << res.salida1 <<"%" << endl;
    cout << "Potencia Motor Derecho = " << res.salida2 <<"%" << endl;
    return 0;
}
