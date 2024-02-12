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

    int* testeRetornoArray(uint size/*, uint8_t deviceAddress, uint8_t registerAddressH, uint8_t registerAddressL, uint8_t lengthL, uint8_t lengthH*/) {
        int* arr = new int[size];

        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        arr[3] = 40;

        return arr;
    }
    
};

#endif