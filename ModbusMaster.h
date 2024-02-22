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

    uint16_t getNumDataBytes8Bits(uint8_t lengthHigh, uint8_t lengthLow) {
        uint16_t length = (lengthHigh << 8) + lengthLow;
        uint16_t numReceivedBytes;

        if (length <= 7) { 
            numReceivedBytes = 1;

        } else if (length % 8 == 0) {
            numReceivedBytes = (int)(length / 8);

        } else {
            numReceivedBytes = (int)(length / 8) + 1;

        }

        return numReceivedBytes;
    }

    uint16_t getNumDataBytes16Bits(uint8_t lengthHigh, uint8_t lengthLow) {
        uint16_t length = (lengthHigh << 8) + lengthLow;
        uint16_t numReceivedBytes;

        if (length <= 7) { 
            numReceivedBytes = length * 2;

        } else if (length % 16 == 0) {
            numReceivedBytes = (int)(length / 16) * 2;

        } else {
            numReceivedBytes = ((int)(length / 16) + 1) * 2;

        }

        return numReceivedBytes;
    }

public:
    ModbusMaster(/*HardwareSerial serial,*/ uint timeout) {
        //this->serial = &serial;
        this->timeout = timeout;
    }

    uint8_t* readCoils(uint8_t deviceAddress, uint8_t startAddressHigh, uint8_t startAddressLow, uint8_t lengthHigh, uint8_t lengthLow) {
        const uint8_t functionCode = 0x01;

        uint16_t crcReq = 0;
        uint8_t crcReqHigh = 0;
        uint8_t crcReqLow = 0;
        
        //Calculando CRC da requisição
        uint8_t arrReqBuffer[6] = {deviceAddress, functionCode, startAddressHigh, startAddressLow, lengthHigh, lengthLow};

        crcReq = this->calcCRC(arrReqBuffer, 6); //6 referente ao comprimento da estrutura de requisição.
        crcReqHigh = crcReq & 0x00FF;
        crcReqLow = (crcReq & 0xFF00) >> 8;

        //Enviando requisição + CRC
        Serial.write(deviceAddress);
        Serial.write(functionCode);
        Serial.write(startAddressHigh);
        Serial.write(startAddressLow);
        Serial.write(lengthHigh);
        Serial.write(lengthLow);
        Serial.write(crcReqHigh);
        Serial.write(crcReqLow);

        //Calculando quantos data bytes virá na resposta.
        uint16_t numReceivedBytes = this->getNumDataBytes8Bits(lengthHigh, lengthLow);

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

                //Recuperando o CRC da resposta:
                uint8_t crcResHigh = buffer[bufferLength - 1];      //Último byte do buffer de resposta.
                uint8_t crcResLow = buffer[bufferLength - 2];       //Penúltimo byte do buffer de resposta.
                uint16_t crcRes = (crcResHigh << 8) + crcResLow;    //O CRC enviado da resposta em 16 bits.

                //Recebe os itens do buffer de resposta.
                uint8_t arrResBuffer[bufferLength - 2];

                //Preenchendo arrResBuffer com os itens do buffer de resposta (tirando os dois últimos itens, que são bytes do crc da resposta).
                for (uint i = 0; i < sizeof(arrResBuffer); i++) {
                    arrResBuffer[i] = buffer[i];
                }

                //Calculando CRC da resposta.
                uint16_t crcResCalc = this->calcCRC(arrResBuffer, sizeof(arrResBuffer));

                uint8_t crcCalcLow = crcResCalc & 0x00FF;
                uint8_t crcCalcHigh = (crcResCalc & 0xFF00) >> 8;
                uint16_t crcCalc = (crcCalcHigh << 8) + crcCalcLow; //CRC calculado.

                /*
                Serial.println("CRC da Resposta: ");
                Serial.println(crcRes, HEX);

                Serial.println("CRC Calculado: ");
                Serial.println(crcCalc, HEX);
                */

                //Se o CRC calculado for igual ao CRC da resposta, não houve erros ou perda de informação dos dados recebidos.
                if (crcCalc == crcRes) {
                    uint8_t* arrDataByte = new uint8_t[numReceivedBytes];

                    uint countIndex = 0;
                    for (uint i = 3; i < sizeof(arrResBuffer); i++) {
                      arrDataByte[countIndex] = arrResBuffer[i];
                      countIndex++;

                    }

                    /*
                    Serial.println("Data Bytes: ");
                    for (uint i = 0; i < sizeof(arrDataByte); i++) {
                      Serial.println(arrDataByte[i], HEX);
                    }
                    */

                    return arrDataByte;
                }

                break;
            }
        }
    }

    uint16_t* readHoldingRegisters(uint8_t deviceAddress, uint8_t startAddressHigh, uint8_t startAddressLow, uint8_t lengthHigh, uint8_t lengthLow) {
        const uint8_t functionCode = 0x03;

        uint16_t crcReq = 0;
        uint8_t crcReqHigh = 0;
        uint8_t crcReqLow = 0;
        
        //Calculando CRC da requisição
        uint8_t arrReqBuffer[6] = {deviceAddress, functionCode, startAddressHigh, startAddressLow, lengthHigh, lengthLow};

        crcReq = this->calcCRC(arrReqBuffer, 6); //6 referente ao comprimento da estrutura de requisição.
        crcReqLow = crcReq & 0x00FF;
        crcReqHigh = (crcReq & 0xFF00) >> 8;

        //Enviando requisição + CRC
        Serial.write(deviceAddress);
        Serial.write(functionCode);
        Serial.write(startAddressHigh);
        Serial.write(startAddressLow);
        Serial.write(lengthHigh);
        Serial.write(lengthLow);
        Serial.write(crcReqLow);
        Serial.write(crcReqHigh);

        //Calculando quantos data bytes virá na resposta.
        uint16_t numReceivedBytes = this->getNumDataBytes16Bits(lengthHigh, lengthLow);

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

                //Recuperando o CRC da resposta:
                uint8_t crcResHigh = buffer[bufferLength - 1];      //Último byte do buffer de resposta.
                uint8_t crcResLow = buffer[bufferLength - 2];       //Penúltimo byte do buffer de resposta.
                uint16_t crcRes = (crcResHigh << 8) + crcResLow;    //O CRC enviado da resposta em 16 bits.

                //Recebe os itens do buffer de resposta.
                uint8_t arrResBuffer[bufferLength - 2];

                //Preenchendo arrResBuffer com os itens do buffer de resposta (tirando os dois últimos itens, que são bytes do crc da resposta).
                for (uint i = 0; i < sizeof(arrResBuffer); i++) {
                    arrResBuffer[i] = buffer[i];
                }

                //Calculando CRC da resposta.
                uint16_t crcResCalc = this->calcCRC(arrResBuffer, sizeof(arrResBuffer));

                uint8_t crcCalcLow = crcResCalc & 0x00FF;
                uint8_t crcCalcHigh = (crcResCalc & 0xFF00) >> 8;
                uint16_t crcCalc = (crcCalcHigh << 8) + crcCalcLow; //CRC calculado.

                /*
                Serial.println("CRC da Resposta: ");
                Serial.println(crcRes, HEX);

                Serial.println("CRC Calculado: ");
                Serial.println(crcCalc, HEX);
                */

                //Se o CRC calculado for igual ao CRC da resposta, não houve erros ou perda de informação dos dados recebidos.
                if (crcCalc == crcRes) {

                    //Os databytes virão inicialmente em um valor de 8 bits.
                    uint8_t arrDataByte[numReceivedBytes];

                    uint countIndex = 0;
                    for (uint i = 3; i < sizeof(arrResBuffer); i++) {
                      arrDataByte[countIndex] = arrResBuffer[i];
                      countIndex++;

                    }

                    //Convertendo os valores para um array de 16 bits (2 bytes).
                    uint arrDataByte16BitsLength = numReceivedBytes / 2;
                    uint16_t* arrDataByte16Bits = new uint16_t[arrDataByte16BitsLength];

                    countIndex = 0;
                    for (uint i = 0; i < numReceivedBytes; i += 2) {
                        arrDataByte16Bits[countIndex] = (arrDataByte[i] << 8) + arrDataByte[i + 1];
                        countIndex++;
                    }

                    //Retornando array de 16 bits.
                    return arrDataByte16Bits;

                }

                break;
            }
        }
    }

    uint16_t* readInputRegisters(uint8_t deviceAddress, uint8_t startAddressHigh, uint8_t startAddressLow, uint8_t lengthHigh, uint8_t lengthLow) {
        const uint8_t functionCode = 0x04;

        uint16_t crcReq = 0;
        uint8_t crcReqHigh = 0;
        uint8_t crcReqLow = 0;
        
        //Calculando CRC da requisição
        uint8_t arrReqBuffer[6] = {deviceAddress, functionCode, startAddressHigh, startAddressLow, lengthHigh, lengthLow};

        crcReq = this->calcCRC(arrReqBuffer, 6); //6 referente ao comprimento da estrutura de requisição.
        crcReqLow = crcReq & 0x00FF;
        crcReqHigh = (crcReq & 0xFF00) >> 8;

        //Enviando requisição + CRC
        Serial.write(deviceAddress);
        Serial.write(functionCode);
        Serial.write(startAddressHigh);
        Serial.write(startAddressLow);
        Serial.write(lengthHigh);
        Serial.write(lengthLow);
        Serial.write(crcReqLow);
        Serial.write(crcReqHigh);

        //Calculando quantos data bytes virá na resposta.
        uint16_t numReceivedBytes = this->getNumDataBytes16Bits(lengthHigh, lengthLow);

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

                //Recuperando o CRC da resposta:
                uint8_t crcResHigh = buffer[bufferLength - 1];      //Último byte do buffer de resposta.
                uint8_t crcResLow = buffer[bufferLength - 2];       //Penúltimo byte do buffer de resposta.
                uint16_t crcRes = (crcResHigh << 8) + crcResLow;    //O CRC enviado da resposta em 16 bits.

                //Recebe os itens do buffer de resposta.
                uint8_t arrResBuffer[bufferLength - 2];

                //Preenchendo arrResBuffer com os itens do buffer de resposta (tirando os dois últimos itens, que são bytes do crc da resposta).
                for (uint i = 0; i < sizeof(arrResBuffer); i++) {
                    arrResBuffer[i] = buffer[i];
                }

                //Calculando CRC da resposta.
                uint16_t crcResCalc = this->calcCRC(arrResBuffer, sizeof(arrResBuffer));

                uint8_t crcCalcLow = crcResCalc & 0x00FF;
                uint8_t crcCalcHigh = (crcResCalc & 0xFF00) >> 8;
                uint16_t crcCalc = (crcCalcHigh << 8) + crcCalcLow; //CRC calculado.

                /*
                Serial.println("CRC da Resposta: ");
                Serial.println(crcRes, HEX);

                Serial.println("CRC Calculado: ");
                Serial.println(crcCalc, HEX);
                */

                //Se o CRC calculado for igual ao CRC da resposta, não houve erros ou perda de informação dos dados recebidos.
                if (crcCalc == crcRes) {

                    //Os databytes virão inicialmente em um valor de 8 bits.
                    uint8_t arrDataByte[numReceivedBytes];

                    uint countIndex = 0;
                    for (uint i = 3; i < sizeof(arrResBuffer); i++) {
                      arrDataByte[countIndex] = arrResBuffer[i];
                      countIndex++;

                    }

                    //Convertendo os valores para um array de 16 bits (2 bytes).
                    uint arrDataByte16BitsLength = numReceivedBytes / 2;
                    uint16_t* arrDataByte16Bits = new uint16_t[arrDataByte16BitsLength];

                    countIndex = 0;
                    for (uint i = 0; i < numReceivedBytes; i += 2) {
                        arrDataByte16Bits[countIndex] = (arrDataByte[i] << 8) + arrDataByte[i + 1];
                        countIndex++;
                    }

                    //Retornando array de 16 bits.
                    return arrDataByte16Bits;

                }

                break;
            }
        }
    }

    bool writeMultipleRegisters(uint8_t deviceAddress, uint8_t startAddressHigh, uint8_t startAddressLow, uint8_t lengthHigh, uint8_t lengthLow, uint16_t dataBytes[], uint dataBytesLength) {
        const uint8_t functionCode = 0x10;

        //Convertendo os elementos da array de 16 bits para 8 bits. 
        uint lengthArrDataBytes8Bits = dataBytesLength * 2;
        uint8_t arrDataBytes8Bits[lengthArrDataBytes8Bits];

        //Avança os índices dos arrays nos laços de repetição.
        uint countIndex = 0;

        for (uint i = 0; i < lengthArrDataBytes8Bits; i++) {
            arrDataBytes8Bits[countIndex] = (dataBytes[i] & 0xFF00) >> 8;
            countIndex++;
            arrDataBytes8Bits[countIndex] = dataBytes[i] & 0x00FF;
            countIndex++;
        }

        //Determinando tamanho do byteCount
        uint8_t byteCount = lengthArrDataBytes8Bits;

        //Dimensionando o tamanho da array da requisição. Itens do array de databytes + 7 (deviceAddress, functionCode, startAddressHigh, startAddressLow, lengthHigh, lengthLow. byteCount)
        uint lengthArrReqBuffer = lengthArrDataBytes8Bits + 7;

        //Preenchendo o array com a requisição + os databytes a serem escritos.
        uint8_t arrReqBuffer[lengthArrReqBuffer] = {deviceAddress, functionCode, startAddressHigh, startAddressLow, lengthHigh, lengthLow, byteCount};

        countIndex = 0;
        for (uint8_t i = 7; i < lengthArrReqBuffer; i++) {
            arrReqBuffer[i] = arrDataBytes8Bits[countIndex];
            countIndex++;
        }

        //Calculando CRC do buffer de requisição:
        uint16_t crcReq = 0;
        uint8_t crcReqHigh = 0;
        uint8_t crcReqLow = 0;

        crcReq = this->calcCRC(arrReqBuffer, lengthArrReqBuffer);
        crcReqLow = crcReq & 0x00FF;
        crcReqHigh = (crcReq & 0xFF00) >> 8;

        //Enviando requisição, databytes a serem escritos e CRC.
        for (uint16_t i = 0; i < lengthArrReqBuffer; i++) {
            Serial.write(arrReqBuffer[i]);
        }

        Serial.write(crcReqLow);
        Serial.write(crcReqHigh);

        //Dimensionando tamanho do buffer de resposta. Quantidade de endereços para serem lidos (cada byte) + 4 (deviceAddress, functionCode, CRC High, CRC Low)
        uint16_t bufferLength = lengthArrDataBytes8Bits + 4;

        /*
        Serial.println(bufferLength);     //Tamanho do buffer de resposta.
        Serial.println("Aguardando resposta...\n");
        */

        while (true) {
            if (Serial.available() > 0) {
                uint8_t buffer[bufferLength];

                //Preenchendo o buffer com a resposta da requisição.
                Serial.readBytes(buffer, bufferLength);

                //Recuperando o CRC da resposta:
                uint8_t crcResHigh = buffer[bufferLength - 1];      //Último byte do buffer de resposta.
                uint8_t crcResLow = buffer[bufferLength - 2];       //Penúltimo byte do buffer de resposta.
                uint16_t crcRes = (crcResHigh << 8) + crcResLow;    //O CRC enviado da resposta em 16 bits.

                //Recebe os itens do buffer de resposta.
                uint8_t arrResBuffer[bufferLength - 2];

                //Preenchendo arrResBuffer com os itens do buffer de resposta (tirando os dois últimos itens, que são bytes do crc da resposta).
                for (uint i = 0; i < sizeof(arrResBuffer); i++) {
                    arrResBuffer[i] = buffer[i];
                }

                //Calculando CRC da resposta.
                uint16_t crcResCalc = this->calcCRC(arrResBuffer, sizeof(arrResBuffer));

                uint8_t crcCalcLow = crcResCalc & 0x00FF;
                uint8_t crcCalcHigh = (crcResCalc & 0xFF00) >> 8;
                uint16_t crcCalc = (crcCalcHigh << 8) + crcCalcLow; //CRC calculado.

                /*
                Serial.println("CRC da Resposta: ");
                Serial.println(crcRes, HEX);

                Serial.println("CRC Calculado: ");
                Serial.println(crcCalc, HEX);
                */

                if (crcCalc == crcRes) {
                    return true;

                } else {
                    return false;

                }

                break;
            }
        }
    }

    uint getLength8BitDataByte(uint8_t lengthHigh, uint8_t lengthLow) {
        return this->getNumDataBytes8Bits(lengthHigh, lengthLow);
    }

    uint getLength16BitDataByte(uint8_t lengthHigh, uint8_t lengthLow) {
        return this->getNumDataBytes16Bits(lengthHigh, lengthLow) / 2; 
    }
};

#endif