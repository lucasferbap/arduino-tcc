#include "TemperatureController.h"

TemperatureController::TemperatureController(
    uint8_t aquecedor,
    uint8_t resfriador,
    uint8_t sensorTemperatura,
    uint8_t indicadorResfriamento,
    uint8_t indicadorAquecedor,
    uint8_t indicadorTemperaturaBaixa,
    uint8_t indicadorTemperaturaCorreta,
    uint8_t indicadorTemperaturaAlta,
    uint8_t sensorManualTemperatura)
    : oneWire(sensorTemperatura), sensorObject(&oneWire)
{
  this->aquecedor = aquecedor;
  this->resfriador = resfriador;
  this->indicadorResfriamento = indicadorResfriamento;
  this->indicadorAquecedor = indicadorAquecedor;
  this->indicadorTemperaturaBaixa = indicadorTemperaturaBaixa;
  this->indicadorTemperaturaCorreta = indicadorTemperaturaCorreta;
  this->indicadorTemperaturaAlta = indicadorTemperaturaAlta;
  this->sensorManualTemperatura = sensorManualTemperatura;
}

void TemperatureController::begin()
{
  this->sensorObject.begin();
  this->sensorObjectResfriamento.begin();

  pinMode(this->aquecedor, OUTPUT);
  pinMode(this->resfriador, OUTPUT);
  pinMode(this->indicadorResfriamento, OUTPUT);
  pinMode(this->indicadorAquecedor, OUTPUT);
  pinMode(this->indicadorTemperaturaBaixa, OUTPUT);
  pinMode(this->indicadorTemperaturaCorreta, OUTPUT);
  pinMode(this->indicadorTemperaturaAlta, OUTPUT);
}

DynamicJsonDocument TemperatureController::update(bool changeSensor)
{

  float temperatura;
  if (changeSensor) {
    static int ultimoValor = 0;
    const int histerese = 2;

    int valorPotenciometro =  map(analogRead(this->sensorManualTemperatura), 1023, 0, 0, 100);

    if (abs(valorPotenciometro - ultimoValor) > histerese) {
      ultimoValor = valorPotenciometro;
    }

    temperatura = ultimoValor;
  
  } else {
    //Serial.println("Medindo pelo sensor");
    this->sensorObject.requestTemperatures();
    temperatura = sensorObject.getTempCByIndex(0);
  }

  DynamicJsonDocument temperature(1024);
  JsonObject controlStatus = temperature.createNestedObject("controlStatus");

  temperature["value"] = temperatura;
  temperature["color"] = (temperatura < 26.0) ? "blue" : (temperatura <= 28.0) ? "green" : "red";
  controlStatus["color"] = (temperatura < 26.0) ? "red" : (temperatura > 28.0) ? "blue" : "white";

  if (temperatura < 26.0)
  {

    digitalWrite(this->aquecedor, LOW);
    digitalWrite(this->resfriador, HIGH);

    digitalWrite(this->indicadorAquecedor, HIGH);
    digitalWrite(this->indicadorResfriamento, LOW);

    digitalWrite(this->indicadorTemperaturaAlta, LOW);
    digitalWrite(this->indicadorTemperaturaCorreta, LOW);
    digitalWrite(this->indicadorTemperaturaBaixa, HIGH);

  }
  else if (temperatura >= 26.0 && temperatura <= 28.0)
  {

    digitalWrite(this->aquecedor, HIGH);
    digitalWrite(this->resfriador, HIGH);

    digitalWrite(this->indicadorAquecedor, LOW);
    digitalWrite(this->indicadorResfriamento, LOW);

    digitalWrite(this->indicadorTemperaturaAlta, LOW);
    digitalWrite(this->indicadorTemperaturaCorreta, HIGH);
    digitalWrite(this->indicadorTemperaturaBaixa, LOW);

  }
  else if (temperatura > 28.0)
  {

    digitalWrite(this->aquecedor, HIGH);
    digitalWrite(this->resfriador, LOW);

    digitalWrite(this->indicadorAquecedor, LOW);
    digitalWrite(this->indicadorResfriamento, HIGH);

    digitalWrite(this->indicadorTemperaturaAlta, HIGH);
    digitalWrite(this->indicadorTemperaturaCorreta, LOW);
    digitalWrite(this->indicadorTemperaturaBaixa, LOW);

  }

  return temperature;
}
