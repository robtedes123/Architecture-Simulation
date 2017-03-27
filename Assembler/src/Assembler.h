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
		Assembler(const char* fname_in, const char* fname_out, const char* fname_out_bin);
		void parse();
	private:
		static uint32_t parse_line(string line);
		static uint32_t parse_reg(uint32_t word, string arg);
		static uint32_t parse_imm(uint32_t word, string arg, int shift);
		static uint32_t parse_cond(uint32_t word, string);

		const char* filename_in;
		const char* filename_out;
		const char* filename_out_bin;
		ofstream file_out;
		ofstream file_out_bin;
		ifstream file_in;
};

