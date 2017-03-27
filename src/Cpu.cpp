#include "Cpu.h"

#include <cassert>

// TODO: check shifts for undefined behavior

//sign extend
static inline int32_t SE(uint32_t data, int sign) {
    int32_t new_data = (int32_t) data;
    new_data <<= 32 - sign;
    new_data >>= 32 - sign;
    return new_data;
}

// Extract bits
static inline uint32_t EB(uint32_t data, int hi, int lo) {
    assert(hi > lo);
    int offset = 32 - hi;
    return (data << offset) >> (offset + lo);
}

CPU::CPU(const vector<uint32_t>& program) {
    // zero out registers
    Reg defaultReg;
    defaultReg.setType(Reg::TypeInfo::WIDTH32);
    defaultReg.setData(0);
    reg.fill(defaultReg);

    // load program into memory
    mem.loadProgram(program);
}

int
CPU::exec() {
    int cycles = 0;

    while (true) {
        uint32_t address = PC.getData();

        uint32_t data = 0;

        for (int i = 0; i < sizeof(uint32_t); i++) {
            data |= mem.read(address+i, &cycles) << (8*i);
        }

        if (data == ~0) {
            puts("CPU HALTING");
            return cycles;
        }

        execInstruction(data);
        getchar();
    }
}

void
CPU::execInstruction(uint32_t instruction) {
    const int EXECUTION = 0;
    const int LOADSTORE = 1;
    const int BRANCH    = 2;

    const int type = EB(instruction, 32, 30);

    switch (type) {
        case EXECUTION : execExecutionInstruction(instruction); break;
        case LOADSTORE : execLoadStoreInstruction(instruction); break;
        case BRANCH    : execBranchInstruction(instruction); break;
    }
}

void
CPU::execExecutionInstruction(uint32_t instruction) {
    const auto OP_ADD  = 0b00000;
    const auto OP_ADDI = 0b00001;
    const auto OP_SUB  = 0b00010;
    const auto OP_SUBI = 0b00011;
    const auto OP_RSUB = 0b00100;
    const auto OP_CMP  = 0b00101;
    const auto OP_CMPI = 0b00110;
    const auto OP_AND  = 0b00111;
    const auto OP_ANDI = 0b01000;
    const auto OP_OR   = 0b01001;
    const auto OP_ORI  = 0b01010;
    const auto OP_NOT  = 0b01011;
    const auto OP_XOR  = 0b01100;
    const auto OP_XORI = 0b01101;
    const auto OP_LSR  = 0b01110;
    const auto OP_LSRI = 0b01111;
    const auto OP_LSL  = 0b10000;
    const auto OP_LSLI = 0b10001;
    const auto OP_ASR  = 0b10010;
    const auto OP_ROR  = 0b10011;
    const auto OP_ROL  = 0b10100;
    const auto OP_MUL  = 0b10101;
    const auto OP_UMUL = 0b10110;
    const auto OP_DIV  = 0b10111;
    const auto OP_UDIV = 0b11000;
    const auto OP_MOD  = 0b11001;
    const auto OP_UMOD = 0b11010;
    const auto OP_MOV  = 0b11011;
    const auto OP_MOVI = 0b11100;
    const auto OP_TRN  = 0b11101;

    const auto op = EB(instruction, 30, 25);

    switch (op) {
        case OP_ADD : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("ADD r%d r%d r%d\n", dst, op1, op2);
            ADD(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_ADDI : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t imm = EB(instruction, 15, 0);
            printf("ADD r%d r%d imm=%d\n", dst, op1, imm);
            ADD(reg[dst], reg[op1], imm);
            break;
        }
        case OP_SUB : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("SUB r%d r%d r%d\n", dst, op1, op2);
            SUB(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_SUBI : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t imm = EB(instruction, 15, 0);
            printf("SUB r%d r%d imm=%d\n", dst, op1, imm);
            SUB(reg[dst], reg[op1], imm);
            break;
        }
        case OP_RSUB : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t imm = EB(instruction, 15, 0);
            printf("RSUB r%d imm=%d r%d\n", dst, imm, op1);
            RSUB(reg[dst], imm, reg[op1]);
            break;
        }
        case OP_CMP : {
            const uint32_t op1 = EB(instruction, 25, 20);
            const uint32_t op2 = EB(instruction, 20, 15);
            printf("CMP r%d r%d\n", op1, op2);
            CMP(reg[op1], reg[op2]);
            break;
        }
        case OP_CMPI : {
            const uint32_t op1 = EB(instruction, 25, 20);
            const uint32_t imm = EB(instruction, 20, 0);
            printf("CMP r%d imm=%d\n", op1, imm);
            CMP(reg[op1], imm);
            break;
        }
        case OP_AND : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("AND r%d r%d r%d\n", dst, op1, op2);
            AND(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_ANDI : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t imm = EB(instruction, 15, 0);
            printf("AND r%d r%d imm=%d\n", dst, op1, imm);
            AND(reg[dst], reg[op1], imm);
            break;
        }
        case OP_OR : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("OR r%d r%d r%d\n", dst, op1, op2);
            OR(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_ORI : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t imm = EB(instruction, 15, 0);
            printf("OR r%d r%d imm=%d\n", dst, op1, imm);
            OR(reg[dst], reg[op1], imm);
            break;
        }
        case OP_NOT : {
            const uint32_t op1 = (instruction >> 20) & 0x1F;
            const uint32_t op2 = (instruction >> 15) & 0x1F;
            printf("NOT r%d r%d\n", op1, op2);
            NOT(reg[op1], reg[op2]);
            break;
        }
        case OP_XOR : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("XOR r%d r%d r%d\n", dst, op1, op2);
            XOR(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_XORI : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t imm = EB(instruction, 15, 0);
            printf("XOR r%d r%d imm=%d\n", dst, op1, imm);
            XOR(reg[dst], reg[op1], imm);
            break;
        }
        case OP_LSR : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("LSR r%d r%d r%d\n", dst, op1, op2);
            LSR(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_LSRI : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t imm = EB(instruction, 15, 0);
            printf("LSR r%d r%d imm=%d\n", dst, op1, imm);
            LSR(reg[dst], reg[op1], imm);
            break;
        }
        case OP_LSL : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("LSL r%d r%d r%d\n", dst, op1, op2);
            LSL(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_LSLI : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t imm = EB(instruction, 15, 0);
            printf("LSL r%d r%d imm=%d\n", dst, op1, imm);
            LSL(reg[dst], reg[op1], imm);
            break;
        }
        case OP_ASR : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("ASR r%d r%d r%d\n", dst, op1, op2);
            ASR(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_ROR : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("ROR r%d r%d r%d\n", dst, op1, op2);
            ROR(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_ROL : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("ROL r%d r%d r%d\n", dst, op1, op2);
            ROL(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_MUL : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("MUL r%d r%d r%d\n", dst, op1, op2);
            MUL(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_UMUL : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("UMUL r%d r%d r%d\n", dst, op1, op2);
            UMUL(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_DIV : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("DIV r%d r%d r%d\n", dst, op1, op2);
            DIV(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_UDIV : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("UDIV r%d r%d r%d\n", dst, op1, op2);
            UDIV(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_MOD : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("MOD r%d r%d r%d\n", dst, op1, op2);
            MOD(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_UMOD : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t op1 = EB(instruction, 20, 15);
            const uint32_t op2 = EB(instruction, 15, 10);
            printf("UMOD r%d r%d r%d\n", dst, op1, op2);
            UMOD(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case OP_MOV : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t src = EB(instruction, 20, 15);
            printf("MOV r%d r%d\n", dst, src);
            MOV(reg[dst], reg[src]);
            break;
        }
        case OP_MOVI : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t imm = EB(instruction, 20, 0);
            printf("MOV r%d imm=%d\n", dst, imm);
            MOV(reg[dst], imm);
            break;
        }
        case OP_TRN : {
            const uint32_t dst = EB(instruction, 25, 20);
            const uint32_t width = EB(instruction, 20, 18);

            printf("TRN r%d size=%d\n", dst, width);
            TRN(reg[dst], (Reg::TypeInfo)width);
            break;
        }
    }

    //Increment PC
    PC.setData(PC.getData() + sizeof(instruction));
}

void
CPU::execLoadStoreInstruction(uint32_t instruction) {
    const auto OP_LDR = 0b0;
    const auto OP_STR = 0b1;

    const auto op = EB(instruction, 30, 29);

    switch (op) {
        case OP_LDR: {
            const uint32_t width = EB(instruction, 29, 27);
            const uint32_t dst = EB(instruction, 27, 22);
            const uint32_t src = EB(instruction, 22, 17);
            printf("ldr size=%d r%d r%d\n", width, dst, src);
            LDR(reg[dst], reg[src], (Reg::TypeInfo)width);
            break;
        }
        case OP_STR: {
            const uint32_t dst = EB(instruction, 29, 24);
            const uint32_t src = EB(instruction, 24, 19);
            printf("str r%d r%d\n", src, dst);
            STR(reg[dst], reg[src]);
            break;
        }
    }

    // Increment PC
    PC.setData(PC.getData() + sizeof(instruction));
}

void
CPU::execBranchInstruction(uint32_t instruction) {
    const auto OP_B    = 0b00;
    const auto OP_BI   = 0b01;
    const auto OP_BL   = 0b10;
    const auto OP_CALL = 0b11;

    const auto op = EB(instruction, 30, 28);

    //TODO: check label sign extension
    switch (op) {
        case OP_B: {
            const uint32_t cond = EB(instruction, 28, 24);
            int32_t label = SE(EB(instruction, 24, 0), 24);
            printf("B cond=%d %d\n", cond, label);
            B(cond, label);
            break;
        }
        case OP_BI: {
            const uint32_t cond = EB(instruction, 28, 24);
            const uint32_t idx  = EB(instruction, 24, 19);
            printf("BI cond=%d r%d\n", cond, idx);
            BI(cond, reg[idx]);
            break;
        }
        case OP_BL: {
            int32_t label = SE(EB(instruction, 28, 0), 28);
            printf("BL %d\n", label);
            BL(label);
            break;
        }
        case OP_CALL: {
            const uint32_t idx = EB(instruction, 28, 23);
            printf("CALL r%d\n", idx);
            CALL(reg[idx]);
            break;
        }
    }

    // NOTE: DO NOT INCREMENT PC HERE
}

/******* EXECUTION INSTRUCTIONS *******/

void
CPU::ADD(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    int width = dst.width();

    uint32_t data;
    switch(width) {
        case 1: {
            uint8_t res;
            if (__builtin_add_overflow((uint8_t)op1_data, (uint8_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
        case 2: {
            uint16_t res;
            if(__builtin_add_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
        case 4: {
            uint32_t res;
            if(__builtin_add_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
    }

    dst.setData(data);
}

void
CPU::ADD(Reg& dst, Reg& op1, uint32_t imm) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, Reg::TypeInfo::WIDTH16);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = imm;

    int width = dst.width();

    uint32_t data;
    switch(width) {
        case 1: {
            uint8_t res;
            if (__builtin_add_overflow((uint8_t)op1_data, (uint8_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
        case 2: {
            uint16_t res;
            if(__builtin_add_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
        case 4: {
            uint32_t res;
            if(__builtin_add_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
    }

    dst.setData(data);
}

void
CPU::SUB(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    int width = dst.width();

    uint32_t data;
    switch(width) {
        case 1: {
            uint8_t res;
            if (__builtin_sub_overflow((uint8_t)op1_data, (uint8_t)op2_data, &res))
                FLAGS.setData(UF);
            data = res;
            break;
        }
        case 2: {
            uint16_t res;
            if(__builtin_sub_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                FLAGS.setData(UF);
            data = res;
            break;
        }
        case 4: {
            uint32_t res;
            if(__builtin_sub_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                FLAGS.setData(UF);
            data = res;
            break;
        }
    }

    dst.setData(data);
}

void
CPU::SUB(Reg& dst, Reg& op1, uint32_t imm) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, Reg::TypeInfo::WIDTH16);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = imm;

    int width = dst.width();

    uint32_t data;
    switch(width) {
        case 1: {
            uint8_t res;
            if (__builtin_sub_overflow((uint8_t)op1_data, (uint8_t)op2_data, &res))
                FLAGS.setData(UF);
            data = res;
            break;
        }
        case 2: {
            uint16_t res;
            if(__builtin_sub_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                FLAGS.setData(UF);
            data = res;
            break;
        }
        case 4: {
            uint32_t res;
            if(__builtin_sub_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                FLAGS.setData(UF);
            data = res;
            break;
        }
    }

    dst.setData(data);
}

void
CPU::RSUB(Reg& dst, uint32_t imm, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op2, Reg::TypeInfo::WIDTH16);
    dst.setType(dst_size);

    uint32_t op1_data = imm;
    uint32_t op2_data = op2.getData();

    int width = dst.width();

    uint32_t data;
    switch(width) {
        case 1: {
            uint8_t res;
            if (__builtin_sub_overflow((uint8_t)op1_data, (uint8_t)op2_data, &res))
                FLAGS.setData(UF);
            data = res;
            break;
        }
        case 2: {
            uint16_t res;
            if(__builtin_sub_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                FLAGS.setData(UF);
            data = res;
            break;
        }
        case 4: {
            uint32_t res;
            if(__builtin_sub_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                FLAGS.setData(UF);
            data = res;
            break;
        }
    }

    dst.setData(data);
}

void
CPU::CMP(Reg& arg1, Reg& arg2) {
    uint32_t a1_data = arg1.getData();
    uint32_t a2_data = arg2.getData();

    if (a1_data > a2_data) {
        FLAGS.setData(GT);
    } else if (a1_data < a2_data) {
        FLAGS.setData(LT);
    } else if (a1_data == a2_data) {
        FLAGS.setData(EQ);
    } else if (a1_data != a2_data) {
        FLAGS.setData(NE);
    }
}

void
CPU::CMP(Reg& arg1, uint32_t imm) {
    uint32_t a1_data = arg1.getData();
    uint32_t a2_data = imm;

    if (a1_data > a2_data) {
        FLAGS.setData(GT);
    } else if (a1_data < a2_data) {
        FLAGS.setData(LT);
    } else if (a1_data == a2_data) {
        FLAGS.setData(EQ);
    } else if (a1_data != a2_data) {
        FLAGS.setData(NE);
    }
}

void
CPU::AND(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    dst.setData(op1_data & op2_data);
}

void
CPU::AND(Reg& dst, Reg& op1, uint32_t imm) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, Reg::TypeInfo::WIDTH16);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = imm;

    dst.setData(op1_data & op2_data);
}

void
CPU::OR(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    dst.setData(op1_data | op2_data);
}

void
CPU::OR(Reg& dst, Reg& op1, uint32_t imm) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, Reg::TypeInfo::WIDTH16);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = imm;

    dst.setData(op1_data | op2_data);
}

void
CPU::NOT(Reg& dst, Reg& arg) {
    dst.setType(arg.getType());
    dst.setData(~arg.getData());
}

void
CPU::XOR(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    dst.setData(op1_data ^ op2_data);
}

void
CPU::XOR(Reg& dst, Reg& op1, uint32_t imm) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, Reg::TypeInfo::WIDTH16);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = imm;

    dst.setData(op1_data ^ op2_data);
}

void
CPU::LSR(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    dst.setData(op1_data >> op2_data);
}

void
CPU::LSR(Reg& dst, Reg& op1, uint32_t imm) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, Reg::TypeInfo::WIDTH16);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = imm;

    dst.setData(op1_data >> op2_data);
}

void
CPU::LSL(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    dst.setData(op1_data << op2_data);
}

void
CPU::LSL(Reg& dst, Reg& op1, uint32_t imm) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, Reg::TypeInfo::WIDTH16);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = imm;

    dst.setData(op1_data << op2_data);
}

void
CPU::ASR(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    int32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    dst.setData(op1_data >> op2_data);
}

void
CPU::ROR(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    int width = dst.width();

    uint32_t data;
    switch(width) {
        case 1: {
            uint8_t op1 = (uint8_t)op1_data;
            uint8_t op2 = (uint8_t)op2_data;
            data = (op1 >> op2 | op1 << (8 - op2));
            break;
        }
        case 2: {
            uint8_t op1 = (uint16_t)op1_data;
            uint8_t op2 = (uint16_t)op2_data;
            data = (op1 >> op2 | op1 << (16 - op2));
            break;
        }
        case 4: {
            uint8_t op1 = (uint32_t)op1_data;
            uint8_t op2 = (uint32_t)op2_data;
            data = (op1 >> op2 | op1 << (32 - op2));
            break;
        }
    }

    dst.setData(data);
}

void
CPU::ROL(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    int width = dst.width();

    uint32_t data;
    switch(width) {
        case 1: {
            uint8_t op1 = (uint8_t)op1_data;
            uint8_t op2 = (uint8_t)op2_data;
            data = (op1 << op2 | op1 >> (8 - op2));
            break;
        }
        case 2: {
            uint8_t op1 = (uint16_t)op1_data;
            uint8_t op2 = (uint16_t)op2_data;
            data = (op1 << op2 | op1 >> (16 - op2));
            break;
        }
        case 4: {
            uint8_t op1 = (uint32_t)op1_data;
            uint8_t op2 = (uint32_t)op2_data;
            data = (op1 << op2 | op1 >> (32 - op2));
            break;
        }
    }

    dst.setData(data);
}

void
CPU::MUL(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    int width = dst.width();

    uint32_t data;
    switch(width) {
        case 1: {
            int8_t res;
            if (__builtin_mul_overflow((int8_t)op1_data, (int8_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
        case 2: {
            int16_t res;
            if(__builtin_mul_overflow((int16_t)op1_data, (int16_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
        case 4: {
            int32_t res;
            if(__builtin_mul_overflow((int32_t)op1_data, (int32_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
    }

    dst.setData(data);
}

void
CPU::UMUL(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    int width = dst.width();

    uint32_t data;
    switch(width) {
        case 1: {
            uint8_t res;
            if (__builtin_mul_overflow((uint8_t)op1_data, (uint8_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
        case 2: {
            uint16_t res;
            if(__builtin_mul_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
        case 4: {
            uint32_t res;
            if(__builtin_mul_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                FLAGS.setData(OF);
            data = res;
            break;
        }
    }

    dst.setData(data);
}

void
CPU::DIV(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    if (op2_data == 0) {
        FLAGS.setData(DZ);
    } else {
        int width = dst.width();

        uint32_t data;
        switch(width) {
            case 1: {
                data = (int8_t)op1_data / (int8_t)op2_data;
                break;
            }
            case 2: {
                data = (int16_t)op1_data / (int16_t)op2_data;
                break;
            }
            case 4: {
                data = (int32_t)op1_data / (int32_t)op2_data;
                break;
            }
        }

        dst.setData(data);
    }
}

void
CPU::UDIV(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    if (op2_data == 0) {
        FLAGS.setData(DZ);
    } else {
        int width = dst.width();

        uint32_t data;
        switch(width) {
            case 1: {
                data = (uint8_t)op1_data / (uint8_t)op2_data;
                break;
            }
            case 2: {
                data = (uint16_t)op1_data / (uint16_t)op2_data;
                break;
            }
            case 4: {
                data = (uint32_t)op1_data / (uint32_t)op2_data;
                break;
            }
        }

        dst.setData(data);
    }
}

void
CPU::MOD(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    if (op2_data == 0) {
        FLAGS.setData(DZ);
    } else {
        int width = dst.width();

        uint32_t data;
        switch(width) {
            case 1: {
                data = (int8_t)op1_data % (int8_t)op2_data;
                break;
            }
            case 2: {
                data = (int16_t)op1_data % (int16_t)op2_data;
                break;
            }
            case 4: {
                data = (int32_t)op1_data % (int32_t)op2_data;
                break;
            }
        }

        dst.setData(data);
    }
}

void
CPU::UMOD(Reg& dst, Reg& op1, Reg& op2) {
    Reg::TypeInfo dst_size = Reg::chooseMaxWidth(op1, op2);
    dst.setType(dst_size);

    uint32_t op1_data = op1.getData();
    uint32_t op2_data = op2.getData();

    if (op2_data == 0) {
        FLAGS.setData(DZ);
    } else {
        int width = dst.width();

        uint32_t data;
        switch(width) {
            case 1: {
                data = (uint8_t)op1_data % (uint8_t)op2_data;
                break;
            }
            case 2: {
                data = (uint16_t)op1_data % (uint16_t)op2_data;
                break;
            }
            case 4: {
                data = (uint32_t)op1_data % (uint32_t)op2_data;
                break;
            }
        }

        dst.setData(data);
    }
}

void
CPU::MOV(Reg& dst, uint32_t imm) {
    dst.setType(Reg::TypeInfo::WIDTH32);
    dst.setData(imm);
}

void
CPU::MOV(Reg& dst, Reg& src) {
    dst.setType(src.getType());
    dst.setData(src.getData());
}

void
CPU::TRN(Reg& arg, Reg::TypeInfo type) {
    uint32_t data = arg.getData();
    arg.setType(type);
    arg.setData(data);
}

/******* LOAD/STORE INSTRUCTIONS *******/

void
CPU::LDR(Reg& dst, Reg& src, Reg::TypeInfo type) {
    assert(type != Reg::TypeInfo::UNUSED);

    dst.setType(type);

    const uint32_t address = src.getData();
    uint32_t data = 0;

    int delay = 0;
    for (int i = 0; i < dst.width(); i++) {
        data |= mem.read(address+i, &delay) << (8*i);
    }

    dst.setData(data);
}

void
CPU::STR(Reg& dst, Reg& src) {
    const uint32_t address = dst.getData();
    const uint32_t data = src.getData();

    int delay = 0;
    for (int i = 0; i < src.width(); i++) {
        mem.write(address+i, (data >> (8*i)) & 0xFF, &delay);
    }
}

/******* LOAD/STORE INSTRUCTIONS *******/

void
CPU::B(uint32_t cond, int32_t label) {
    if (cond == AL || FLAGS.getData() == cond) {
        PC.setData(PC.getData() + label);
    }
}

void
CPU::BI(uint32_t cond, Reg& arg) {
    if (cond == AL || FLAGS.getData() == cond) {
        PC.setData(arg.getData());
    }
}

void
CPU::BL(int32_t label) {
    uint32_t pc = PC.getData();

    LR.setType(Reg::TypeInfo::WIDTH32);
    LR.setData(pc + sizeof(uint32_t));

    PC.setData(pc + label);
}

void
CPU::CALL(Reg& arg) {
    uint32_t pc = PC.getData();

    LR.setType(Reg::TypeInfo::WIDTH32);
    LR.setData(pc + sizeof(uint32_t));

    PC.setData(arg.getData());
}
