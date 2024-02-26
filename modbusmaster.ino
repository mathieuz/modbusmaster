#include "ModbusMaster.h"

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

void setup() {
    delay(3000);
    Serial.begin(115200);
}

void loop() {
    delay(1000);
}