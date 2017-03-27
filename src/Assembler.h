#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

enum type_code {
			ADD,
			SUB,
			RSUB,
			CMP,
			AND,
			OR,
			NOT,
			XOR,
			LSR,
			LSL,
			ASR,
			ROR,
			ROL,
			MUL,
			UMUL,
			DIV,
			UDIV,
			MOD,
			UMOD,
			MOV,
			TRN,
			LDR,
			STR,
			B,
			BI,
			BL,
			CALL
		};

static map<string, type_code> s_mapTypeValues;
void enumerate();

class Assembler 
{
	public:
		Assembler(const char* fname_in, const char* fname_out);
		void parse();
	private:
		static uint32_t parse_line(string line);

		const char* filename_in;
		const char* filename_out;
		ofstream file_out;
		ifstream file_in;
};

