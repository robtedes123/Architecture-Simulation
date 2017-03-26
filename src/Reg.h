#pragma once

#include <cstdint>

using namespace std;

struct Reg {
    public:
        enum class TypeInfo : uint32_t {
            WIDTH8  = 0,
            WIDTH16 = 1,
            WIDTH32 = 2,
        };

        int width() const;

        TypeInfo getType() const;
        uint32_t getData() const;

        void setType(TypeInfo type);
        void setData(uint32_t data);

    private:
        TypeInfo type;

        union {
            union {
                uint8_t w8_ll;
                uint8_t w8_lh;
                uint8_t w8_hl;
                uint8_t w8_hh;
            };
            union {
                uint16_t w16_l;
                uint16_t w16_h;
            };
            uint32_t w32;
        } data;
};

