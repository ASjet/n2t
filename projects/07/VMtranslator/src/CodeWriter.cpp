#include "CodeWriter.h"
////////////////////////////////////////////////////////////////////////////////
std::map<std::string, int> SEGMENT_MAP = {
    {"pointer", 3},
};
std::string GET_OPERATOR("@THAT\r\nD=M\r\n@THIS\r\n");
std::string JUDGE("D=A\r\n@LCL\r\nM=D\r\n@THAT\r\nD=M\r\n@THIS\r\nD=M-D\r\n@PUSH_TRUE_TO_THIS\r\n");
std::string INC_STACK_POINTER("@SP\r\nM=M+1\r\n");
std::string DEC_STACK_POINTER("@SP\r\nM=M-1\r\n");
////////////////////////////////////////////////////////////////////////////////
inline void CodeWriter::constructor(std::ofstream& _OutputFile)
{
    File = &_OutputFile;
    ArithmeticCnt = 0;
}
void CodeWriter::setFileName(const std::string _FileName)
{
    Name = _FileName;
}
void CodeWriter::writeArithmetic(std::string _Command)
{
    std::string label = Name + std::string(".") + _Command + std::to_string(ArithmeticCnt) + std::string("\r\n");
    if(_Command == "add")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << GET_OPERATOR << "M=M+D\r\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "sub")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << GET_OPERATOR << "M=M-D\r\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "neq")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << '@' << label << JUDGE << "D;JNE\r\n"
              << '(' << label << ")\r\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "eq")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << '@' << label << JUDGE << "D;JEQ\r\n"
              << '(' << label << ")\r\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "gt")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << '@' << label << JUDGE << "D;JGT\r\n"
              << '(' << label << ")\r\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "lt")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << '@' << label << JUDGE << "D;JLT\r\n"
              << '(' << label << ")\r\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "and")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << GET_OPERATOR << "M=M&D\r\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "or")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << GET_OPERATOR << "M=M|D\r\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "not")
    {
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << "@THIS\r\nM=!M\r\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else
        std::cerr << "Unknown command \"" << _Command << '\"' << std::endl;
    ++ArithmeticCnt;
}
void CodeWriter::writePushPop(COMMAND_TYPE _Command, std::string _Segment, int _Index)
{
    if(_Command == C_PUSH)
    {
        if(_Segment == "constant")
        {
            _Index %= 32768;
            *File << '@' << _Index << "\r\nD=A\r\n@SP\r\nA=M\r\nM=D\r\n";
        }
        else if(_Segment == "pointer")
        {
            *File << '@' << SEGMENT_MAP[_Segment] + _Index << "\r\nD=M\r\n@SP\r\nA=M\r\nM=D\r\n";
        }
        *File << INC_STACK_POINTER;
    }
    else if(_Command == C_POP)
    {
        if(_Segment == "pointer")
        {
            *File << "@SP\r\nM=M-1\r\nA=M\r\nD=M\r\n@" << SEGMENT_MAP[_Segment] + _Index << "\r\nM=D\r\n";
        }
    }
}