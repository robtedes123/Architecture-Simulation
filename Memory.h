#ifndef _MEMORY_H
#define _MEMORY_H

// Class representing memory operations for Architecture Simulation

#include <cstdint>
#include <vector>

using namespace std;

class Memory
{
    public:
        Memory(const int delay) : DELAY(delay) {}

        virtual bool read(const int address, uint8_t* byte) const = 0;
        virtual void write(const int address, const uint8_t byte) = 0;

        // Number cycles required to process a request
        const int DELAY;
};

class Controller
{
    public:
        Controller(const vector<Memory*> layers);

        int read(const int address, uint8_t* byte) const;
        int write(const int address, uint8_t byte);

    private:
        const vector<Memory*> layers;
};

class Cache : public Memory
{
    public:
        Cache(const int delay);

        virtual bool read(const int address, uint8_t* byte) const;
        virtual void write(const int address, const uint8_t byte);

    private:
        struct Line {

        };
};

class RAM : public Memory
{
    public:
        RAM(const int size, const int delay);

        virtual bool read(const int address, uint8_t* byte) const;
        virtual void write(const int address, const uint8_t byte);

    private:
        // Internal representation of memory
        vector<uint8_t> memory;
};

#endif // _MEMORY_H
