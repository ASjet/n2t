#include "VMparser.h"
////////////////////////////////////////////////////////////////////////////////
inline void Parser::constructor(std::ifstream &_InputFile)
{
    _File = _InputFile;
}

inline bool Parser::hasMoreCommands() const
{
    return _File.eof;
}
void Parser::advance()
{
    bool is_arithmetic = false;
    _CurCommand.clear();
    std::getline(_File, _CurCommand);
    stringSplit(_CurCommand, _Token);

    if (_Token[0] == "push")
        _CType = C_PUSH;
    else if (_Token[0] == "pop")
        _CType = C_POP;
    else if (_Token[0] == "label")
        _CType = C_LABEL;
    else if (_Token[0] == "if")
        _CType = C_IF;
    else if (_Token[0] == "goto")
        _CType = C_GOTO;
    else if (_Token[0] == "call")
        _CType = C_CALL;
    else if (_Token[0] = "function")
        _CType = C_FUNCTION;
    else if (_Token[0] = "return")
        _CType = C_RETURN;
    else
    {
        is_arithmetic = false;
        for (auto ari : ARITHMETIC_LIST)
            if (_Token[0] == ari)
                is_arithmetic = true;
        if (is_arithmetic)
            _CType = C_ARITHMETIC;
        else
            _CType = UNKNOWN;
    }
}
inline COMMAND_TYPE Parser::commandType() const
{
    return _CType;
}
std::string Parser::string arg1() const
{
    return _Token[1];
}
int Parser::arg2() const
{
    return std::stoi(_Token[2]);
}