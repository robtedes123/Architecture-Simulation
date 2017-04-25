#include "Cpu.h"
#include "Util.h"

#include <cassert>
#include <iostream>

CPU::CPU(const vector<uint32_t>& program) : pipeline(vector<Instruction*>(3, nullptr)) {
    // zero out registers
    for (int i = 0; i < 32; i++) {
        reg[i] = Reg::zeroed();
    }

    // load program into memory
    mem.loadProgram(program);
}

void
CPU::run() {
    uint32_t dummy = 0;
    cout << "mem = " << mem.read(516, &dummy) << " "
                     << mem.read(520, &dummy) << " "
                     << mem.read(524, &dummy) << " "
                     << mem.read(528, &dummy) << " "
                     << mem.read(532, &dummy) << " "
                     << mem.read(536, &dummy) << " "
                     << mem.read(540, &dummy) << " "
                     << mem.read(544, &dummy) << " "
                     << mem.read(548, &dummy) << " "
                     << mem.read(552, &dummy) << " "
                     << mem.read(556, &dummy) << " "
                     << mem.read(560, &dummy) << " "
                     << endl;

    while (!isHalted) {
        cout << "STEP" << endl;
        step();
    }
    cout << "mem = " << mem.read(516, &dummy) << " "
                     << mem.read(520, &dummy) << " "
                     << mem.read(524, &dummy) << " "
                     << mem.read(528, &dummy) << " "
                     << mem.read(532, &dummy) << " "
                     << mem.read(536, &dummy) << " "
                     << mem.read(540, &dummy) << " "
                     << mem.read(544, &dummy) << " "
                     << mem.read(548, &dummy) << " "
                     << mem.read(552, &dummy) << " "
                     << mem.read(556, &dummy) << " "
                     << mem.read(560, &dummy) << " "
                     << endl;
}

void
CPU::step() {
    writeback();
    memory();
    execute();
    decode();
    fetch();
}

void
CPU::fetch() {
    if (fetch_stalled || decode_stalled) {
        cout << "FETCH STALLED" << endl;
        return;
    }

    // Get address of instruction
    uint32_t address = PC.getData();

    cout << "FETCH: " << address << endl;

    // Increment PC
    PC.setData(address + sizeof(uint32_t));

    // Fetch instruction from memory
    to_be_decoded = mem.read(address, &cycles);
}

void
CPU::decode() {
    if (decode_stalled) {
        decode_stalled =
            stalled_instruction->isConflicting(pipeline[MEMORY]) ||
            stalled_instruction->isConflicting(pipeline[WRITEBACK]);

        if (!decode_stalled) {
            cout << "DECODE: stalled instruction" << endl;
            pipeline[EXECUTE] = stalled_instruction;
            if (stalled_instruction->isBranch()) {
                fetch_stalled = true;
            }
            stalled_instruction = nullptr;
        } else {
            cout << "DECODE STILL STALLED" << endl;
        }

        return;
    }

    if (to_be_decoded == -1)
        return;

    cout << "DECODE: ";

    const uint32_t instruction = to_be_decoded;

    const int type = EB(instruction, 32, 30);

    switch (type) {
        case EXECUTION : {
            const auto op = EB(instruction, 30, 25);

            switch (op) {
                case OP_ADD : {
                    const uint32_t dst  = EB(instruction, 25, 20);
                    const uint32_t op1  = EB(instruction, 20, 15);
                    const uint32_t op2  = EB(instruction, 15, 10);
                    cout << "add r" << dst << " r" << op1 << " r" << op2 << endl;
                    pipeline[EXECUTE] = new Add(dst, op1, op2);
                    break;
                }
                case OP_ADDI : {
                    const uint32_t dst  = EB(instruction, 25, 20);
                    const uint32_t op1  = EB(instruction, 20, 15);
                    const uint32_t op2  = EB(instruction, 15,  0);
                    cout << "add" << endl;
                    pipeline[EXECUTE] = new Addi(dst, op1, op2);
                    break;
                }
                case OP_SUB : {
                    const uint32_t dst  = EB(instruction, 25, 20);
                    const uint32_t op1  = EB(instruction, 20, 15);
                    const uint32_t op2  = EB(instruction, 15, 10);
                    cout << "sub" << endl;
                    pipeline[EXECUTE] = new Sub(dst, op1, op2);
                    break;
                }
                case OP_SUBI : {
                    const uint32_t dst  = EB(instruction, 25, 20);
                    const uint32_t op1  = EB(instruction, 20, 15);
                    const uint32_t op2  = EB(instruction, 15,  0);
                    cout << "sub" << endl;
                    pipeline[EXECUTE] = new Subi(dst, op1, op2);
                    break;
                }
                case OP_CMP : {
                    const uint32_t arg1 = EB(instruction, 25, 20);
                    const uint32_t arg2 = EB(instruction, 20, 15);
                    cout << "cmp" << endl;
                    pipeline[EXECUTE] = new Cmp(arg1, arg2);
                    break;
                }
                case OP_CMPI : {
                    const uint32_t arg = EB(instruction, 25, 20);
                    const uint32_t imm = SE(EB(instruction, 20, 0), 20);
                    cout << "cmp" << endl;
                    pipeline[EXECUTE] = new Cmpi(arg, imm);
                    break;
                }
                case OP_MUL : {
                    const uint32_t dst  = EB(instruction, 25, 20);
                    const uint32_t op1  = EB(instruction, 20, 15);
                    const uint32_t op2  = EB(instruction, 15, 10);
                    cout << "mul" << endl;
                    pipeline[EXECUTE] = new Mul(dst, op1, op2);
                    break;
                }
                case OP_MOVI : {
                    const uint32_t dst = EB(instruction, 25, 20);
                    const uint32_t imm = EB(instruction, 20, 0);
                    cout << "mov" << endl;
                    pipeline[EXECUTE] = new Movi(dst, imm);
                    break;
                }
                default: {
                    assert(false);
                }
            }

            break;
        }
        case LOADSTORE : {
            const auto op = EB(instruction, 30, 29);

            switch (op) {
                case OP_LDR: {
                    const Reg::Type type = (Reg::Type) EB(instruction, 29, 27);
                    const uint32_t dst  = EB(instruction, 27, 22);
                    const uint32_t src  = EB(instruction, 22, 17);
                    cout << "ldr" << endl;
                    pipeline[EXECUTE] = new Ldr(dst, src, type);
                    break;
                }
                case OP_STR: {
                    const uint32_t src  = EB(instruction, 29, 24);
                    const uint32_t dst  = EB(instruction, 24, 19);
                    cout << "str" << endl;
                    pipeline[EXECUTE] = new Str(dst, src);
                    break;
                }
            }

            break;
        }
        case BRANCH : {
            const auto op = EB(instruction, 30, 28);

            //TODO: check label sign extension
            switch (op) {
                case OP_B: {
                    const uint32_t cond = EB(instruction, 28, 20);
                    int32_t label = SE(EB(instruction, 24, 0), 20);
                    pipeline[EXECUTE] = new B(cond, label);
                    cout << "b" << endl;
                    break;
                }
                default: {
                    assert(false);
                }
            }

            break;
        }
        case SPECIAL: {
            cout << "hlt" << endl;
            pipeline[EXECUTE] = new Hlt();
            break;
        }
    };

    // Check if we should stall
    decode_stalled =
        pipeline[EXECUTE]->isConflicting(pipeline[MEMORY]) ||
        pipeline[EXECUTE]->isConflicting(pipeline[WRITEBACK]);

    if (decode_stalled) {
        cout << "DECODE STALL" << endl;
        to_be_decoded = -1;
        stalled_instruction = pipeline[EXECUTE];
        pipeline[EXECUTE] = nullptr;
    } else {
        if (pipeline[EXECUTE]->isBranch()) {
            fetch_stalled = true;
        }
    }
}

void
CPU::execute() {
    Instruction* instruction = pipeline[EXECUTE];

    if (instruction != nullptr) {
        instruction->execute(*this);
    }

    pipeline[EXECUTE] = nullptr;
    pipeline[MEMORY] = instruction;
}

void
CPU::memory() {
    Instruction* instruction = pipeline[MEMORY];

    if (instruction != nullptr) {
        cout << "MEMORY" << endl;
        instruction->memory_access(*this);
    }

    pipeline[MEMORY] = nullptr;
    pipeline[WRITEBACK] = instruction;
}

void
CPU::writeback() {
    Instruction* instruction = pipeline[WRITEBACK];

    if (instruction != nullptr) {
        cout << "WRITEBACK" << endl;
        instruction->writeback(*this);
        delete pipeline[WRITEBACK];
        pipeline[WRITEBACK] = nullptr;
    }
}

