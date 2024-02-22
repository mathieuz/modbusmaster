#include "ModbusMaster.h"

//Instância do ModbusMaster.
ModbusMaster ms = ModbusMaster(20000);

//Estrutura de Requisição.
uint8_t deviceAddress    = 0x0F; //Endereço do escravo.
uint8_t startAddressHigh = 0x00; //Byte do endereço inicial.
uint8_t startAddressLow  = 0x64; //Byte do endereço inicial.
uint8_t lengthHigh       = 0x00; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).
uint8_t lengthLow        = 0x02; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).

const uint dataBytesLength = 2;
uint16_t arrDataBytes[dataBytesLength] = {0x000A, 0x0014};

void setup() {
    delay(3000);

    pinMode(PA0, OUTPUT); //Led Verde
    pinMode(PA1, OUTPUT); //Led Azul

    Serial.begin(115200);
}

void loop() {
    delay(5000);

    ms.writeMultipleRegisters(deviceAddress, startAddressHigh, startAddressLow, lengthHigh, lengthLow, arrDataBytes, dataBytesLength);
}