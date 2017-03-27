// Class for running Assembler

#include "Assembler.h"

int main(int argc, char* argv[])
{
	Assembler assembler(argv[1], argv[2]);
	assembler.parse();
}
