#include "Reg.h"

#include <cstdio>
#include <cstdlib>

int
Reg::width() const {
    switch (type) {
        case TypeInfo::WIDTH8  : return 1;
        case TypeInfo::WIDTH16 : return 2;
        case TypeInfo::WIDTH32 : return 4;
        case TypeInfo::UNUSED  : {
            puts("BAD TYPE INFO");
            exit(-1);
        }
    }
}

Reg::TypeInfo
Reg::getType() const {
    return type;
}

uint32_t
Reg::getData() const {
    switch (type) {
        case TypeInfo::WIDTH8  : return w8_ll;
        case TypeInfo::WIDTH16 : return w16_l;
        case TypeInfo::WIDTH32 : return w32;
        case TypeInfo::UNUSED  : {
            puts("BAD TYPE INFO");
            exit(-1);
        }
    }
}

void
Reg::setType(TypeInfo type) {
    this->type = type;
}

void
Reg::setData(uint32_t data) {
    switch (type) {
        case TypeInfo::WIDTH8  : w8_ll =  (uint8_t) data; break;
        case TypeInfo::WIDTH16 : w16_l = (uint16_t) data; break;
        case TypeInfo::WIDTH32 : w32   = (uint32_t) data; break;
        case TypeInfo::UNUSED  : {
            puts("BAD TYPE INFO");
            exit(-1);
        }
    }
}
