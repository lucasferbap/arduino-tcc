#ifndef SoilHumiditController_h
#define SoilHumiditController_h

#include <ArduinoJson.h>
#include "VolumeController.h"

typedef StatusTransferencia StatusTransferenciaVolume;

class SoilHumiditController {
public:
  SoilHumiditController(
    uint8_t sensorUmidadeSolo
  );
  void begin();
  void update(JsonObject& SoilHumidit, StatusTransferenciaVolume status, uint8_t releBombaAgua);

private:
  uint8_t sensorUmidadeSolo;
};

#endif