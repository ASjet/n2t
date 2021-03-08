#include "CodeWriter.h"
////////////////////////////////////////////////////////////////////////////////
inline void CodeWriter::constructor(std::ofstream& _OutputFile)
{
    _File = _OutputFile;
    _ArithmeticCnt = 0;
}
void CodeWriter::setFileName(const std::string _FileName)
{
    _Name = _FileName;
}
void CodeWriter::writeArithmetic(std::string _Command)
{
    std::string label = _Name + std::string(".") + _Command + std::to_string(_ArithmeticCnt);
    if(_Command == "add")
    {
        CodeWriter::writePushPop(C_POP,"that",0);
        CodeWriter::writePushPop(C_POP,"this",0);
        _File << GET_OPERATOR << "M=M+D\r\n";
        CodeWriter::writePushPop(C_PUSH,"this",0);
    }
    else if(_Command == "sub")
    {
        CodeWriter::writePushPop(C_POP,"that",0);
        CodeWriter::writePushPop(C_POP,"this",0);
        _File << GET_OPERATOR << "M=M-D\r\n";
        CodeWriter::writePushPop(C_PUSH,"this",0);
    }
    else if(_Command == "neq")
    {
        CodeWriter::writePushPop(C_POP,"that",0);
        CodeWriter::writePushPop(C_POP,"this",0);
        _File << '@' << label << JUDGE << "D;JNE\r\n"
              << '(' << label << ")\r\n";
        CodeWriter::writePushPop(C_PUSH,"this",0);
    }
    else if(_Command == "eq")
    {
        CodeWriter::writePushPop(C_POP,"that",0);
        CodeWriter::writePushPop(C_POP,"this",0);
        _File << '@' << label << JUDGE << "D;JEQ\r\n"
              << '(' << label << ")\r\n";
        CodeWriter::writePushPop(C_PUSH,"this",0);
    }
    else if(_Command == "gt")
    {
        CodeWriter::writePushPop(C_POP,"that",0);
        CodeWriter::writePushPop(C_POP,"this",0);
        _File << '@' << label << JUDGE << "D;JGT\r\n"
              << '(' << label << ")\r\n";
        CodeWriter::writePushPop(C_PUSH,"this",0);
    }
    else if(_Command == "lt")
    {
        CodeWriter::writePushPop(C_POP,"that",0);
        CodeWriter::writePushPop(C_POP,"this",0);
        _File << '@' << label << JUDGE << "D;JLT\r\n"
              << '(' << label << ")\r\n";
        CodeWriter::writePushPop(C_PUSH,"this",0);
    }
    else if(_Command == "and")
    {
        CodeWriter::writePushPop(C_POP,"that",0);
        CodeWriter::writePushPop(C_POP,"this",0);
        _File << GET_OPERATOR << "M=M&D\r\n";
        CodeWriter::writePushPop(C_PUSH,"this",0);
    }
    else if(_Command == "or")
    {
        CodeWriter::writePushPop(C_POP,"that",0);
        CodeWriter::writePushPop(C_POP,"this",0);
        _File << GET_OPERATOR << "M=M|D\r\n";
        CodeWriter::writePushPop(C_PUSH,"this",0);
    }
    else if(_Command == "not")
    {
        CodeWriter::writePushPop(C_POP,"this",0);
        _File << "@THIS\r\nM=!M\r\n";
        CodeWriter::writePushPop(C_PUSH,"this",0);
    }
    else
        std::cerr << "Unknown command \"" << _Command << '\"' << std::endl;
}
void writePushPop(COMMAND_TYPE _Command, std::string _Segment, int _Index)
{
;
}