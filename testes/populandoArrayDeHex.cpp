#include "ModbusMaster.h"

ModbusMaster ms = ModbusMaster(10000);

void setup() {
    Serial.begin(115200);
    delay(2000);
}

void loop() {

    /*Retornando o ponteiro para a array populada.*/
    delay(4000);
    uint8_t* res = ms.testeRetornoArray(4);

    for (int i = 0; i < sizeof(res); i++) {
        Serial.printf("%d\n", res[i]);
    }

    delete[] res;
}