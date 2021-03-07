#ifndef VMPARSER_H
#define VMPARSER_H

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

std::vector<std::string> ARITHMETIC_LIST = {
    "add",
    "sub",
    "neg",
    "eq",
    "gt",
    "lt",
    "and",
    "or",
    "not"
};

std::map<std::string,int> SEGMENT_MAP = {
    {"constant",256}
};

////////////////////////////////////////////////////////////////////////////////
class Parser
{
public:
    Parser() = default;
    Parser(std::ifstream &_InputFile) : _File(_InputFile) {}

    inline void constructor(std::ifstream &_InputFile);
    inline bool hasMoreCommands() const;
    void advance();
    inline COMMAND_TYPE commandType() const;
    std::string arg1() const;
    int arg2() const;

private:
    std::ifstream &_File;
    std::string _CurCommand;
    COMMAND_TYPE _CType;
    std::vector<std::string> _Token;
};

#endif