#include "Assembler.h"

Assembler::Assembler(const char* fname_in, const char* fname_out, const char* fname_out_bin)
{
	filename_in = fname_in;
	filename_out = fname_out;
	filename_out_bin = fname_out_bin;
}

void Assembler::parse()
{
	string temp;
	uint32_t bit_string;
	file_in.open(filename_in);
	file_out.open(filename_out);
	file_out_bin.open(filename_out_bin, ofstream::binary);

	if(file_in.is_open())
	{
		while(getline(file_in, temp))
		{
			bit_string = parse_line(temp);
			if(file_out.is_open())
			{
				file_out << bitset<32>(bit_string) << endl;
			}
			else
			{
				cout << "Unable to open output file" << endl;
			}
			if(file_out_bin.is_open())
			{
			
			 	file_out_bin.write((char*)&bit_string, sizeof(bit_string));
		
			}
			else
			{
				cout << "Unable to open binary output file" << endl;
			}
		}
	}
	else
	{
	cout << "Unable to open input file" << endl;
	}

	file_in.close();
	file_out.close();
	file_out_bin.close();

}

uint32_t Assembler::parse_line(string line)
{
	string arg[4];
	size_t start = 0;
	size_t end = 0;
	uint32_t word = 0;

	int i = 0;
	while(start != string::npos){
		end = line.find(" ", start);
		arg[i] = line.substr(start, end - start);
		start = line.find_first_not_of(" ", end);
		i++;
	}

	enumerate();

	switch(s_mapTypeValues[arg[0]])
	{
		//TO:DO shift the type and opcode values and add padding
		//ADD
		case 0:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_reg(word,arg[3]);
				//add padding
			}
			else
			{
				word += 0b1;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_imm(word,arg[3], 15);
				//add padding
			}
			break;
		//SUB
		case 1:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word += 0b10;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_reg(word,arg[3]);
				//add padding
			}
			else
			{
				word += 0b11;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_imm(word,arg[3], 15);
				//add padding
			}
			break;
		//RSUB
		case 2:
			word += 0b100;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[3]);
			word = parse_imm(word,arg[2], 15);
			//add padding
			break;
		//CMP
		case 3:
			if((arg[2].at(0) == 'R' || arg[1].at(0) == 'r'))
			{
				word += 0b101;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				//add padding
			}
			else
			{
				word += 0b110;
				word = parse_reg(word,arg[1]);
				word = parse_imm(word,arg[2], 20);
				//add padding
			}
			break;
		//AND
		case 4:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word += 0b111;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_reg(word,arg[3]);
				//add padding
			}
			else
			{
				word += 0b1000;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_imm(word,arg[3], 20);
				//add padding
			}
			break;
		//OR
		case 5:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word += 0b1001;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_reg(word,arg[3]);
				//add padding
			}
			else
			{
				word += 0b1010;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_imm(word,arg[3], 15);
				//add padding
			}
			break;
		//NOT
		case 6:
			word += 0b1011;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[2]);
			//add padding
			break;
		//XOR
		case 7:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word += 0b1100;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_reg(word,arg[3]);
				//add padding
			}
			else
			{
				word += 0b1101;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_imm(word,arg[3], 15);
				//add padding
			}
			break;
		//LSR
		case 8:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word += 0b1110;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_reg(word,arg[3]);
				//add padding
			}
			else
			{
				word += 0b1111;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_imm(word,arg[3], 8);
				//add padding
			}
			break;
		//LSL
		case 9:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word += 0b10000;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_reg(word,arg[3]);
				//add padding
			}
			else
			{
				word += 0b10001;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_imm(word,arg[3], 8);
				//add padding
			}
			break;
		//ASR
		case 10:
			word += 0b10010;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[2]);
			word = parse_reg(word,arg[3]);
			//add padding
			break;
		//ROR
		case 11:
			word += 0b10011;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[2]);
			word = parse_reg(word,arg[3]);
			//add padding
			break;
		//ROL
		case 12:
			word += 0b10100;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[2]);
			word = parse_reg(word,arg[3]);
			//add padding
			break;
		//MUL
		case 13:
			word += 0b10101;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[2]);
			word = parse_reg(word,arg[3]);
			//add padding
			break;
		//UMUL
		case 14:
			word += 0b10110;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[2]);
			word = parse_reg(word,arg[3]);
			//add padding
			break;
		//DIV
		case 15:
			word += 0b10111;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[2]);
			word = parse_reg(word,arg[3]);
			//add padding
			break;
		//UDIV
		case 16:
			word += 0b11000;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[2]);
			word = parse_reg(word,arg[3]);
			//add padding
			break;
		//MOD
		case 17:
			word += 0b11001;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[2]);
			word = parse_reg(word,arg[3]);
			//add padding
			break;
		//UMOD
		case 18:
			word += 0b11010;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[2]);
			word = parse_reg(word,arg[3]);
			//add padding
			break;
		//MOV
		case 19:
			if((arg[2].at(0) == 'R' || arg[1].at(0) == 'r'))
			{
				word += 0b11011;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				//add padding
			}
			else
			{
				word += 0b11100;
				word = parse_reg(word,arg[1]);
				word = parse_imm(word,arg[2], 22);
				//add padding
			}
			break;
		//TRN
		case 20:
			word += 0b11101;
			word = parse_reg(word,arg[1]);
			//parse size
			//add padding
			break;
		//LDR
		case 21:
			word += 0b1;
			word = word << 1;

			if(stoi(arg[1]) == 16)
			{
				word += 0b1;
			}
			else if(stoi(arg[1]) == 32)
			{
				word += 0b10;
			}

			word = word << 2;
			word = parse_reg(word,arg[2]);
			word = parse_reg(word,arg[3]);
			word = word << 17;
			break;
		//STR
		case 22:
			word += 0b11;
			word = parse_reg(word,arg[1]);
			word = parse_reg(word,arg[2]);
			word = word << 19;
			break;
		//B
		case 23:
			word += 0b1000;
			word = parse_cond(word, arg[1]);
			// encode label
			word = word << 24;
			break;
		//BI
		case 24:
			word += 0b1001;
			word = parse_cond(word, arg[1]);
			word = parse_reg(word,arg[2]);
			word = word << 19;
			break;
		//BL
		case 25:
			word += 0b1010;
			// encode label
			word = word << 28;
			break;
		//CALL
		case 26:
			word += 0b1011;
			word = parse_reg(word,arg[1]);
			word = word << 23;
			break;
		default:
			break;
	}

	return word;
}

uint32_t Assembler::parse_reg(uint32_t word, string arg)
{
	word = word << 5;

	if(arg.at(0) == 'R' || arg.at(0) == 'r')
	{
		switch(stoi(arg.substr(1, 2)))
		{
			case 1:
				word += 0b1;
				break;
			case 2:
				word += 0b10;
				break;
			case 3:
				word += 0b11;
				break;
			case 4:
				word += 0b100;
				break;
			case 5:
				word += 0b101;
				break;
			case 6:
				word += 0b110;
				break;
			case 7:
				word += 0b111;
				break;
			case 8:
				word += 0b1000;
				break;
			case 9:
				word += 0b1001;
				break;
			case 10:
				word += 0b1010;
				break;
			case 11:
				word += 0b1011;
				break;
			case 12:
				word += 0b1100;
				break;
			case 13:
				word += 0b1101;
				break;
			case 14:
				word += 0b1110;
				break;
			case 15:
				word += 0b1111;
				break;
			case 16:
				word += 0b10000;
				break;
			case 17:
				word += 0b10001;
				break;
			case 18:
				word += 0b10010;
				break;
			case 19:
				word += 0b10011;
				break;
			case 20:
				word += 0b10100;
				break;
			case 21:
				word += 0b10101;
				break;
			case 22:
				word += 0b10110;
				break;
			case 23:
				word += 0b10111;
				break;
			case 24:
				word += 0b11000;
				break;
			case 25:
				word += 0b11001;
				break;
			case 26:
				word += 0b11010;
				break;
			case 27:
				word += 0b11011;
				break;
			default:
				break;
		}
	}

	switch(arg.at(0))
	{
		case 'L':
			word += 0b11100;
			break;
		case 'S':
			word += 0b11101;
			break;
		case 'F':
			word += 0b11110;
			break;
		case 'P':
			word += 0b11111;
			break;
		default:
			break;
	}

	return word;

}

uint32_t Assembler::parse_imm(uint32_t word, string arg, int shift)
{
	int temp = stoi(arg);
	word = word << shift;
	return word += temp;
}

uint32_t Assembler::parse_cond(uint32_t word, string arg)
{

	word = word << 4;

	switch(arg.at(0))
	{
		case 'G':
			word += 0b1;
			break;
		case 'L':
			word += 0b10;
			break;
		case 'E':
			word += 0b11;
			break;
		case 'N':
			word += 0b100;
			break;
		case 'O':
			word += 0b101;
			break;
		case 'U':
			word += 0b110;
			break;
		case 'C':
			word += 0b111;
			break;
		case 'D':
			word += 0b1000;
			break;
		default:
			break;
	}
	return word;
}

void enumerate()
{
	s_mapTypeValues["ADD"] = ADD;
	s_mapTypeValues["SUB"] = SUB;
	s_mapTypeValues["RSUB"] = RSUB;
	s_mapTypeValues["CMP"] = CMP;
	s_mapTypeValues["AND"] = AND;
	s_mapTypeValues["OR"] = OR;
	s_mapTypeValues["NOT"] = NOT;
	s_mapTypeValues["XOR"] = XOR;
	s_mapTypeValues["LSR"] = LSR;
	s_mapTypeValues["LSL"] = LSL;
	s_mapTypeValues["ASR"] = ASR;
	s_mapTypeValues["ROR"] = ROR;
	s_mapTypeValues["ROL"] = ROL;
	s_mapTypeValues["MUL"] = MUL;
	s_mapTypeValues["UMUL"] = UMUL;
	s_mapTypeValues["DIV"] = DIV;
	s_mapTypeValues["UDIV"] = UDIV;
	s_mapTypeValues["MOD"] = MOD;
	s_mapTypeValues["UMOD"] = UMOD;
	s_mapTypeValues["MOV"] = MOV;
	s_mapTypeValues["TRN"] = TRN;
	s_mapTypeValues["LDR"] = LDR;
	s_mapTypeValues["STR"] = STR;
	s_mapTypeValues["B"] = B;
	s_mapTypeValues["BI"] = BI;
	s_mapTypeValues["BL"] = BL;
	s_mapTypeValues["CALL"] = CALL;
}
