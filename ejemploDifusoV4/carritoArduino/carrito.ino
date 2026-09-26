#include "difuso.h"
#include <AccelStepper.h>
#define HALFSTEP 8
#define M_IZQ_IN1 25
#define M_IZQ_IN2 26
#define M_IZQ_IN3 27
#define M_IZQ_IN4 14
#define M_DER_IN1 18
#define M_DER_IN2 17
#define M_DER_IN3 16
#define M_DER_IN4 4

AccelStepper motorIzq(HALFSTEP, M_IZQ_IN1, M_IZQ_IN3, M_IZQ_IN2, M_IZQ_IN4);
AccelStepper motorDer(HALFSTEP, M_DER_IN1, M_DER_IN3, M_DER_IN2, M_DER_IN4);
const unsigned long TIEMPO_CONTROL = 100;
const float PASOS_VUELTA = 4075.7696;
float ANGULO_PASO = ((360.0/PASOS_VUELTA)*PI)/180.0;
float RADIO_RUEDAS = 3.4;
float DIST_RUEDAS = 14;

struct Pos{float x, y;};
struct Error{float dist, ang;};
unsigned long tiempoInicio = 0;
long pasosAntMI = 0;
long pasosAntMD = 0;
Difuso::Res ctrlVel = {0.0,0.0};
Pos posObj = {60.0,60.0};
Pos posAct = {0.0,0.0};
float orientAct = 0.0;
int cont = 0;

Difuso difuso;

void setup() {
  Serial.begin(9600);
  Serial.println("=== INICIANDO SISTEMA ===");
  // Definición de límites, conjuntos y reglas difusas​
 
  //LImites o definicion del universo
  difuso.angLimites(-45,45);
  difuso.distLimites(0,100);
 
  //Conjuntos
  difuso.agregarCd("cero",0,0,50);      // Cero (Z)
  difuso.agregarCd("media",0,50,100);   // Media (M)
  difuso.agregarCd("lejos",50,100,100);  // Lejos (L)
 
  difuso.agregarCa("negativo",-45,-45,0); // Negativo (N)
  difuso.agregarCa("cero",-45,0,45);      // Cero (Z)
  difuso.agregarCa("positivo",0,45,45);   // Positivo (P)
 
  difuso.agregarCp("GN",-100,-100,-50);  // 0 - Grande Negativa (GN)
  difuso.agregarCp("MN",-100,-50,0);     // 1 - Media Negativa (MN)
  difuso.agregarCp("Z",-50,0,50);        // 2 - Cero (Z)
  difuso.agregarCp("MP",0,50,100);       // 3 - Media Positiva (MP)
  difuso.agregarCp("GP",50,100,100);     // 4 - Grande Positiva (GP)
 
  //Reglas
  difuso.agregarRegla("cero","negativo","MN","MP");
  difuso.agregarRegla("cero","cero","Z","Z");
  difuso.agregarRegla("cero","positivo","MP","MN");
  difuso.agregarRegla("media","negativo","Z","MP");
  difuso.agregarRegla("media","cero","MP","MP");
  difuso.agregarRegla("media","positivo","MP","Z");
  difuso.agregarRegla("lejos","negativo","MP","GP");
  difuso.agregarRegla("lejos","cero","GP","GP");
  difuso.agregarRegla("lejos","positivo","GP","MP");

  motorIzq.setMaxSpeed(1000.0);
  motorIzq.setSpeed(0);
  motorDer.setMaxSpeed(1000.0);
  motorDer.setSpeed(0);

  tiempoInicio = millis();
  delay(10000);

}
void actualizarPosicion() {
  // Calcular pasos de los motores (apoyados en currentPosition())
  long pasosActMI = motorIzq.currentPosition();
  long pasosActMD = motorDer.currentPosition();
  long deltaPasosMI = (pasosActMI - pasosAntMI) * -1.0;
  long deltaPasosMD = pasosActMD - pasosAntMD;

  // Calcular vueltas
  float vueltasMI = (ANGULO_PASO * deltaPasosMI)/(2*PI);
  float vueltasMD = (ANGULO_PASO * deltaPasosMD)/(2*PI);
  // Calcular avance llantas, y variación angular​
  // Perimetro = 2 * PI * RADIO_RUEDAS
  float perimetro = (2*PI*RADIO_RUEDAS);
  float avanceLlantaMI = vueltasMI * perimetro;
  float avanceLlantaMD = vueltasMD * perimetro;
  //Avance lineal del centro
  float avanceCentro = (avanceLlantaMD + avanceLlantaMI) / 2.0;
  //Variacion angular
  //W = DIST_RUEDAS
  float deltaTheta = (avanceLlantaMD - avanceLlantaMI) / DIST_RUEDAS;
  // Calcular nueva posición y nueva orientación​
  //Posicion eje x
  posAct.x = posAct.x + avanceCentro * cos((orientAct*PI/180.0) + (deltaTheta/2.0));
  // Posicion eje y
  posAct.y = posAct.y + avanceCentro * sin((orientAct*PI/180.0) + (deltaTheta/2.0));
  //Orientacion global
  orientAct = orientAct + deltaTheta*180.0/PI;

  pasosAntMI = pasosActMI;
  pasosAntMD = pasosActMD;
}
Error calcularError() {
  Error err;
  // Calcular error de distancia, nuevo ángulo objetivo y error de ángulo​
  //Error distancia
  err.dist = sqrt(pow(posObj.x - posAct.x,2) + pow(posObj.y - posAct.y,2));
  //Angulo objetivo
  float thetaObj = atan2(posObj.y-posAct.y, posObj.x - posAct.x);
  //Error de orientacion
  err.ang = thetaObj - orientAct;

  return err;
}

void loop() {
  motorIzq.runSpeed();
  motorDer.runSpeed();
  while (posAct.x != posObj.x && posAct.y != posObj.y) {
    if (millis() - tiempoInicio > TIEMPO_CONTROL) {
    // actualizar posición, calcular error, aplicar sistema difuso​
    // actualizar pos
    actualizarPosicion();
    Error error = calcularError();
    ctrlVel = difuso.calcular(error.dist, error.ang);
    //nuevas velocidades
    motorDer.setSpeed(ctrlVel.potMD*10.0);
    motorIzq.setSpeed(-ctrlVel.potMI*10.0);
    cont++;
    Serial.print("Error distancia: ");
    Serial.println(error.dist);
    Serial.print("Error angulo: ");
    Serial.println(error.ang);
    Serial.print("Control velocidad derecho: ");
    Serial.println(ctrlVel.potMD*10);
    Serial.print("Control velocidad izquierdo: ");
    Serial.println(ctrlVel.potMI*10);
    Serial.println(cont);
    Serial.println("-----------------------");
    // aplicar nuevas velocidades​
    tiempoInicio=millis();
    }
  }
}