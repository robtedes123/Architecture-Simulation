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

        int exec();
    private:
        void execInstruction(uint32_t instruction);
        void execExecutionInstruction(uint32_t instruction);
        void execLoadStoreInstruction(uint32_t instruction);
        void execBranchInstruction(uint32_t instruction);

        // EXECUTION
        void ADD(Reg& dst, Reg& op1, Reg& op2);
        void ADD(Reg& dst, Reg& op1, uint32_t imm);
        void SUB(Reg& dst, Reg& op1, Reg& op2);
        void SUB(Reg& dst, Reg& op1, uint32_t imm);
        void RSUB(Reg& dst, uint32_t imm, Reg& op2);
        void CMP(Reg& arg1, Reg& arg2);
        void CMP(Reg& arg1, uint32_t imm);
        void AND(Reg& dst, Reg& op1, Reg& op2);
        void AND(Reg& dst, Reg& op1, uint32_t imm);
        void OR(Reg& dst, Reg& op1, Reg& op2);
        void OR(Reg& dst, Reg& op1, uint32_t imm);
        void NOT(Reg& dst, Reg& arg);
        void XOR(Reg& dst, Reg& op1, Reg& op2);
        void XOR(Reg& dst, Reg& op1, uint32_t imm);
        void LSR(Reg& dst, Reg& op1, Reg& op2);
        void LSR(Reg& dst, Reg& op1, uint32_t imm);
        void LSL(Reg& dst, Reg& op1, Reg& op2);
        void LSL(Reg& dst, Reg& op1, uint32_t imm);
        void ASR(Reg& dst, Reg& op1, Reg& op2);
        void ROR(Reg& dst, Reg& op1, Reg& op2);
        void ROL(Reg& dst, Reg& op1, Reg& op2);
        void MUL(Reg& dst, Reg& op1, Reg& op2);
        void UMUL(Reg& dst, Reg& op1, Reg& op2);
        void DIV(Reg& dst, Reg& op1, Reg& op2);
        void UDIV(Reg& dst, Reg& op1, Reg& op2);
        void MOD(Reg& dst, Reg& op1, Reg& op2);
        void UMOD(Reg& dst, Reg& op1, Reg& op2);
        void MOV(Reg& dst, uint32_t imm);
        void MOV(Reg& dst, Reg& src);
        void TRN(Reg& arg, Reg::TypeInfo type);

        // LOAD/STORE
        void LDR(Reg& dst, Reg& src, Reg::TypeInfo type);
        void STR(Reg& dst, Reg& src);

        // BRANCH
        void B(uint32_t cond, uint32_t label);
        void BI(uint32_t cond, Reg& arg);
        void BL(uint32_t label);
        void CALL(Reg& arg);

        // FLAGS
        const static auto AL = 0;
        const static auto GT = 1;
        const static auto LT = 2;
        const static auto EQ = 3;
        const static auto NE = 4;
        const static auto OF = 5;
        const static auto UF = 6;
        const static auto CA = 7;
        const static auto DZ = 8;

        // Memory
        Memory mem;

        // Registers
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

