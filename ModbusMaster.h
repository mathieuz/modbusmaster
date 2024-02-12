#ifndef ModbusMaster_h
#define ModbusMaster_h

class ModbusMaster
{

private:
    uint timeout;
    //HardwareSerial *serial;

public:
    ModbusMaster(uint timeout) {
        this->timeout = timeout;
        //this->serial = &serial;
    }

    /*
    uint8_t* testeRetornoArray(uint size, uint8_t deviceAddress, uint8_t registerAddressH, uint8_t registerAddressL, uint8_t lengthL, uint8_t lengthH) {
        uint8_t* arr = new uint8_t[size];

        for (int i = 0; i < size; i++) {
            arr[i] = 0x01;
        }

        return arr;
    }
    */

    uint8_t* testeRetornoArray(uint size/*, uint8_t deviceAddress, uint8_t registerAddressH, uint8_t registerAddressL, uint8_t lengthL, uint8_t lengthH*/) {
        uint8_t* arr = new uint8_t[size];

        /*
        arr[0] = 0x01;
        arr[1] = 0x02;
        arr[2] = 0x03;
        arr[3] = 0x04;
        */

        for (uint i = 0; i < size; i++) {
            arr[i] = (0x00 << 4) + i;
        }

        return arr;
    }
    
};

#endif