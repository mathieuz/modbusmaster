#ifndef ModbusMaster_h
#define ModbusMaster_h

class ModbusMaster
{

private:
    uint timeout;
    HardwareSerial *serial;

    uint16_t calcCRC(uint8_t buffer[], uint8_t length) {
        uint16_t crc = 0xFFFF;

        for (uint pos = 0; pos < length; pos++) {

            crc ^= (uint16_t)buffer[pos];
            
            for (uint16_t i = 8; i != 0; i--) {

                if ((crc & 0x0001) != 0) {
                    crc >>= 1;
                    crc ^= 0xA001;

                } else {
                    crc >>= 1;

                }
            }
        }

        return crc;

    }

public:
    ModbusMaster(/*HardwareSerial serial,*/ uint timeout) {
        //this->serial = &serial;
        this->timeout = timeout;
    }

    void readCoilFunction01(uint8_t deviceAddress, uint8_t functionCode, uint8_t startAddressHigh, uint8_t startAddressLow, uint8_t lengthHigh, uint8_t lengthLow) {
        uint16_t crc = 0;
        uint8_t crcHigh = 0;
        uint8_t crcLow = 0;
        
        //Calculando CRC da requisição
        uint8_t arrBuffer[6] = {deviceAddress, functionCode, startAddressHigh, startAddressLow, lengthHigh, lengthLow};

        crc = this->calcCRC(arrBuffer, 6); //6 referente ao comprimento da estrutura de requisição.
        crcHigh = crc & 0x00FF;
        crcLow = (crc & 0xFF00) >> 8;

        //Enviando requisição + CRC
        Serial.write(deviceAddress);
        Serial.write(functionCode);
        Serial.write(startAddressHigh);
        Serial.write(startAddressLow);
        Serial.write(lengthHigh);
        Serial.write(lengthLow);
        Serial.write(crcHigh);
        Serial.write(crcLow);

        //Determinando a quantidade de data bytes a serem recebidos.
        uint16_t length = (lengthHigh << 8) + lengthLow;
        uint16_t numReceivedBytes = length <= 7 ? 1 : (int)(length / 8) + 1;

        //Tamanho do buffer/resposta do escravo. Número de data bytes + 5 (endereço do escravo, código função, byte count, crcHigh, crcLow)
        uint16_t bufferLength = numReceivedBytes + 5;

        Serial.println(numReceivedBytes); //Número de data bytes a serem recebidos na resposta.
        Serial.println(bufferLength);     //Tamanho do buffer de resposta.
        Serial.println("Aguardando resposta...\n");

        while (true) {
            if (Serial.available() > 0) {
                uint8_t buffer[bufferLength];

                //Preenchendo o buffer com a resposta da requisição.
                Serial.readBytes(buffer, bufferLength);

                //Exibindo todos os bytes da resposta:
                /*
                Serial.print("Endereço Device: ");
                Serial.println(buffer[0], HEX);

                Serial.print("Código Função: ");
                Serial.println(buffer[1], HEX);

                Serial.print("Byte Count: ");
                Serial.println(buffer[2], HEX);

                Serial.print("Byte Data: ");
                Serial.println(buffer[3], HEX);

                Serial.print("CRC: ");
                Serial.println(buffer[4], HEX);

                Serial.print("CRC: ");
                Serial.println(buffer[5], HEX);
                */

                for (int i = 0; i < bufferLength; i++) {
                    Serial.println(buffer[i]);
                }

                break;
            }
        }

    }

    /*
    uint8_t* testeRetornoArray(uint size) {
        uint8_t* arr = new uint8_t[size];

        for (uint i = 0; i < size; i++) {
            arr[i] = (0x00 << 4) + i;
        }

        return arr;
    }
    */

    /*
    void testeSerial() {
        Serial.println("Serial.");
    }
    */
    
};

#endif