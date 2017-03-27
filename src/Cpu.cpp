#include "Cpu.h"

#include <cassert>

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
    }
}

void
CPU::execInstruction(uint32_t instruction) {
    const int EXECUTION = 0;
    const int LOADSTORE = 1;
    const int BRANCH    = 2;

    const int type = (instruction >> 30) & 0b11;

    switch (type) {
        case EXECUTION : execExecutionInstruction(instruction); break;
        case LOADSTORE : execLoadStoreInstruction(instruction); break;
        case BRANCH    : execBranchInstruction(instruction); break;
    }
}

void
CPU::execExecutionInstruction(uint32_t instruction) {
    const auto ADD  = 0b00000;
    const auto ADDI = 0b00001;
    const auto SUB  = 0b00010;
    const auto SUBI = 0b00011;
    const auto RSUB = 0b00100;
    const auto CMP  = 0b00101;
    const auto CMPI = 0b00110;
    const auto AND  = 0b00111;
    const auto ANDI = 0b01000;
    const auto OR   = 0b01001;
    const auto ORI  = 0b01010;
    const auto NOT  = 0b01011;
    const auto XOR  = 0b01100;
    const auto XORI = 0b01101;
    const auto LSR  = 0b01110;
    const auto LSRI = 0b01111;
    const auto LSL  = 0b10000;
    const auto LSLI = 0b10001;
    const auto ASR  = 0b10010;
    const auto ROR  = 0b10011;
    const auto ROL  = 0b10100;
    const auto MUL  = 0b10101;
    const auto UMUL = 0b10110;
    const auto DIV  = 0b10111;
    const auto UDIV = 0b11000;
    const auto MOD  = 0b11001;
    const auto UMOD = 0b11010;
    const auto MOV  = 0b11011;
    const auto MOVI = 0b11100;
    const auto TRN  = 0b11101;

    const auto op = (instruction >> 25) & 0x1F;

    switch (op) {
        case ADD : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("ADD r%d r%d r%d\n", dst, op1, op2);
            //add(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case ADDI : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t imm = instruction & 0x7FFF;
            printf("ADD r%d r%d imm=%d\n", dst, op1, imm);
            //add(reg[dst], reg[op1], imm);
            break;
        }
        case SUB  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("SUB r%d r%d r%d\n", dst, op1, op2);
            //sub(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case SUBI : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t imm = instruction & 0x7FFF;
            printf("SUB r%d r%d imm=%d\n", dst, op1, imm);
            //sub(reg[dst], reg[op1], imm);
            break;
        }
        case RSUB : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t imm = instruction & 0x7FFF;
            printf("RSUB r%d imm=%d r%d\n", dst, imm, op1);
            //rsub(reg[dst], reg[op1], imm);
            break;
        }
        case CMP  : {
            const uint32_t op1 = (instruction >> 20) & 0x1F;
            const uint32_t op2 = (instruction >> 15) & 0x1F;
            printf("CMP r%d r%d\n", op1, op2);
            //cmp(reg[op1], reg[op2]);
            break;
        }
        case CMPI : {
            const uint32_t op1 = (instruction >> 20) & 0x1F;
            const uint32_t imm = instruction & 0xFFFFF;
            printf("CMP r%d imm=%d\n", op1, imm);
            //cmp(reg[op1], imm);
            break;
        }
        case AND  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("AND r%d r%d r%d\n", dst, op1, op2);
            //and(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case ANDI : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t imm = instruction & 0x7FFF;
            printf("AND r%d r%d imm=%d\n", dst, op1, imm);
            //and(reg[dst], reg[op1], imm);
            break;
        }
        case OR   : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("OR r%d r%d r%d\n", dst, op1, op2);
            //or(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case ORI  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t imm = instruction & 0x7FFF;
            printf("OR r%d r%d imm=%d\n", dst, op1, imm);
            //or(reg[dst], reg[op1], imm);
            break;
        }
        case NOT  : {
            const uint32_t op1 = (instruction >> 20) & 0x1F;
            const uint32_t op2 = (instruction >> 15) & 0x1F;
            printf("NOT r%d r%d\n", op1, op2);
            //not(reg[op1], reg[op2]);
            break;
        }
        case XOR  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("XOR r%d r%d r%d\n", dst, op1, op2);
            //xor(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case XORI : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t imm = instruction & 0x7FFF;
            printf("XOR r%d r%d imm=%d\n", dst, op1, imm);
            //xor(reg[dst], reg[op1], imm);
            break;
        }
        case LSR  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("LSR r%d r%d r%d\n", dst, op1, op2);
            //lsr(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case LSRI : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t imm = instruction & 0x7FFF;
            printf("LSR r%d r%d imm=%d\n", dst, op1, imm);
            //lsr(reg[dst], reg[op1], imm);
            break;
        }
        case LSL  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("LSL r%d r%d r%d\n", dst, op1, op2);
            //lsl(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case LSLI : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t imm = instruction & 0x7FFF;
            printf("LSL r%d r%d imm=%d\n", dst, op1, imm);
            //lsl(reg[dst], reg[op1], imm);
            break;
        }
        case ASR  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("ASR r%d r%d r%d\n", dst, op1, op2);
            //asr(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case ROR  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("ROR r%d r%d r%d\n", dst, op1, op2);
            //ror(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case ROL  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("ROL r%d r%d r%d\n", dst, op1, op2);
            //rol(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case MUL  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("MUL r%d r%d r%d\n", dst, op1, op2);
            //mul(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case UMUL : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("UMUL r%d r%d r%d\n", dst, op1, op2);
            //umul(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case DIV  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("DIV r%d r%d r%d\n", dst, op1, op2);
            //div(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case UDIV : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("UDIV r%d r%d r%d\n", dst, op1, op2);
            //udiv(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case MOD  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("MOD r%d r%d r%d\n", dst, op1, op2);
            //mod(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case UMOD : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t op1 = (instruction >> 15) & 0x1F;
            const uint32_t op2 = (instruction >> 10) & 0x1F;
            printf("UMOD r%d r%d r%d\n", dst, op1, op2);
            //umod(reg[dst], reg[op1], reg[op2]);
            break;
        }
        case MOV  : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t src = (instruction >> 15) & 0x1F;
            printf("MOV r%d r%d\n", dst, src);
            //mov(reg[dst], reg[src]);
            break;
        }
        case MOVI : {
            const uint32_t dst = (instruction >> 20) & 0x1F;
            const uint32_t imm = instruction & 0x3FFFFF;
            printf("MOV r%d imm=%d\n", dst, imm);
            //mov(reg[dst], imm);
            break;
        }
        case TRN  : {
            const uint32_t dst   = (instruction >> 20) & 0x1F;
            const uint32_t width = (instruction >> 18) & 0b11;

            printf("TRN r%d size=%d\n", dst, width);
            //trn(reg[dst], (Reg::TypeInfo)width);
            break;
        }
    }

    //Increment PC
    PC.setData(PC.getData() + sizeof(instruction));
}

void
CPU::execLoadStoreInstruction(uint32_t instruction) {
    const auto LDR = 0b0;
    const auto STR = 0b1;

    const auto op = (instruction >> 29) & 0b1;

    switch (op) {
        case LDR: {
            const uint32_t width = (instruction >> 27) & 0b11;
            const uint32_t dst = (instruction >> 22) & 0b11111;
            const uint32_t src = (instruction >> 17) & 0b11111;
            printf("ldr size=%d r%d r%d\n", width, dst, src);
            //ldr(reg[dst], reg[src], (Reg::TypeInfo)width);
            break;
        }
        case STR: {
            const uint32_t dst = (instruction >> 24) & 0b11111;
            const uint32_t src = (instruction >> 19) & 0b11111;
            printf("str r%d r%d\n", src, dst);
            //str(reg[dst], reg[src]);
            break;
        }
    }

    // Increment PC
    PC.setData(PC.getData() + sizeof(instruction));
}

void
CPU::execBranchInstruction(uint32_t instruction) {
    const auto B    = 0b00;
    const auto BI   = 0b01;
    const auto BL   = 0b10;
    const auto CALL = 0b11;

    const auto op = (instruction >> 28) & 0b11;

    //TODO: check label sign extension
    switch (op) {
        case B: {
            const uint32_t cond = (instruction >> 24) & 0xF;
            const uint32_t label = instruction & 0xFFFFFF;
            printf("B cond=%d %d\n", cond, label);
            branch(cond, label);
            break;
        }
        case BI: {
            const uint32_t cond = (instruction >> 24) & 0xF;
            const uint32_t idx  = (instruction >> 19) & 0x1F;
            printf("BI cond=%d r%d\n", cond, idx);
            branch_indirect(cond, reg[idx]);
            break;
        }
        case BL: {
            const uint32_t label = instruction & 0xFFFFFFF;
            printf("BL %d\n", label);
            branch_and_link(label);
            break;
        }
        case CALL: {
            const uint32_t idx = (instruction >> 23) & 0x1F;
            printf("CALL r%d\n", idx);
            call(reg[idx]);
            break;
        }
    }

    // NOTE: DO NOT INCREMENT PC HERE
}

/******* EXECUTION INSTRUCTIONS *******/

void
CPU::add(Reg& dst, Reg& op1, Reg& op2) {
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
                // TODO: set overflow
                puts("OVERFLOW");
            data = res;
            break;
        }
        case 2: {
            uint16_t res;
            if(__builtin_add_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                // TODO: set overflow
                puts("OVERFLOW");
            data = res;
            break;
        }
        case 4: {
            uint32_t res;
            if(__builtin_add_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                // TODO: set overflow
                puts("OVERFLOW");
            data = res;
            break;
        }
    }

    dst.setData(data);
}

void
CPU::mov(Reg& dst, uint32_t imm) {
    dst.setType(Reg::TypeInfo::WIDTH32);
    dst.setData(imm);
}

void
CPU::mov(Reg& dst, Reg& src) {
    dst.setType(src.getType());
    dst.setData(src.getData());
}

void
CPU::trn(Reg& arg, Reg::TypeInfo type) {
    uint32_t data = arg.getData();
    arg.setType(type);
    arg.setData(data);
}

/******* LOAD/STORE INSTRUCTIONS *******/

void
CPU::ldr(Reg& dst, Reg& src, Reg::TypeInfo type) {
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
CPU::str(Reg& dst, Reg& src) {
    const uint32_t address = dst.getData();
    const uint32_t data = src.getData();

    int delay = 0;
    for (int i = 0; i < src.width(); i++) {
        mem.write(address+i, (data >> (8*i)) & 0xFF, &delay);
    }
}

/******* LOAD/STORE INSTRUCTIONS *******/

// THESE ARE UNTESTED!
void
CPU::branch(uint32_t cond, uint32_t label) {
    if (FLAGS.getData() & cond) {
        uint32_t data = PC.getData() + label;
        PC.setData(data);
    }
}

void
CPU::branch_indirect(uint32_t cond, Reg& arg) {
    if (FLAGS.getData() & cond) {
        PC.setData(arg.getData());
    }
}

void
CPU::branch_and_link(uint32_t label) {
    uint32_t pc = PC.getData();

    LR.setType(Reg::TypeInfo::WIDTH32);
    LR.setData(pc + 4);

    PC.setData(pc + label);
}

void
CPU::call(Reg& arg) {
    uint32_t pc = PC.getData();

    LR.setType(Reg::TypeInfo::WIDTH32);
    LR.setData(pc + 4);

    PC.setData(pc + arg.getData());
}
