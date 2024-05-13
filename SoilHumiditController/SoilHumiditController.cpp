#include "SoilHumiditController.h"

SoilHumiditController::SoilHumiditController(uint8_t sensorUmidadeSolo) {
  this->sensorUmidadeSolo = sensorUmidadeSolo;
}

void SoilHumiditController::begin() {
  pinMode(this->sensorUmidadeSolo, INPUT);
}


void SoilHumiditController::update(JsonObject& soilHumidit, StatusTransferenciaVolume tanqueStatus, uint8_t releBombaAguaTanque) {
  
  int soilHumiditValue = map(analogRead(this->sensorUmidadeSolo), 1023, 0, 0, 100);

  soilHumidit["value"] = soilHumiditValue;
  soilHumidit["color"] = (soilHumiditValue < 40.0) ? "red" : "green";
  soilHumidit["controlStatus"]["color"] =(soilHumiditValue < 40 && tanqueStatus == 1) ? "green" : "white";

  soilHumiditValue < 40 
    ? (tanqueStatus == 1 
        ? digitalWrite(releBombaAguaTanque, LOW)
        : digitalWrite(releBombaAguaTanque, HIGH))
    : digitalWrite(releBombaAguaTanque, HIGH);
}

