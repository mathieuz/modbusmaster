#include <ModbusMaster.h>

//Instância do Serial
HardwareSerial hs(SERIAL_UART1);

//Instância do ModbusMaster.
ModbusMaster ms = ModbusMaster(hs, 10000);

//Estrutura de Requisição.
uint8_t deviceAddress    = 0x11; //Endereço do escravo.
uint8_t startAddressHigh = 0x00; //Byte do endereço inicial.
uint8_t startAddressLow  = 0x01; //Byte do endereço inicial.
uint8_t lengthHigh       = 0x00; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).
uint8_t lengthLow        = 0x02; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).

//Array com os databytes a serem escritos + tamanho do array.
const uint dataBytesLength = 2;
uint16_t arrDataBytes[dataBytesLength] = {0x000A, 0x0102};

void setup() {
    delay(3000);

    pinMode(PA0, OUTPUT); //Led Verde
    pinMode(PA1, OUTPUT); //Led Azul

    Serial.begin(115200);
}

void loop() {
    delay(2000);

    Serial.println("Aguardando resposta do escravo...\n");
    
    bool res = ms.writeMultipleRegisters(deviceAddress, startAddressHigh, startAddressLow, lengthHigh, lengthLow, arrDataBytes, dataBytesLength);
    Serial.println(res ? "true" : "false");
}