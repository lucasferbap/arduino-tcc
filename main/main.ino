#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <TemperatureController.h>
#include <VolumeController.h>
#include <AmoniaController.h>
#include <SoilHumiditController.h>

// ------------------------------------------ Tanque ------------------------------------------ 

// obs.: acrescentar na medida da altura: 0,5 cm
/* Dimensoes do tanque de peixes (valores ja com o acrescimo de 0.5 cm)
  Altura: 17.5 cm 
  Largura: 13.5 cm 
  Comprimento: 16 cm 
  Desconto: 7.0 cm 
*/
const float alturaTanque = 17.5;
const float larguraTanque = 13.5;
const float comprimentoTanque = 16.0;
const float descontoTanque = 7.0;
const uint8_t triggerTanque = 23;
const uint8_t echoTanque = 22;
const uint8_t indicadorNivelBaixoTanque = 26;
const uint8_t indicadorNivelAltoTanque = 25;
const uint8_t releBombaAguaTanque = 24;

VolumeController controleVolumeTanque(
  alturaTanque,
  larguraTanque,
  comprimentoTanque,
  descontoTanque,
  triggerTanque,
  echoTanque,
  indicadorNivelBaixoTanque,
  indicadorNivelAltoTanque, 
  releBombaAguaTanque
);

const uint8_t aquecedorTanque = 33;
const uint8_t resfriadorTanque = 34;
const uint8_t sensorTemperaturaTanque = 36;
const uint8_t indicadorResfriamentoTanque = 32;
const uint8_t indicadorAquecedorTanque = 35;
const uint8_t indicadorTemperaturaBaixaTanque = 37;
const uint8_t indicadorTemperaturaCorretaTanque = 38;
const uint8_t indicadorTemperaturaAltaTanque = 39;
const uint8_t sensorManualTemperaturaTanque = A2;
bool changeSensorTanque = false;

TemperatureController controleTemperaturaTanque(
  aquecedorTanque,
  resfriadorTanque,
  sensorTemperaturaTanque,
  indicadorResfriamentoTanque,
  indicadorAquecedorTanque,
  indicadorTemperaturaBaixaTanque,
  indicadorTemperaturaCorretaTanque,
  indicadorTemperaturaAltaTanque,
  sensorManualTemperaturaTanque
);

const uint8_t sensorAmoniaTanque = A0;
const uint8_t indicadorAmoniaBaixaTanque = 9;
const uint8_t indicadorAmoniaMediaTanque = 10;
const uint8_t indicadorAmoniaAltaTanque = 11;

AmoniaController controleAmoniaTanque(
  sensorAmoniaTanque,
  indicadorAmoniaBaixaTanque,
  indicadorAmoniaMediaTanque,
  indicadorAmoniaAltaTanque
);



// ------------------------------------------ Reservatorio ------------------------------------------ 

// obs.: acrescentar na medida da altura: 0,5 cm
/* Dimensoes do tanque de peixes (valores ja com o acrescimo de 0.5 cm)
  Altura: 17.5 cm 
  Largura: 13.5 cm 
  Comprimento: 16 cm 
  Desconto: 7.0 cm 
*/
const float alturaReservatorio = 17.5;
const float larguraReservatorio = 13.5;
const float comprimentoReservatorio = 16.0;
const float descontoReservatorio = 7.0;
const uint8_t triggerReservatorio = 3;
const uint8_t echoReservatorio = 2;
const uint8_t indicadorNivelBaixoReservatorio = 50;
const uint8_t indicadorNivelAltoReservatorio = 53;
const uint8_t releBombaAguaReservatorio = 52;

VolumeController controleVolumeReservatorio(
  alturaReservatorio,
  larguraReservatorio,
  comprimentoReservatorio,
  descontoReservatorio,
  triggerReservatorio,
  echoReservatorio,
  indicadorNivelBaixoReservatorio,
  indicadorNivelAltoReservatorio, 
  releBombaAguaReservatorio
);

const uint8_t aquecedorReservatorio = 51;
const uint8_t resfriadorReservatorio = 48;
const uint8_t sensorTemperaturaReservatorio = 46;
const uint8_t indicadorResfriamentoReservatorio = 49;
const uint8_t indicadorAquecedorReservatorio = 47;
const uint8_t indicadorTemperaturaBaixaReservatorio = 45;
const uint8_t indicadorTemperaturaCorretaReservatorio = 44;
const uint8_t indicadorTemperaturaAltaReservatorio = 43;
const uint8_t sensorManualTemperaturaReservatorio = A3;
bool changeSensorReservatorio = false;


TemperatureController controleTemperaturaReservatorio(
  aquecedorReservatorio,
  resfriadorReservatorio,
  sensorTemperaturaReservatorio,
  indicadorResfriamentoReservatorio,
  indicadorAquecedorReservatorio,
  indicadorTemperaturaBaixaReservatorio,
  indicadorTemperaturaCorretaReservatorio,
  indicadorTemperaturaAltaReservatorio,
  sensorManualTemperaturaReservatorio
);

const uint8_t sensorUmidadeSolo = A1;
SoilHumiditController controleUmidadeSolo(sensorUmidadeSolo);

// Esp2866
SoftwareSerial ESP2866(12,13); // rx, tx

void setup() {
  
  ESP2866.begin(115200);
  Serial.begin(115200);

  // TANQUE
  controleVolumeTanque.begin();
  controleTemperaturaTanque.begin();
  controleAmoniaTanque.begin();

  // RESERVATORIO
  controleVolumeReservatorio.begin();
  controleTemperaturaReservatorio.begin();
  controleUmidadeSolo.begin();
}

void loop() {
  if (Serial.available()) {
    String receivedString = Serial.readStringUntil('\n');
    if (receivedString.equals("manual-tank:true\r")) {
      changeSensorTanque = true;
    } else if (receivedString.equals("manual-tank:false\r")) {
      changeSensorTanque = false;
    } else if (receivedString.equals("manual-reservoir:true\r")) {
      changeSensorReservatorio = true;
    } else if (receivedString.equals("manual-reservoir:false\r")) {
      changeSensorReservatorio = false;
    }
  }

  DynamicJsonDocument data(1024);

  // RESERVATORIO
  JsonObject reservoir = data.createNestedObject("reservoir");
  JsonObject soilHumidity = data.createNestedObject("soilHumidity");
  
  reservoir["reservoirVolume"] = controleVolumeReservatorio.update();
  StatusTransferenciaVolume  statusTransfereciaReservatorio = controleVolumeReservatorio.getStatusTransferencia();
  
  reservoir["reservoirTemperature"] = controleTemperaturaReservatorio.update(changeSensorReservatorio);
  
  // TANQUE
  JsonObject tank = data.createNestedObject("tank");
  JsonObject amonia = data.createNestedObject("amonia");
  
  tank["tankTemperature"] = controleTemperaturaTanque.update(changeSensorTanque);
  
  tank["tankVolume"] = controleVolumeTanque.update(statusTransfereciaReservatorio, releBombaAguaReservatorio); //Conflito
  StatusTransferenciaVolume  statusTransfereciaTanque = controleVolumeTanque.getStatusTransferencia(); 
  
  controleAmoniaTanque.update(amonia, statusTransfereciaReservatorio, statusTransfereciaTanque, releBombaAguaReservatorio); // Conflito 

  controleUmidadeSolo.update(soilHumidity, statusTransfereciaTanque, releBombaAguaTanque);

  String jsonString;
  serializeJson(data, jsonString);
  ESP2866.println(jsonString);
  Serial.println(jsonString);

  delay(1000);
}