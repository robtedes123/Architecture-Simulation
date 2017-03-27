// Class for running simulation

#include "Cpu.h"

#include <cstdio>

int
main(int argc, char** argv) {
    // NOTE: This code writes program1 to test.bin
    //
    //const vector<uint32_t> program1 = {
    //    (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00000 << 20 | 16),
    //    (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00001 << 20 | 100),
    //    (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00010 << 20 | 0),
    //    (uint32_t)(0b00 << 30 | 0b00101 << 25 | 0b00000 << 20 | 0b00001 << 15),
    //    //(uint32_t)(0b10 << 30 | 0b00    << 28 | 0b111111111111111111111100),
    //    (uint32_t)(0b10 << 30 | 0b00    << 28 | 4),
    //    (uint32_t)(0b10 << 30 | 0b00    << 28 | 0),
    //};
    //FILE* wf;
    //wf = fopen("test.bin", "wb+");
    //fwrite(program1.data(), sizeof(uint32_t), program1.size(), wf);
    //fclose(wf);

    FILE* file = fopen("test.bin", "rb");

    // get file size in instruction words
    fseek(file, 0L, SEEK_END);
    int sz = ftell(file);
    fseek(file, 0L, SEEK_SET);

    int sz_in_words = sz / sizeof(uint32_t);

    // read that many words from the file
    vector<uint32_t> program2(sz_in_words);
    fread(program2.data(), sizeof(uint32_t), sz_in_words, file);

    fclose(file);

//    const vector<uint32_t> program2 = {
//        (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00000 << 20 | 16),
//        (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00001 << 20 | 100),
//        (uint32_t)(0b00 << 30 | 0b11100 << 25 | 0b00010 << 20 | 0),
//        (uint32_t)(0b00 << 30 | 0b00101 << 25 | 0b00000 << 20 | 0b00001 << 15),
//        (uint32_t)(0b10 << 30 | 0b00    << 28 | 0b111111111111111111111100),
//  //      (uint32_t)(0b10 << 30 | 0b01    << 28 | 0b0000 << 24 | 0b00010 << 19),
//        (uint32_t)(0b10 << 30 | 0b00    << 28 | 0b0010 << 24 | 4),
//        (uint32_t)(0b10 << 30 | 0b00    << 28 | 0b0000 << 24 | 0),
//    };

    CPU cpu(program2);
    int num_cycles = cpu.exec();

    printf("num cycles = %d\n", num_cycles);
}
