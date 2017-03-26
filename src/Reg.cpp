#include "Reg.h"

int
Reg::width() const {
    switch (type) {
        case TypeInfo::WIDTH8  : return 1;
        case TypeInfo::WIDTH16 : return 2;
        case TypeInfo::WIDTH32 : return 4;
    }
}

Reg::TypeInfo
Reg::getType() const {
    return type;
}

uint32_t
Reg::getData() const {
    switch (type) {
        case TypeInfo::WIDTH8  : return data.w8_ll;
        case TypeInfo::WIDTH16 : return data.w16_l;
        case TypeInfo::WIDTH32 : return data.w32;
    }
}

void
Reg::setType(TypeInfo type) {
    this->type = type;
}

void
Reg::setData(uint32_t data) {
    switch (type) {
        case TypeInfo::WIDTH8  : this->data.w8_ll = (uint8_t) data;
        case TypeInfo::WIDTH16 : this->data.w16_l = (uint16_t) data;
        case TypeInfo::WIDTH32 : this->data.w32 = data;
    }
}
