#include "VolumeController.h"

VolumeController::VolumeController(
  float altura,
  float largura,
  float comprimento,
  float desconto,
  uint8_t trigger,
  uint8_t echo,
  uint8_t indicadorNivelBaixo,
  uint8_t indicadorNivelAlto,
  uint8_t releBombaAgua
) {
  this->altura = altura;
  this->largura = largura;
  this->comprimento = comprimento;
  this->desconto = desconto;
  this->trigger = trigger;
  this->echo = echo;
  this->indicadorNivelBaixo = indicadorNivelBaixo;
  this->indicadorNivelAlto = indicadorNivelAlto;
  this->releBombaAgua = releBombaAgua;
}

StatusTransferencia VolumeController::getStatusTransferencia() {
  return this->statusTransferencia;
}

void VolumeController::begin() {
  pinMode(this->trigger, OUTPUT);
  pinMode(this->echo, INPUT);
  pinMode(this->indicadorNivelAlto, OUTPUT);
  pinMode(this->indicadorNivelBaixo, OUTPUT);
  pinMode(this->releBombaAgua, OUTPUT);
  digitalWrite(this->releBombaAgua, HIGH);
}

float VolumeController::arredondar (float numero) {
  return round(numero * 2) / 2.0;
}

float VolumeController::distancia() {
  digitalWrite(this->trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(this->trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(this->trigger, LOW);

  long duration = pulseIn(this->echo, HIGH);

  return (duration * 0.034 / 2) - this->desconto;
}

float VolumeController::volume() {
  float distanciaLida = this->distancia();
  distanciaLida = distanciaLida > this->altura ? this->altura : distanciaLida;  
  return ((this->largura * this->comprimento) * (this->altura - distanciaLida))/1000; // ((area da base = largura * comprimento) * (altura = altura do recipiente - distancia medida pelo sonar)) / 1000 ----> volume em litros
}

DynamicJsonDocument VolumeController::update(StatusTransferencia status, uint8_t releBombaAgua) {
  float volumeMedido = this->volume();

  DynamicJsonDocument volume(1024);
  JsonObject controlStatus = volume.createNestedObject("controlStatus");

  volume["value"] = volumeMedido;
  volume["color"] = (volumeMedido < 2.0) ? "red" : "green";
  controlStatus["color"] = volumeMedido < 2.0 
                            && status == 1 
                            ? "green" 
                            : "white";

  if (volumeMedido <= 2.0) {
    this->statusTransferencia = NaoPermiteTransferir;

    status == 1 
      ? digitalWrite(releBombaAgua, LOW) 
      : digitalWrite(releBombaAgua, HIGH);

    digitalWrite(this->indicadorNivelBaixo, HIGH);
    digitalWrite(this->indicadorNivelAlto, LOW);

  } else {
    this->statusTransferencia = PermiteTransferir;

    digitalWrite(releBombaAgua, HIGH);

    digitalWrite(this->indicadorNivelBaixo, LOW);
    digitalWrite(this->indicadorNivelAlto, HIGH);
  }

  return volume;
}

DynamicJsonDocument VolumeController::update() {
  float volumeMedido = this->volume();

  DynamicJsonDocument volume(1024);

  volume["value"] = volumeMedido;
  volume["color"] = (volumeMedido < 2.0) ? "red" : "green";

  if (volumeMedido <= 2.0) {
    this->statusTransferencia = NaoPermiteTransferir;

    digitalWrite(this->indicadorNivelBaixo, HIGH);
    digitalWrite(this->indicadorNivelAlto, LOW);

  } else {
    this->statusTransferencia = PermiteTransferir;

    digitalWrite(this->indicadorNivelBaixo, LOW);
    digitalWrite(this->indicadorNivelAlto, HIGH);
  }

  return volume;
}

