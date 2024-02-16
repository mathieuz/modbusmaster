#include "ModbusMaster.h"

//Instância do ModbusMaster.
ModbusMaster ms = ModbusMaster(20000);

//Estrutura de Requisição.
uint8_t deviceAddress    = 0x02; //Endereço do escravo.
uint8_t functionCode     = 0x01; //Código da função.
uint8_t startAddressHigh = 0x00; //Byte do endereço inicial.
uint8_t startAddressLow  = 0x20; //Byte do endereço inicial.
uint8_t lengthHigh       = 0x00; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).
uint8_t lengthLow        = 0x0C; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).

void setup() {
    delay(3000);

    pinMode(PA0, OUTPUT); //Led Verde
    pinMode(PA1, OUTPUT); //Led Azul

    Serial.begin(115200);
}

void loop() {
    delay(3000);

    //Requisição/pergunta do mestre.
    ms.readCoilFunction01(deviceAddress, functionCode, startAddressHigh, startAddressLow, lengthHigh, lengthLow);

}