#include "VMparser.h"
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
void Parser::constructor(std::ifstream &_InputFile)
{
    File = &_InputFile;
}

bool Parser::hasMoreCommands() const
{
    return !File->eof();
}
bool Parser::advance()
{
    bool is_arithmetic = false;
    Token.clear();

    // Filte comments and blank line
    do
    {
        CurCommand.clear();
        if(hasMoreCommands())
            std::getline(*File, CurCommand);
        else
            return false;
    }
    while(isComment(CurCommand));


    int offset = CurCommand.size();
    for(auto i = CurCommand.begin(); i != CurCommand.end(); ++i)
    {
        if(*i == '\r' || *i == '\n')
            offset = i - CurCommand.begin();
    }

    CurCommand = CurCommand.substr(0, offset);

    stringSplit(CurCommand, ' ', Token);

    if (Token[0] == "push")
        CType = C_PUSH;
    else if (Token[0] == "pop")
        CType = C_POP;
    else if (Token[0] == "label")
        CType = C_LABEL;
    else if (Token[0] == "if")
        CType = C_IF;
    else if (Token[0] == "goto")
        CType = C_GOTO;
    else if (Token[0] == "call")
        CType = C_CALL;
    else if (Token[0] == "function")
        CType = C_FUNCTION;
    else if (Token[0] == "return")
        CType = C_RETURN;
    else
    {
        is_arithmetic = false;
        for (auto ari : ARITHMETIC_LIST)
            if (Token[0] == ari)
                is_arithmetic = true;
        if (is_arithmetic)
            CType = C_ARITHMETIC;
        else
            CType = UNKNOWN;
    }
    return -1;
}
COMMAND_TYPE Parser::commandType() const
{
    return CType;
}
std::string Parser::arg1() const
{
    if(CType == C_ARITHMETIC)
        return Token[0];
    else
        return Token[1];
}
int Parser::arg2() const
{
    return std::stoi(Token[2]);
}