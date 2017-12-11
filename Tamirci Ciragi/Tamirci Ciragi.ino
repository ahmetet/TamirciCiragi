// ÇİZGİ İZLEYEN TASLAK
// 20.10.2017 - 22.10.2017
// ----------------------------------------------------------------------------------- QTR BAŞLANGIÇ -----------------------------------------------------------------------------------
#include <QTRSensors.h>
#define NUM_SENSORS             6
#define NUM_SAMPLES_PER_SENSOR  4
#define EMITTER_PIN             QTR_NO_EMITTER_PIN

boolean cizgi = 1; // 1 =beyaz,0=siyah
QTRSensorsAnalog qtra((unsigned char[]) {
  A0, A1, A2, A3, A4, A5
}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];
// ----------------------------------------------------------------------------------- QTR BİTİŞ ---------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------- MOTORLAR BAŞLANGIÇ ------------------------------------------------------------------------------
#define M_AIN1 9
#define M_AIN2 8
#define M_APWM 3

#define M_BIN1 10
#define M_BIN2 11
#define M_BPWM 5

double sagMax = 255;//175; // max değer // 200
double solMax = 255;//175; // max değer // 200
double sagBase ;//150; //70Motor tam olarak çizgideyken alacağı değer // 150 // 200
double solBase ;//150; //70 Motor tam olarak çizgideyken alacağı değer // 150 //200
double motorHiz = 0;
double sagmotorHiz = 0;
double solmotorHiz = 0;
// ----------------------------------------------------------------------------------- MOTORLAR BİTİŞ ----------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------- PID BAŞLANGIÇ -----------------------------------------------------------------------------------
int hata;
double setP = 2500;
double Kpigment = 0.075;
double Kdeniz = 0.952;
uint16_t say;
int SonHata = 0;
// ----------------------------------------------------------------------------------- PID BİTİŞ ---------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
 // Serial.begin(9600);


  KALIBRASYON();

}
void loop() {
  /*
    unsigned int position = qtra.readLine(sensorValues, 1, cizgi);
    hata = position - setP;

    for (int u = 0; u < 6; u++) {
    Serial.print(sensorValues[u]);
    Serial.print(" ");
    }
    Serial.print(" ");
    Serial.print(position);
    Serial.print(" ");
    Serial.print(hata);
    Serial.println();
 */
  YUZEY();
  PID();


}
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------S
// ----------------------------------------------------------------------------------- MOTORLAR BAŞLANGIÇ ------------------------------------------------------------------------------
void sag_geri (int h) {
  analogWrite(M_APWM, h);
  digitalWrite(M_AIN1, 1);
  digitalWrite(M_AIN2, 0);
}
void sol_geri (int h) {
  analogWrite(M_BPWM, h);
  digitalWrite(M_BIN1, 0);
  digitalWrite(M_BIN2, 1);
}

void sag_ileri(int h) {
  analogWrite(M_APWM, h);
  digitalWrite(M_AIN1, 0);
  digitalWrite(M_AIN2, 1);
}
void sol_ileri (int h) {
  analogWrite(M_BPWM, h);
  digitalWrite(M_BIN1, 1);
  digitalWrite(M_BIN2, 0);
}
// ----------------------------------------------------------------------------- MOTORLAR BİTİŞ ----------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------- QTR KALIBRASYON BAŞLANGIÇ -----------------------------------------------------------------------------
void KALIBRASYON() {
  for (int i = 0; i < 200; i++)
  {
    qtra.calibrate();
  }
}
// ----------------------------------------------------------------------------- QTR KALIBRASYON BİTİŞ ---------------------------------------------------------------------------------

// ----------------------------------------------------------------------------- PID BAŞLANGIÇ -----------------------------------------------------------------------------------------
void PID() {
  unsigned int position = qtra.readLine(sensorValues, 1, cizgi);
  hata = position - setP;
  if (abs(hata) >= 2500) {
    Kpigment = 1;
    Kdeniz = 0;
    sagBase = 0;
    solBase = 0;
  }
  else if (hata >= -2500 && hata <= 2500)
  {
    Kpigment = 0.075; // 0.045;
    Kdeniz = 0.952; // 1
    sagBase = 200; //225 170;
    solBase = 200; //225 170;
  }
  if (micros() - say > 10000) {
    motorHiz = Kpigment * hata + Kdeniz * (hata - SonHata);
    SonHata = hata;
    sagmotorHiz = sagBase - motorHiz;
    solmotorHiz = solBase + motorHiz;
   tamirci();
    say = micros();
  }
}
// ----------------------------------------------------------------------------- PID BİTİŞ ---------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------- YUZEY BAŞLANGIÇ ---------------------------------------------------------------------------------------
void YUZEY() {
  //  EKSTRA
  
  // EKSTRA
  if (sensorValues[0] >= 700 && sensorValues[1] >= 700 && sensorValues[4] >= 700 && sensorValues[5] > 700) {
    cizgi = 1;
  }
  if (sensorValues[0] <= 50 && sensorValues[1] <= 50 && sensorValues[4] <= 50 && sensorValues[5] <= 50) {
    cizgi = 0;
  }
}
// ----------------------------------------------------------------------------- YUZEY BİTİŞ -------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------- TAMIRCI BAŞLANGIÇ -------------------------------------------------------------------------------------
void tamirci() {
  if (sagmotorHiz > sagMax) {
    sagmotorHiz = sagMax;
  }

  if (sagmotorHiz < 0)
  {
    sag_geri(abs(sagmotorHiz)); // + lar ekstra
  }
  else
  {
    sag_ileri(sagmotorHiz);
  }

  if (solmotorHiz > solMax) {
    solmotorHiz = solMax;
  }

  if (solmotorHiz < 0)
  {
    sol_geri(abs(solmotorHiz));
  }
  else
  {
    sol_ileri(solmotorHiz);
  }
}
// ----------------------------------------------------------------------------- TAMIRCI BİTİŞ ------------------------------------------------------------------------------------------
