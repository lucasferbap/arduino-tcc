#ifndef AmoniaController_h
#define AmoniaController_h

#include <ArduinoJson.h>
#include "VolumeController.h"

typedef StatusTransferencia StatusTransferenciaVolume;

class AmoniaController {
public:
  AmoniaController(
    uint8_t sensorAmonia,
    uint8_t indicadorAmoniaBaixa,
    uint8_t indicadorAmoniaMedia,
    uint8_t indicadorAmoniaAlta
  );
  void begin();
  void update(
    JsonObject& amonia, 
    StatusTransferenciaVolume statusReservatorio, 
    StatusTransferenciaVolume statusTanque, 
    uint8_t releBombaAgua
  );

private:
  uint8_t sensorAmonia,
          indicadorAmoniaBaixa,
          indicadorAmoniaMedia,
          indicadorAmoniaAlta;
};

#endif