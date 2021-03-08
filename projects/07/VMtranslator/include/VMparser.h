#ifndef VMPARSER_H
#define VMPARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "utility.h"

////////////////////////////////////////////////////////////////////////////////
enum COMMAND_TYPE
{
    UNKNOWN,
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL
};

extern std::vector<std::string> ARITHMETIC_LIST;

////////////////////////////////////////////////////////////////////////////////
class Parser
{
public:
    Parser() = default;
    Parser(std::ifstream &_InputFile) : File(&_InputFile) {}
    ~Parser(){
        File = nullptr;
    }

    void constructor(std::ifstream &_InputFile);
    bool hasMoreCommands() const;
    bool advance();
    COMMAND_TYPE commandType() const;
    std::string arg1() const;
    int arg2() const;

private:
    std::ifstream* File;
    std::string CurCommand;
    COMMAND_TYPE CType;
    std::vector<std::string> Token;
};

#endif