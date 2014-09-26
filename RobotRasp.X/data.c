
#include "data.h"

uint8_t data[256];

void data_init(void) {
    int i;

    for(i = 0; i < 256; i++) {
        data[i] = 0;
    }
}

uint8_t data_get(uint8_t addr) {
    return data[addr];
}

void data_set(uint8_t addr, uint8_t value){
    data[addr] = value;
}
