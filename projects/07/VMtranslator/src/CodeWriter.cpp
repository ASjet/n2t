#include "CodeWriter.h"
////////////////////////////////////////////////////////////////////////////////
std::map<std::string, int> SEGMENT_MAP = {
    {"pointer", 3},
    {"local",300},
    {"argument",400},
    {"this",3000},
    {"that",3010},
    {"temp",5}
};
std::string GET_OPERATOR("@THAT\nD=M\n@THIS\n");
std::string JUDGE("\nD=A\n@LCL\nM=D\n@THAT\nD=M\n@THIS\nD=M-D\n@PUSH_TRUE_TO_THIS\n");
std::string INC_STACK_POINTER("@SP\nM=M+1\n");
std::string DEC_STACK_POINTER("@SP\nM=M-1\n");
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
    std::string label = Name + std::string(".") + _Command + std::to_string(ArithmeticCnt);
    if(_Command == "add")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << GET_OPERATOR << "M=M+D\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "sub")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << GET_OPERATOR << "M=M-D\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "neg")
    {
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << "@THIS\nM=-M\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "eq")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << '@' << label << JUDGE << "D;JEQ\n@THIS\nM=0\n"
              << '(' << label << ")\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "gt")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << '@' << label << JUDGE << "D;JGT\n@THIS\nM=0\n"
              << '(' << label << ")\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "lt")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << '@' << label << JUDGE << "D;JLT\n@THIS\nM=0\n"
              << '(' << label << ")\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "and")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << GET_OPERATOR << "M=M&D\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "or")
    {
        CodeWriter::writePushPop(C_POP,"pointer",1);
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << GET_OPERATOR << "M=M|D\n";
        CodeWriter::writePushPop(C_PUSH,"pointer",0);
    }
    else if(_Command == "not")
    {
        CodeWriter::writePushPop(C_POP,"pointer",0);
        *File << "@THIS\nM=!M\n";
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
            *File << '@' << _Index << "\nD=A\n@SP\nA=M\nM=D\n";
        }
        else
            *File << '@' << SEGMENT_MAP[_Segment] + _Index << "\nD=M\n@SP\nA=M\nM=D\n";

        *File << INC_STACK_POINTER;
    }
    else if(_Command == C_POP)
    {
        *File << "@SP\nM=M-1\nA=M\nD=M\n@" << SEGMENT_MAP[_Segment] + _Index << "\nM=D\n";
    }
}