#include <Servo.h>
#include <math.h>

int bomba = 7;       // Relé 1
int solenoide = 8;   // Relé 2

Servo servoX;
Servo servoY;

void setup() {

  Serial.begin(115200);

  // Inicialização dos servos motores
  servoX.attach(5);
  servoX.write(0);
  servoY.attach(6);
  servoY.write(0);

  // Inicialização da bomba e válvula
  pinMode(bomba, OUTPUT);
  pinMode(solenoide, OUTPUT);
  digitalWrite(bomba, HIGH);     // Desliga bomba
  digitalWrite(solenoide, HIGH);   // Desliga válvula

}

double mapAngle1(double linhas, double colunas){
  if (colunas == 0)
    return 1.57;
  double ang = atan(linhas/(colunas));
  if(ang < 0)
    return ang + M_PI; // Ensure the angle is positive
  return ang;
}

float iterativeAngle(double v, double L, double C, double g, double h){

  float ang = 0.001, angmin = 0.001;
  float value1 = sqrt(L * L + C * C) * g/v;
  float value2 = cos(ang) * (-v * sin(ang) + sqrt(v * v * sin(ang) * sin(ang) + 2 * g * h));
  float min = 0.1;
  
  for(ang = 0.001; ang <= 1.570; ang += 0.001){
    if(value1 - (cos(ang) * (-v * sin(ang) + sqrt(v * v * sin(ang) * sin(ang) + 2 * g * h))) < min && 
    value1 - (cos(ang) * (-v * sin(ang) + sqrt(v * v * sin(ang) * sin(ang) + 2 * g * h))) > -min){
      angmin = ang;
      min = value1 - (cos(ang) * (-v * sin(ang) + sqrt(v * v * sin(ang) * sin(ang) + 2 * g * h)));
    }
  }
  return angmin;
}

void loop() {

  if (Serial.available()) { // Verifica se há dados enviados pelo ESP -> FOGO DETECTADO
    String dados = Serial.readStringUntil('\n');
    int entradaIJ = dados.toInt(); // Recebe o valor inteiro enviado pelo ESP
    double focoJ = entradaIJ%100;
    double focoI = entradaIJ/100;

    Serial.println("Fogo detectado na posição I= " + String(focoI) + " e J= " + String(focoJ));

    double v = 1.7, L = (focoI/24) * 0.315 ,C = (focoJ - 16)/16 * 0.26, g = 9.81, h = 0.5;

    double angle = iterativeAngle(v, L, C, g, h);
    int angulo = (int)(angle * (180.0 / M_PI + 0.5));
    int angulo2 = (int)(mapAngle1(focoI, focoJ - 16) * (180.0 / M_PI + 0.5));

    // Gira os servos até a posição do fogo
    Serial.println("Girando motores...");

    Serial.println(angulo2);
    servoX.write(angulo2);
    delay(1000);

    Serial.println(angulo);
    servoY.write(angulo);
    delay(1000);
    
    // Ativa bomba e válvula por 1,5 segundos
    /*digitalWrite(bomba, LOW);
    digitalWrite(solenoide, LOW);
    delay(5000);

    digitalWrite(bomba, HIGH);
    digitalWrite(solenoide, HIGH);
    delay(5000);*/

    // Retorna os servos para as posições 0
    servoY.write(0);
    delay(600);
    servoX.write(0);
    delay(600);
    
  }
}


