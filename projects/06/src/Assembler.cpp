#include "Binary.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <cctype>
#include <map>
////////////////////////////////////////////////////////////////////////////////
using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;
////////////////////////////////////////////////////////////////////////////////
enum COMMAND_TYPE
{
    A_COMMAND,
    C_COMMAND,
};
bool issymbol(char ch);
bool isexpr(char ch);
bool issplit(char ch);
bool isvariable(char ch);
const int VARIABLE_BASE_ADDRESS = 0x10;
map<string, int> SymbolTable;
vector<string> PredefinedSymbolTable{
    string("SP"),
    string("LCL"),
    string("ARG"),
    string("THIS"),
    string("THAT"),
    string("R0"),
    string("R1"),
    string("R2"),
    string("R3"),
    string("R4"),
    string("R5"),
    string("R6"),
    string("R7"),
    string("R8"),
    string("R9"),
    string("R10"),
    string("R11"),
    string("R12"),
    string("R13"),
    string("R14"),
    string("R15"),
    string("SCREEN"),
    string("KBD")};
vector<int> PredefinedSymbolValue{
    0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16384, 24576};
int pc;
int variable_cnt;
////////////////////////////////////////////////////////////////////////////////
class instruction
{
public:
    instruction() = default;
    instruction(string _instruction) : _Inst(_instruction) {}
    instruction parse()
    {
        _CommandType = C_COMMAND;
        string tmp;
        for (auto i = _Inst.begin(); i != _Inst.end(); ++i)
        {
            if (*i == '@')
            {
                tmp.clear();
                _CommandType = A_COMMAND;
                ++i;
                while (issymbol(*i))
                    tmp += *i++;
                _Symbol = tmp;
                break;
            }
        }

        if (_CommandType == C_COMMAND)
        {
            vector<string> expr;
            expr.clear();
            for (auto i = _Inst.begin(); i != _Inst.end(); ++i)
            {
                if (*i == '/' && *(i + 1) == '/')
                    break;

                if (issplit(*i))
                    continue;
                else
                {
                    tmp.clear();
                    while (!issplit(*i))
                        tmp += *i++;
                    expr.push_back(tmp);
                }
            }

            assert((expr.size() > 0 && expr.size() < 4) && "Too many commands in one line!");
            if (expr.size() == 1)
            {
                if (expr[0][0] == 'J')
                    _Jump = expr[0];
                else
                    _ComputeCommand = expr[0];
            }
            else if (expr.size() == 2)
            {
                if (expr[1][0] == 'J')
                {
                    _ComputeCommand = expr[0];
                    _Jump = expr[1];
                }
                else
                {
                    _Destination = expr[0];
                    _ComputeCommand = expr[1];
                }
            }
            else
            {
                _Destination = expr[0];
                _ComputeCommand = expr[1];
                _Jump = expr[2];
            }
        }
        return *this;
    }
    inline string symbol() const
    {
        assert((_CommandType == A_COMMAND) && "There is no symbol in C_COMMAND!");
        return _Symbol;
    }
    inline string dest() const
    {
        assert((_CommandType == C_COMMAND) && "Only C_COMMAND has a destination!");
        return _Destination;
    }
    inline string comp() const
    {
        assert((_CommandType == C_COMMAND) && "Only C_COMMAND has a compute command!");
        return _ComputeCommand;
    }
    inline string jump() const
    {
        assert((_CommandType == C_COMMAND) && "Only C_COMMAND has a jump command");
        return _Jump;
    }
    inline COMMAND_TYPE commandType() const
    {
        return _CommandType;
    }

private:
    string _Inst;
    COMMAND_TYPE _CommandType;
    string _Symbol;
    string _Destination;
    string _ComputeCommand;
    string _Jump;
};

////////////////////////////////////////////////////////////////////////////////
string translator(instruction _Inst);
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    // Initialize predefined symbols
    assert(PredefinedSymbolTable.size() == PredefinedSymbolValue.size());
    for (auto i = 0; i < PredefinedSymbolTable.size(); ++i)
        SymbolTable[PredefinedSymbolTable[i]] = PredefinedSymbolValue[i];

    // Get input "filename.asm" and convert to "filename.hack"
    string filename = string(argv[1]);
    string output_file_name = filename.substr(0, filename.size() - 4) + string(".hack");

    // initialize file stream
    std::ifstream asm_file;
    std::ofstream hack_file;

    string line; // Per line of input file
    string tmp;
    bool is_comment; // Flag of whether this line is a comment
    bool is_blank;   // Flag of whether this line is a blank

    asm_file.open(argv[1], std::ios::in);
    hack_file.open(output_file_name, std::ios::out);

    pc = 0; // Set program counter to 0
    variable_cnt = 0; // Set variable counter to 0

    // Preload input file to save label-pc pairs
    while (std::getline(asm_file, line))
    {
        is_comment = false;
        is_blank = true;
        for (auto i = line.begin(); i != line.end(); ++i)
        {
            if (!isspace(*i))
            {
                is_blank = false;
                if (*i == '/' && *(i + 1) == '/')
                    is_comment = true;
                else if (*i == '(')
                {
                    is_comment = true;
                    tmp.clear();
                    ++i;
                    while (*i != ')')
                        tmp += *i++;
                    SymbolTable[tmp] = pc;
                }
                break;
            }
        }

        if (!(is_comment || is_blank))
            ++pc;
    }

    // Set file pointer to head of file
    asm_file.clear();
    asm_file.seekg(0);
    pc = 0;

    // Load file and parse command
    while (std::getline(asm_file, line))
    {
        is_comment = false;
        is_blank = true;
        for (auto i = line.begin(); i != line.end(); ++i)
        {
            if (!isspace(*i))
            {
                is_blank = false;
                if ((*i == '/' && *(i + 1) == '/') || *i == '(')
                    is_comment = true;
                else if (*i == '@')
                {
                    tmp.clear();
                    ++i;
                    if (isvariable(*i))
                        while (issymbol(*i))
                            tmp += *i++;
                    // Allocate and save variable-address pairs     [C++20]
                    if (!(SymbolTable.contains(tmp) || tmp.empty()))
                    {
                        SymbolTable[tmp] = VARIABLE_BASE_ADDRESS + variable_cnt;
                        ++variable_cnt;
                    }
                }
                break;
            }
        }

        if (!(is_comment || is_blank))
        {
            instruction inst = instruction(line);
            inst.parse();

            if (pc)
                hack_file << endl << translator(inst);
            else
                hack_file << translator(inst);

            ++pc;
        }
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
string translator(instruction _Inst)
{
    string bin_inst;
    if (_Inst.commandType() == A_COMMAND)
    {
        int value;

        if (isdigit((_Inst.symbol()[0])))
            value = stoi(_Inst.symbol());
        else
            value = SymbolTable[_Inst.symbol()];

        bin_inst = string("0") + Binary(value).str(15, 15);
    }
    else
    {
        string flag_c, flag_d, flag_j;

        assert((!_Inst.comp().empty()) && "Unknown command!");
        if (_Inst.comp() == "0")
            flag_c = "0101010";
        else if (_Inst.comp() == "1")
            flag_c = "0111111";
        else if (_Inst.comp() == "-1")
            flag_c = "0111010";
        else if (_Inst.comp() == "D")
            flag_c = "0001100";
        else if (_Inst.comp() == "A")
            flag_c = "0110000";
        else if (_Inst.comp() == "M")
            flag_c = "1110000";
        else if (_Inst.comp() == "!D")
            flag_c = "0001101";
        else if (_Inst.comp() == "!A")
            flag_c = "0110001";
        else if (_Inst.comp() == "!M")
            flag_c = "1110001";
        else if (_Inst.comp() == "-D")
            flag_c = "0001111";
        else if (_Inst.comp() == "-A")
            flag_c = "0110011";
        else if (_Inst.comp() == "-M")
            flag_c = "1110011";
        else if (_Inst.comp() == "D+1")
            flag_c = "0011111";
        else if (_Inst.comp() == "A+1")
            flag_c = "0110111";
        else if (_Inst.comp() == "M+1")
            flag_c = "1110111";
        else if (_Inst.comp() == "D-1")
            flag_c = "0001110";
        else if (_Inst.comp() == "A-1")
            flag_c = "0110010";
        else if (_Inst.comp() == "M-1")
            flag_c = "1110010";
        else if (_Inst.comp() == "D+A")
            flag_c = "0000010";
        else if (_Inst.comp() == "D+M")
            flag_c = "1000010";
        else if (_Inst.comp() == "D-A")
            flag_c = "0010011";
        else if (_Inst.comp() == "D-M")
            flag_c = "1010011";
        else if (_Inst.comp() == "A-D")
            flag_c = "0000111";
        else if (_Inst.comp() == "M-D")
            flag_c = "1000111";
        else if (_Inst.comp() == "D&A")
            flag_c = "0000000";
        else if (_Inst.comp() == "D&M")
            flag_c = "1000000";
        else if (_Inst.comp() == "D|A")
            flag_c = "0010101";
        else if (_Inst.comp() == "D|M")
            flag_c = "1010101";

        if (_Inst.dest().empty())
            flag_d = "000";
        else
        {
            if (_Inst.dest() == "M")
                flag_d = "001";
            else if (_Inst.dest() == "D")
                flag_d = "010";
            else if (_Inst.dest() == "A")
                flag_d = "100";
            else if (_Inst.dest() == "MD" || _Inst.dest() == "DM")
                flag_d = "011";
            else if (_Inst.dest() == "AD" || _Inst.dest() == "DA")
                flag_d = "110";
            else if (_Inst.dest() == "AM" || _Inst.dest() == "MA")
                flag_d = "101";
            else
                flag_d = "111";
        }

        if (_Inst.jump().empty())
            flag_j = "000";
        else
        {
            if (_Inst.jump() == "JGT")
                flag_j = "001";
            else if (_Inst.jump() == "JEQ")
                flag_j = "010";
            else if (_Inst.jump() == "JLT")
                flag_j = "100";
            else if (_Inst.jump() == "JGE")
                flag_j = "011";
            else if (_Inst.jump() == "JLE")
                flag_j = "110";
            else if (_Inst.jump() == "JNE")
                flag_j = "101";
            else
                flag_j = "111";
        }

        bin_inst = string("111") + flag_c + flag_d + flag_j;
    }
    return bin_inst;
}
////////////////////////////////////////////////////////////////////////////////
bool issymbol(char ch)
{
    if (isalnum(ch) || ch == '_' || ch == '.' || ch == '$' || ch == ':')
        return true;
    else
        return false;
}
bool isexpr(char ch)
{
    if (ch == '@' || ch == 'A' || ch == 'M' || ch == 'D' || ch == '0' || ch == '1' || ch == 'J')
        return true;
    else
        return false;
}
bool issplit(char ch)
{
    if (isspace(ch) || ch == '=' || ch == ';')
        return true;
    else
        return false;
}
bool isvariable(char ch)
{
    if (isalpha(ch) || ch == '_' || ch == '.' || ch == '$' || ch == ':')
        return true;
    else
        return false;
}