#include "Cpu.h"

CPU::CPU(const vector<uint32_t>& program) {
    // zero out registers
    Reg defaultReg;
    defaultReg.setType(Reg::TypeInfo::WIDTH32);
    defaultReg.setData(0);
    reg.fill(defaultReg);

    // load program into memory
    mem.loadProgram(program);
}

void
CPU::execute() {
    mov(r0, ~0);
    mov(r1, 2);
    add(r2, r1, r0);
    printf("%u\n", r2.getData());

    //str(reg[0], reg[1]);
    //ldr(reg[2], reg[1], Reg::TypeInfo::WIDTH16);
    //printf("%d\n", r2.getData());
    //mov(reg[1], 0);
    //ldr(reg[2], reg[1], Reg::TypeInfo::WIDTH32);
    //printf("%d\n", r2.getData());
}

/******* EXECUTION *******/

void
CPU::add(Reg& dst, Reg& op1, Reg& op2) {
    int op1_size = (int)op1.getType();
    int op2_size = (int)op2.getType();
    int dst_size = std::max(op1_size, op2_size);

    dst.setType((Reg::TypeInfo)dst_size);

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

void
CPU::str(Reg& src, Reg& dst) {
    const uint32_t address = dst.getData();
    const uint32_t data = src.getData();

    int delay = 0;
    for (int i = 0; i < src.width(); i++) {
        mem.write(address+i, (data >> (8*i)) & 0xFF, &delay);
    }
}

void
CPU::ldr(Reg& dst, Reg& src, Reg::TypeInfo type) {
    dst.setType(type);

    const uint32_t address = src.getData();
    uint32_t data = 0;

    int delay = 0;
    for (int i = 0; i < dst.width(); i++) {
        data |= mem.read(address+i, &delay) << (8*i);
    }

    dst.setData(data);
}

