#include "ModbusMaster.h"

//Instância do ModbusMaster.
ModbusMaster ms = ModbusMaster(Serial0, 20000);

//Estrutura de Requisição.
uint8_t deviceAddress    = 0x02; //Endereço do escravo.
uint8_t functionCode     = 0x01; //Código da função.
uint8_t startAddressHigh = 0x00; //Byte do endereço inicial.
uint8_t startAddressLow  = 0x03; //Byte do endereço inicial.
uint8_t lengthHigh       = 0x00; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).
uint8_t lengthLow        = 0x04; //Byte da quantidade de endereços a serem lidos (a partir do endereço inicial).

void setup() {
    pinMode(PA0, OUTPUT); //Led Verde
    pinMode(PA1, OUTPUT); //Led Azul

    ms.setup(115200);

    delay(3000);
}

void loop() {
    delay(3000);

    ms.readCoilFunction01(0x01, 0x01, 0x00, 0x00, 0x00, 0x01);

    while (true) {
        if (Serial.available()) {

            //Array que recebe a resposta do escravo (?).
            uint8_t resposta[6];

            //Preenchendo array com as informações recebidas.
            Serial.readBytes(resposta, 6);

            //Verificando device address e functionCode.
            if (resposta[0] == 0x01 && resposta[1] == 0x01){
                //Verificando se o primeiro campo é igual a 1. Se sim, os leds verde e azul acendem, se não, apagam.
                if (resposta[3] == 0x01) {
                    digitalWrite(PA0, HIGH);
                    digitalWrite(PA1, HIGH);

                } else {
                    digitalWrite(PA0, LOW);
                    digitalWrite(PA1, LOW);

                }
            }

            break;

        }
    }
}