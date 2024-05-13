#ifndef VolumeController_h
#define VolumeController_h

#include <ArduinoJson.h>
#include <Ultrasonic.h>

enum StatusTransferencia {
  NaoPermiteTransferir,
  PermiteTransferir
};

class VolumeController {
public:
  VolumeController(
    float altura,
    float largura,
    float comprimento,
    float desconto,
    uint8_t trigger,
    uint8_t echo,
    uint8_t indicadorNivelBaixo,
    uint8_t indicadorNivelAlto,
    uint8_t releBombaAgua
  );
  void begin();
  float arredondar (float numero); 
  float distancia ();
  float volume ();
  StatusTransferencia getStatusTransferencia();
  DynamicJsonDocument update(StatusTransferencia status, uint8_t releBombaAgua);
  DynamicJsonDocument update();

private:
  // dimensoes do recipiente em centimetros
  float altura, 
        largura, 
        comprimento, 
        desconto; // onde desconto e a diferenca do topo do sonar, ate a borda do recipiente
  
  // pinos do sonar e indicadores de volume
  uint8_t trigger,
          echo,
          indicadorNivelAlto,
          indicadorNivelBaixo,
          releBombaAgua;
  
  StatusTransferencia statusTransferencia = PermiteTransferir;
  
};

#endif