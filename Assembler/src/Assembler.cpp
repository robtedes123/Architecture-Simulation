#include "Assembler.h"

Assembler::Assembler(const char* fname_in, const char* fname_out)
{
	filename_in = fname_in;
	filename_out = fname_out;
}

void Assembler::parse()
{
	string temp;
	uint32_t bit_string;
	file_in.open(filename_in);
	file_out.open(filename_out);

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
		}
	}
	else
	{
	cout << "Unable to open input file" << endl;
	}

	file_in.close();
	file_out.close();

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
		case 0:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				word = parse_reg(word,arg[3]);
			}
			else
			{
				word += 0b1;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				//parse immediate
			}
			break;
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
				//parse immediate
				//add padding
			}
			break;
		case 2:
				word += 0b100;
				word = parse_reg(word,arg[1]);
				word = parse_reg(word,arg[2]);
				//parse immediate
				//add padding
			break;
		case 3:
			if((arg[2].at(0) == 'R' || arg[1].at(0) == 'r'))
			{
				word += 0b101;
			}
			else
			{
				word += 0b110;
			}
			break;
		case 4:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word += 0b111;
			}
			else
			{
				word += 0b1000;
			}
			break;
		case 5:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word += 0b1001;
			}
			else
			{
				word += 0b1010;
			}
			break;
		case 6:
			word += 0b1011;
			break;
		case 7:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word += 0b1100;
			}
			else
			{
				word += 0b1101;
			}
			break;
		case 8:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word += 0b1110;
			}
			else
			{
				word += 0b1111;
			}
			break;
		case 9:
			if((arg[3].at(0) == 'R' || arg[3].at(0) == 'r'))
			{
				word += 0b10000;
			}
			else
			{
				word += 0b10001;
			}
			break;
		case 10:
			word += 0b10010;
			break;
		case 11:
			word += 0b10011;
			break;
		case 12:
			word += 0b10100;
			break;
		case 13:
			word += 0b10101;
			break;
		case 14:
			word += 0b10110;
			break;
		case 15:
			word += 0b10111;
			break;
		case 16:
			word += 0b11000;
			break;
		case 17:
			word += 0b11001;
			break;
		case 18:
			word += 0b11010;
			break;
		case 19:
			if((arg[2].at(0) == 'R' || arg[1].at(0) == 'r'))
			{
				word += 0b11011;
			}
			else
			{
				word += 0b11100;
			}
			break;
		case 20:
			word += 0b11101;
			break;
		case 21:
			word += 0b10;
			break;
		case 22:
			word += 0b11;
			break;
		case 23:
			word += 0b1000;
			break;
		case 24:
			word += 0b1001;
			break;
		case 25:
			word += 0b1010;
			break;
		case 26:
			word += 0b1011;
			break;
		default:
			break;
	}

	return word;
}

uint32_t Assembler::parse_reg(uint32_t word, string arg)
{

	switch(stoi(arg.substr(1, 2)))
	{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15:
			break;
		default:
			break;
	}

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
