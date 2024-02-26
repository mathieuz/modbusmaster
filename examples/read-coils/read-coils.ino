#include <ModbusMaster.h>

/*
HardwareSerial DEFAULT_SERIAL_CONSOLE //Serial padrão.
HardwareSerial SERIAL_UART0
HardwareSerial SERIAL_UART1 .
HardwareSerial SERIAL_UART2
*/

//Instância do Serial
HardwareSerial hs(SERIAL_UART1);

//Instância do ModbusMaster.
ModbusMaster ms = ModbusMaster(hs, 10000);

//Estrutura de Requisição.
uint8_t deviceAddress    = 0x0B; //Endereço do escravo.
uint8_t startAddressHigh = 0x00; //Byte do endereço inicial.
uint8_t startAddressLow  = 0x1D; //Byte do endereço inicial.
uint8_t lengthHigh       = 0x00; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).
uint8_t lengthLow        = 0x1F; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).

void setup() {
    delay(3000);

    pinMode(PA0, OUTPUT); //Led Verde
    pinMode(PA1, OUTPUT); //Led Azul

    Serial.begin(115200);
}

void loop() {
    delay(2000);

    Serial.println("Aguardando resposta do escravo...\n");
    uint8_t* arr = ms.readCoils(deviceAddress, startAddressHigh, startAddressLow, lengthHigh, lengthLow);

    if (arr != 0) {
        Serial.println("Resposta do escravo:");

        for (uint i = 0; i < ms.getLength8BitDataByte(lengthHigh, lengthLow); i++) {
            Serial.print(arr[i], HEX);
            Serial.print(" ");
        }

        Serial.println("\n");

    } else {
        Serial.println("Não houve resposta do escravo.\n");
    }

    delete[] arr;

}