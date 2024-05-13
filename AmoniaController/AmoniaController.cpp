#include "AmoniaController.h"

AmoniaController::AmoniaController(
  uint8_t sensorAmonia,
  uint8_t indicadorAmoniaBaixa,
  uint8_t indicadorAmoniaMedia,
  uint8_t indicadorAmoniaAlta
) {
  this->sensorAmonia = sensorAmonia;
  this->indicadorAmoniaBaixa = indicadorAmoniaBaixa;
  this->indicadorAmoniaMedia = indicadorAmoniaMedia;
  this->indicadorAmoniaAlta = indicadorAmoniaAlta;
}

void AmoniaController::begin() {
  pinMode(this->sensorAmonia, INPUT);
  pinMode(this->indicadorAmoniaBaixa, OUTPUT);
  pinMode(this->indicadorAmoniaMedia, OUTPUT);
  pinMode(this->indicadorAmoniaAlta, OUTPUT);
}


void AmoniaController::update(
  JsonObject& amonia, 
  StatusTransferenciaVolume statusReservatorio, 
  StatusTransferenciaVolume statusTanque, 
  uint8_t releBombaAguaReservatorio
  ) {
  int amoniaValue = map(analogRead(this->sensorAmonia), 0, 1023, 0, 100);

  amonia["value"] = amoniaValue;
  amonia["color"] = (amoniaValue <= 20.0) 
                    ? "green" 
                    : (amoniaValue > 20 && amoniaValue <= 50) 
                      ? "yellow" 
                      : "red";
  amonia["controlStatus"]["color"] = amoniaValue > 50 
                            && statusReservatorio == 1 
                            ? "green" 
                            : "white";

  if (amoniaValue <= 20) {
    if (!statusTanque == 0) {
      digitalWrite(releBombaAguaReservatorio, HIGH);
    }

    digitalWrite(indicadorAmoniaBaixa, HIGH);  // Acende a luz verde
    digitalWrite(indicadorAmoniaMedia, LOW);
    digitalWrite(indicadorAmoniaAlta, LOW);
  }  
  if (amoniaValue > 20 && amoniaValue <= 50) {
   if (!statusTanque == 0) {
      digitalWrite(releBombaAguaReservatorio, HIGH);
    }

    digitalWrite(indicadorAmoniaMedia, HIGH);  // Acende a luz amarela
    digitalWrite(indicadorAmoniaBaixa, LOW);
    digitalWrite(indicadorAmoniaAlta, LOW);
  } 
  if (amoniaValue > 50) {
    statusReservatorio == 1 
      ? digitalWrite(releBombaAguaReservatorio, LOW) 
      : digitalWrite(releBombaAguaReservatorio, HIGH);

    digitalWrite(indicadorAmoniaAlta, HIGH);  // Acende a luz vermelha
    digitalWrite(indicadorAmoniaBaixa, LOW);
    digitalWrite(indicadorAmoniaMedia, LOW);
  }
}

