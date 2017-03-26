#pragma once

#include "Reg.h"
#include "Mem.h"

#include <array>
#include <vector>

#include <cstdint>

using namespace std;

class CPU {
    public:
        CPU(const vector<uint32_t>& program);

        void execute();
    private:
        void mov(Reg& dst, uint32_t imm);
        void mov(Reg& dst, Reg& src);
        void str(Reg& src, Reg& dst);
        void ldr(Reg& dst, Reg& src, Reg::TypeInfo type);

        Memory mem;

        union {
            struct {
                Reg r0;
                Reg r1;
                Reg r2;
                Reg r3;
                Reg r4;
                Reg r5;
                Reg r6;
                Reg r7;
                Reg r8;
                Reg r9;
                Reg r10;
                Reg r11;
                Reg r12;
                Reg r13;
                Reg r14;
                Reg r15;
                Reg r16;
                Reg r17;
                Reg r18;
                Reg r19;
                Reg r20;
                Reg r21;
                Reg r22;
                Reg r23;
                Reg r24;
                Reg r25;
                Reg r26;
                Reg r27;
                Reg LR;
                Reg SP;
                Reg FLAGS;
                Reg PC;
            };
            array<Reg, 32> reg;
        };
};

