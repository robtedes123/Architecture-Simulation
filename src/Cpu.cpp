#include "Cpu.h"

CPU::CPU(const vector<uint32_t>& program) : mem(0x2000, 100) {
    // zero out registers
    Reg defaultReg;
    defaultReg.setType(Reg::TypeInfo::WIDTH32);
    defaultReg.setData(0);
    reg.fill(defaultReg);

    // load program into memory
    mem.loadProgram(0x0000, program);
}

void
CPU::execute() {
    mov(reg[0], 2575);
    mov(reg[1], 100);
    str(reg[0], reg[1]);
    ldr(reg[2], reg[1], Reg::TypeInfo::WIDTH16);
    printf("%d\n", r2.getData());
    mov(reg[1], 0);
    ldr(reg[2], reg[1], Reg::TypeInfo::WIDTH32);
    printf("%d\n", r2.getData());
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
CPU::str(Reg& src, Reg& dst) {
    uint32_t address = dst.getData();
    uint32_t data = src.getData();

    int delay = 0;
    for (int i = 0; i < src.width(); i++) {
        mem.write(address+i, (data >> (8*i)) & 0xFF, &delay);
    }
}

void
CPU::ldr(Reg& dst, Reg& src, Reg::TypeInfo type) {
    dst.setType(type);

    uint32_t address = src.getData();
    uint32_t data = 0;

    int delay = 0;
    for (int i = 0; i < dst.width(); i++) {
        data |= mem.read(address+i, &delay) << (8*i);
    }

    dst.setData(data);
}

