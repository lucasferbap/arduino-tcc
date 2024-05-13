#ifndef TemperatureController_h
#define TemperatureController_h

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include <ArduinoJson.h>

class TemperatureController
{
public:
  TemperatureController(
      uint8_t aquecedor,
      uint8_t resfriador,
      uint8_t sensorTemperatura,
      uint8_t indicadorResfriamento,
      uint8_t indicadorAquecedor,
      uint8_t indicadorTemperaturaBaixa,
      uint8_t indicadorTemperaturaCorreta,
      uint8_t indicadorTemperaturaAlta,
      uint8_t sensorManualTemperatura);

  void begin();
  DynamicJsonDocument update(bool changeSensor);

private:
  uint8_t aquecedor,
      resfriador,
      sensorTemperatura,
      indicadorResfriamento,
      indicadorAquecedor,
      indicadorTemperaturaBaixa,
      indicadorTemperaturaCorreta,
      indicadorTemperaturaAlta,
      sensorManualTemperatura;

  OneWire oneWire;
  DallasTemperature sensorObject;

  OneWire oneWireResfriador;
  DallasTemperature sensorObjectResfriamento;
};

#endif