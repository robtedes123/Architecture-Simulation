#pragma once

#include "Reg.h"
#include "Mem.h"

#include <vector>
#include <initializer_list>

#include <cstdint>
#include <iostream>

using namespace std;

class CPU {
    public:
        CPU(const vector<uint32_t>& program);

        // Execute the program
        void run();

        // Step program one instruction
        void step();

        // True if the program has finished running
        bool isHalted = false;

        // Cycle Counter
        uint32_t cycles = 0;

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
            Reg reg[32];
        };

    private:
        void fetch();
        void decode();
        void execute();
        void memory();
        void writeback();

        const static auto REG_LR    = 28;
        const static auto REG_SP    = 29;
        const static auto REG_FLAGS = 30;
        const static auto REG_PC    = 31;

        // FLAGS
        enum COND {
            AL = 0,
            GT = 1 << 0,
            LT = 1 << 1,
            EQ = 1 << 2,
            NE = 1 << 3,
            OF = 1 << 4,
            UF = 1 << 5,
            CA = 1 << 6,
            DZ = 1 << 7,
        };

        struct Instruction {
            const vector<uint32_t> targets;
            const vector<uint32_t> operands;

            uint32_t mem_address = -1;

            vector<Reg> target_writeback;

            Instruction(initializer_list<uint32_t> targets, initializer_list<uint32_t> operands) :
                targets(vector<uint32_t>(targets)),
                operands(vector<uint32_t>(operands))
            {
            }

            virtual void execute(CPU& cpu)=0;

            virtual void memory_access(CPU& cpu) {}

            virtual void writeback(CPU& cpu) {
                for (int i = 0; i < targets.size(); i++) {
                    cpu.reg[targets[i]] = target_writeback[i];
                }
            }

            virtual bool isBranch() {
                return false;
            }

            bool isConflicting(Instruction* instruction) {
                if (instruction == nullptr)
                    return false;

                for (uint32_t target : instruction->targets) {
                    for (uint32_t operand : operands) {
                        if (operand == target) {
                            return true;
                        }
                    }
                }

                return false;
            }
        };

        // Fetch, Decode, Execute, Memory, Writeback
        const static auto EXECUTE   = 0;
        const static auto MEMORY    = 1;
        const static auto WRITEBACK = 2;

        bool fetch_stalled  = false;
        bool decode_stalled = false;

        Instruction* stalled_instruction = nullptr;

        uint32_t to_be_decoded = -1;
        vector<Instruction*> pipeline;

        struct Add : public Instruction {
            Add(uint32_t target, uint32_t arg1, uint32_t arg2) :
                Instruction({target, REG_FLAGS}, {arg1, arg2}) {}

            void execute(CPU& cpu) {
                cout << "EXECUTE ADD" << endl;
                Reg& op1 = cpu.reg[operands[0]];
                Reg& op2 = cpu.reg[operands[1]];

                Reg result;
                Reg flags;

                result.setType(Reg::chooseMaxWidth(op1,op2));
                flags.setType(Reg::Type::WIDTH32);

                uint32_t op1_data = op1.getData();
                uint32_t op2_data = op2.getData();

                switch(result.width()) {
                    case 1: {
                        uint8_t res;
                        if (__builtin_add_overflow((uint8_t)op1_data, (uint8_t)op2_data, &res))
                            flags.setData(OF);
                        result.setData(res);
                        break;
                    }
                    case 2: {
                        uint16_t res;
                        if(__builtin_add_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                            flags.setData(OF);
                        result.setData(res);
                        break;
                    }
                    case 4: {
                        uint32_t res;
                        if(__builtin_add_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                            flags.setData(OF);
                        result.setData(res);
                        break;
                    }
                };

                target_writeback = vector<Reg>({ result, flags });
            }
        };

        struct Addi : public Instruction {
            const uint32_t imm;

            Addi(uint32_t target, uint32_t arg, uint32_t imm) :
                Instruction({target, REG_FLAGS}, {arg}),
                imm(imm) {}

            void execute(CPU& cpu) {
                cout << "EXECUTE ADD" << endl;
                Reg& op1 = cpu.reg[operands[0]];

                Reg result;
                Reg flags;

                result.setType(Reg::chooseMaxWidth(op1,Reg::Type::WIDTH16));
                flags.setType(Reg::Type::WIDTH32);

                uint32_t op1_data = op1.getData();
                uint32_t op2_data = imm;

                switch(result.width()) {
                    case 1: {
                        uint8_t res;
                        if (__builtin_add_overflow((uint8_t)op1_data, (uint8_t)op2_data, &res))
                            flags.setData(OF);
                        result.setData(res);
                        break;
                    }
                    case 2: {
                        uint16_t res;
                        if(__builtin_add_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                            flags.setData(OF);
                        result.setData(res);
                        break;
                    }
                    case 4: {
                        uint32_t res;
                        if(__builtin_add_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                            flags.setData(OF);
                        result.setData(res);
                        break;
                    }
                }

                target_writeback = vector<Reg>({ result, flags });
            }
        };

        struct Sub : public Instruction {
            Sub(uint32_t target, uint32_t arg1, uint32_t arg2) :
                Instruction({target, REG_FLAGS}, {arg1, arg2}) {}

            void execute(CPU& cpu) {
                cout << "EXECUTE Sub" << endl;
                Reg& op1 = cpu.reg[operands[0]];
                Reg& op2 = cpu.reg[operands[1]];

                Reg result;
                Reg flags;

                result.setType(Reg::chooseMaxWidth(op1,op2));
                flags.setType(Reg::Type::WIDTH32);

                uint32_t op1_data = op1.getData();
                uint32_t op2_data = op2.getData();

                switch(result.width()) {
                    case 1: {
                        uint8_t res;
                        if (__builtin_sub_overflow((uint8_t)op1_data, (uint8_t)op2_data, &res))
                            flags.setData(UF);
                        result.setData(res);
                        break;
                    }
                    case 2: {
                        uint16_t res;
                        if(__builtin_sub_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                            flags.setData(UF);
                        result.setData(res);
                        break;
                    }
                    case 4: {
                        uint32_t res;
                        if(__builtin_sub_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                            flags.setData(UF);
                        result.setData(res);
                        break;
                    }
                }

                target_writeback = vector<Reg>({ result, flags });
            }
        };

        struct Subi : public Instruction {
            const uint32_t imm;

            Subi(uint32_t target, uint32_t arg, uint32_t imm) :
                Instruction({target, REG_FLAGS}, {arg}),
                imm(imm) {}

            void execute(CPU& cpu) {
                cout << "EXECUTE Subi" << endl;
                Reg& op1 = cpu.reg[operands[0]];

                Reg result;
                Reg flags;

                result.setType(Reg::chooseMaxWidth(op1,Reg::Type::WIDTH16));
                flags.setType(Reg::Type::WIDTH32);

                uint32_t op1_data = op1.getData();
                uint32_t op2_data = imm;

                switch(result.width()) {
                    case 1: {
                        uint8_t res;
                        if (__builtin_sub_overflow((uint8_t)op1_data, (uint8_t)op2_data, &res))
                            flags.setData(UF);
                        result.setData(res);
                        break;
                    }
                    case 2: {
                        uint16_t res;
                        if(__builtin_sub_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                            flags.setData(UF);
                        result.setData(res);
                        break;
                    }
                    case 4: {
                        uint32_t res;
                        if(__builtin_sub_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                            flags.setData(UF);
                        result.setData(res);
                        break;
                    }
                }

                target_writeback = vector<Reg>({ result, flags });
            }
        };

        struct Mul : public Instruction {
            Mul(uint32_t target, uint32_t arg1, uint32_t arg2) :
                Instruction({target, REG_FLAGS}, {arg1, arg2}) {}

            void execute(CPU& cpu) {
                cout << "EXECUTE Mul" << endl;
                Reg& op1 = cpu.reg[operands[0]];
                Reg& op2 = cpu.reg[operands[1]];

                Reg result;
                Reg flags;

                result.setType(Reg::chooseMaxWidth(op1,op2));
                flags.setType(Reg::Type::WIDTH32);

                uint32_t op1_data = op1.getData();
                uint32_t op2_data = op2.getData();

                switch(result.width()) {
                    case 1: {
                        uint8_t res;
                        if (__builtin_mul_overflow((uint8_t)op1_data, (uint8_t)op2_data, &res))
                            flags.setData(OF);
                        result.setData(res);
                        break;
                    }
                    case 2: {
                        uint16_t res;
                        if(__builtin_mul_overflow((uint16_t)op1_data, (uint16_t)op2_data, &res))
                            flags.setData(OF);
                        result.setData(res);
                        break;
                    }
                    case 4: {
                        uint32_t res;
                        if(__builtin_mul_overflow((uint32_t)op1_data, (uint32_t)op2_data, &res))
                            flags.setData(OF);
                        result.setData(res);
                        break;
                    }
                }

                target_writeback = vector<Reg>({ result, flags });
            }
        };

        struct Cmp : public Instruction {
            Cmp(uint32_t arg1, uint32_t arg2) :
                Instruction({REG_FLAGS}, {arg1, arg2}) {}

            void execute(CPU& cpu) {
                cout << "EXECUTE Cmp r" << operands[0] << " r" << operands[1] << endl;
                Reg& op1 = cpu.reg[operands[0]];
                Reg& op2 = cpu.reg[operands[1]];

                uint32_t a1_data = op1.getData();
                uint32_t a2_data = op2.getData();
                cout << "EXECUTE Cmp " << a1_data << " " << a2_data << endl;

                uint32_t f = 0;

                if (a1_data > a2_data)  {
                    cout << "GT" << endl;
                    f |= GT;
                }
                if (a1_data < a2_data)  {
                    cout << "LT" << endl;
                    f |= LT;
                }
                if (a1_data == a2_data) {
                    cout << "EQ" << endl;
                    f |= EQ;
                }
                if (a1_data != a2_data) {
                    cout << "NE" << endl;
                    f |= NE;
                }

                Reg flags;
                flags.setType(Reg::Type::WIDTH32);
                flags.setData(f);

                target_writeback = vector<Reg>({ flags });
            }
        };

        struct Cmpi : public Instruction {
            const uint32_t imm;

            Cmpi(uint32_t arg, uint32_t imm) :
                Instruction({REG_FLAGS}, {arg}),
                imm(imm) {}

            void execute(CPU& cpu) {
                cout << "EXECUTE Cmp" << endl;
                Reg& op1 = cpu.reg[operands[0]];

                uint32_t a1_data = op1.getData();
                uint32_t a2_data = imm;

                uint32_t f = 0;

                if (a1_data > a2_data)  {
                    cout << "GT" << endl;
                    f |= GT;
                }
                if (a1_data < a2_data)  {
                    cout << "LT" << endl;
                    f |= LT;
                }
                if (a1_data == a2_data) {
                    cout << "EQ" << endl;
                    f |= EQ;
                }
                if (a1_data != a2_data) {
                    cout << "NE" << endl;
                    f |= NE;
                }

                Reg flags;
                flags.setType(Reg::Type::WIDTH32);
                flags.setData(f);

                target_writeback = vector<Reg>({ flags });
            }
        };

        struct Movi : public Instruction {
            const uint32_t imm;

            Movi(uint32_t dst, uint32_t imm) :
                Instruction({dst}, {}),
                imm(imm) {}

            void execute(CPU& cpu) {
                cout << "EXECUTE Mov" << endl;
                Reg reg;
                reg.setType(Reg::Type::WIDTH32);
                reg.setData(imm);

                target_writeback = vector<Reg>({ reg });
            }
        };

        struct Ldr : public Instruction {
            Reg::Type type;

            Ldr(uint32_t dst, uint32_t src, Reg::Type type) :
                Instruction({dst}, {src}),
                type(type) {}

            void execute(CPU& cpu) {
                cout << "EXECUTE Ldr" << endl;
            }

            void memory_access(CPU& cpu) {
                cout << "Memory Ldr" << endl;
                Reg& src = cpu.reg[operands[0]];

                const uint32_t address = src.getData();
                uint32_t data = cpu.mem.read(address, &cpu.cycles);

                Reg dst;
                dst.setType(type);
                dst.setData(data);

                target_writeback = vector<Reg>({ dst });
            }
        };

        struct Str : public Instruction {
            Str(uint32_t dst, uint32_t src) :
                Instruction({}, {src, dst}) {}

            void execute(CPU& cpu) {
                cout << "EXECUTE Str" << endl;
            }

            void memory_access(CPU& cpu) {
                cout << "Memory Str" << endl;
                Reg& src = cpu.reg[operands[0]];
                Reg& dst = cpu.reg[operands[1]];

                const uint32_t address = dst.getData();
                const uint32_t data = src.getData();

                uint32_t curr = cpu.mem.read(address, &cpu.cycles);
                uint64_t mask = (1 << (8*src.width())) - 1;
                curr &= (uint32_t) mask;
                curr |= data;

                cpu.mem.write(address, data, &cpu.cycles);
            }
        };

        struct B : public Instruction {
            const uint32_t cond;
            const int32_t label;

            B(uint32_t cond, int32_t label) :
                Instruction({REG_PC}, {REG_PC, REG_FLAGS}),
                cond(cond),
                label(label) {}

            void execute(CPU& cpu) {
                cout << "EXECUTE B" << endl;
                Reg& cur = cpu.reg[operands[0]];
                Reg& op1 = cpu.reg[operands[1]];

                Reg result;
                result.setType(Reg::Type::WIDTH32);

                cout << "B cond=" << cond << " Flags = " << op1.getData() << endl;

                if ((op1.getData() & cond) == cond) {
                    result.setData(cur.getData() + label);
                } else {
                    result.setData(cur.getData());
                }

                target_writeback = vector<Reg>({ result });
            }

            void writeback(CPU& cpu) {
                Instruction::writeback(cpu);
                cout << "WRITEBACK b" <<  endl;
                cpu.fetch_stalled = false;
            }

            bool isBranch() {
                return true;
            }
        };

        struct Hlt : public Instruction {
            Hlt() : Instruction({}, {}) {}

            void execute(CPU& cpu) {}

            void writeback(CPU& cpu) {
                cpu.isHalted = true;
            }
        };

        // INSTRUCTION TYPES
        const static auto EXECUTION = 0;
        const static auto LOADSTORE = 1;
        const static auto BRANCH    = 2;
        const static auto SPECIAL   = 3;

        // EXECUTION INSTRUCTION TYPES
        const static auto OP_ADD  = 0b00000;
        const static auto OP_ADDI = 0b00001;
        const static auto OP_SUB  = 0b00010;
        const static auto OP_SUBI = 0b00011;
        const static auto OP_RSUB = 0b00100;
        const static auto OP_CMP  = 0b00101;
        const static auto OP_CMPI = 0b00110;
        const static auto OP_AND  = 0b00111;
        const static auto OP_ANDI = 0b01000;
        const static auto OP_OR   = 0b01001;
        const static auto OP_ORI  = 0b01010;
        const static auto OP_NOT  = 0b01011;
        const static auto OP_XOR  = 0b01100;
        const static auto OP_XORI = 0b01101;
        const static auto OP_LSR  = 0b01110;
        const static auto OP_LSRI = 0b01111;
        const static auto OP_LSL  = 0b10000;
        const static auto OP_LSLI = 0b10001;
        const static auto OP_ASR  = 0b10010;
        const static auto OP_ROR  = 0b10011;
        const static auto OP_ROL  = 0b10100;
        const static auto OP_MUL  = 0b10101;
        const static auto OP_UMUL = 0b10110;
        const static auto OP_DIV  = 0b10111;
        const static auto OP_UDIV = 0b11000;
        const static auto OP_MOD  = 0b11001;
        const static auto OP_UMOD = 0b11010;
        const static auto OP_MOV  = 0b11011;
        const static auto OP_MOVI = 0b11100;
        const static auto OP_TRN  = 0b11101;

        // LOADSTORE INSTRUCTION TYPES
        const static auto OP_LDR  = 0b0;
        const static auto OP_STR  = 0b1;

        // BRANCH INSTRUCTION TYPES
        const static auto OP_B    = 0b00;
        const static auto OP_BI   = 0b01;
        const static auto OP_BL   = 0b10;
        const static auto OP_CALL = 0b11;
};

