/*

//Include Libraries
#include <Arduino.h>

// Pins am Arduino für HX711
const int DOUT_PIN = 2;
const int SCK_PIN = 3;

// Kalibrierungsfaktoren für deinen Drucksensor (müssen entsprechend deinem Sensor angepasst werden)
const float SCALE_FACTOR = 1.0;  // Skalierungsfaktor zur Umrechnung in Druckwerte
const float OFFSET = 0.0;        // Offset-Korrektur (falls nötig)

void setup() {
  Serial.begin(115200);
  pinMode(DOUT_PIN, INPUT);
  pinMode(SCK_PIN, OUTPUT);
}

long readHX711() {
  long value = 0;
  byte data[3] = {0};
  
  while (digitalRead(DOUT_PIN));  // Warte, bis DOUT auf LOW geht
  for (int i = 0; i < 3; ++i) {
    for (int j = 7; j >= 0; --j) {
      digitalWrite(SCK_PIN, HIGH);
      bitWrite(data[i], j, digitalRead(DOUT_PIN));
      digitalWrite(SCK_PIN, LOW);
    }
  }
  
  digitalWrite(SCK_PIN, HIGH);
  digitalWrite(SCK_PIN, LOW);
  
  // Setze den ADC-Wandler in den Leerzustand
  data[2] ^= 0x80;
  value = (static_cast<long>(data[2]) << 16) | (static_cast<long>(data[1]) << 8) | data[0];
  
  return value;
}

void loop() {
  long rawValue = readHX711();
  
  // Hier musst du die spezifische Kalibrierung für deinen MPS2107-Sensor vornehmen,
  // um die tatsächliche Druckmessung zu berechnen. Der genaue Zusammenhang zwischen
  // dem Rohwert (rawValue) und dem Druck hängt von den spezifischen Eigenschaften deines Sensors ab.

  // Beispiel: Annahme, dass der MPS2107 0.5 V bei 0 PSI (0 kPa) und 4.5 V bei 100 PSI (689.48 kPa) ausgibt.
  // Beachte, dass dies nur ein Beispiel ist und du deine eigene Kalibrierung durchführen musst.
  float voltage = (rawValue * 5.0) / static_cast<float>(0x7FFFFF);
  float pressure_psi = (voltage - 0.5) * (100.0 / 4.0);
  float pressure_kpa = pressure_psi * 6.89476;

  // Anwenden der Kalibrierungsfaktoren
  pressure_psi = (pressure_psi * SCALE_FACTOR) + OFFSET;
  pressure_kpa = (pressure_kpa * SCALE_FACTOR) + OFFSET;

  // Ausgabe auf der seriellen Konsole
  Serial.print("Rohwert: ");
  Serial.print(rawValue);
  
  Serial.print(", Spannung: ");
  Serial.print(voltage, 4);
  Serial.print(" V");

  Serial.print(", Druck (PSI): ");
  Serial.print(pressure_psi, 2);
  Serial.print(" PSI");

  Serial.print(", Druck (kPa): ");
  Serial.print(pressure_kpa, 2);
  Serial.println(" kPa");

  delay(1000);  // Eine Sekunde warten, bevor die nächste Messung durchgeführt wird
}

*/

//Include Libraries
#include <Arduino.h>
#include "PressureSensor.h"

// Pins am Arduino für HX711
HX711 P1("P1",2,3);
HX711 P2("P2",4,5);
//const int DOUT_PIN = 2;
//const int SCK_PIN = 3;


void setup() {
  Serial.begin(115200);
  P1.setup();
  P2.setup();
  /*
  pinMode(DOUT_PIN, INPUT);
  pinMode(SCK_PIN, OUTPUT);
  */
}



void loop() {
  P1.read();
  P2.read();
  /*
  long rawValue = readHX711();
  
  // Hier musst du die spezifische Kalibrierung für deinen MPS2107-Sensor vornehmen,
  // um die tatsächliche Druckmessung zu berechnen. Der genaue Zusammenhang zwischen
  // dem Rohwert (rawValue) und dem Druck hängt von den spezifischen Eigenschaften deines Sensors ab.

  // Beispiel: Annahme, dass der MPS2107 0.5 V bei 0 PSI (0 kPa) und 4.5 V bei 100 PSI (689.48 kPa) ausgibt.
  // Beachte, dass dies nur ein Beispiel ist und du deine eigene Kalibrierung durchführen musst.
  float voltage = (rawValue * 5.0) / static_cast<float>(0x7FFFFF);
  float pressure_psi = (voltage - 0.5) * (100.0 / 4.0);
  float pressure_kpa = pressure_psi * 6.89476;

  // Anwenden der Kalibrierungsfaktoren
  pressure_psi = (pressure_psi * SCALE_FACTOR) + OFFSET;
  pressure_kpa = (pressure_kpa * SCALE_FACTOR) + OFFSET;
  */
  // Ausgabe auf der seriellen Konsole
  P1.printTest();
  P2.printTest();
  /*
  Serial.print("Rohwert: ");
  Serial.print(rawValue);
  
  Serial.print(", Spannung: ");
  Serial.print(voltage, 4);
  Serial.print(" V");

  Serial.print(", Druck (PSI): ");
  Serial.print(pressure_psi, 2);
  Serial.print(" PSI");

  Serial.print(", Druck (kPa): ");
  Serial.print(pressure_kpa, 2);
  Serial.println(" kPa");
  */
  delay(1000);  // Eine Sekunde warten, bevor die nächste Messung durchgeführt wird
}
