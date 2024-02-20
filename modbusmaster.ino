#include "ModbusMaster.h"

//Instância do ModbusMaster.
ModbusMaster ms = ModbusMaster(20000);

//Estrutura de Requisição.
uint8_t deviceAddress    = 0x01; //Endereço do escravo.
uint8_t startAddressHigh = 0x00; //Byte do endereço inicial.
uint8_t startAddressLow  = 0x02; //Byte do endereço inicial.
uint8_t lengthHigh       = 0x00; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).
uint8_t lengthLow        = 0x02; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).

void setup() {
    delay(3000);

    pinMode(PA0, OUTPUT); //Led Verde
    pinMode(PA1, OUTPUT); //Led Azul

    Serial.begin(115200);
}

void loop() {
    delay(5000);

    //Requisição/pergunta do mestre.
    uint8_t* arr = ms.readHoldingRegistersFunction03(deviceAddress, startAddressHigh, startAddressLow, lengthHigh, lengthLow);

    uint16_t length = ms.getNumDataBytes16Bits(lengthHigh, lengthLow);

    Serial.println("Data Bytes:");
    for (uint i = 0; i < length; i++) {
        Serial.println(arr[i], HEX);
    }

    delete[] arr;
}