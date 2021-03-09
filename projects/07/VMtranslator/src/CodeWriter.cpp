#include "CodeWriter.h"
////////////////////////////////////////////////////////////////////////////////
std::map<std::string, int> SEGMENT_MAP = {
    {"pointer", 3},
    {"temp",5},
    {"static",16}
};
std::string GET_ONE_OPERATOR("@SP\nM=M-1\nA=M\n");
std::string GET_TWO_OPERATOR("@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\n");
std::string PUSH_SEGMENT_OFFSET("A=A+D\nD=M\n@SP\nA=M\nM=D\n");
std::string POP_SP_TO_D("@SP\nM=M-1\nA=M\nD=M\n");
std::string SAVE_A_TO_LCL("\nD=A\n@LCL\nM=D\n");
std::string INC_STACK_POINTER("@SP\nM=M+1\n");
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
        *File << GET_TWO_OPERATOR << "M=M+D\n" << INC_STACK_POINTER;
    }
    else if(_Command == "sub")
    {
        *File << GET_TWO_OPERATOR << "M=M-D\n" << INC_STACK_POINTER;
    }
    else if(_Command == "neg")
    {
        *File << GET_ONE_OPERATOR << "M=-M\n" << INC_STACK_POINTER;
    }
    else if(_Command == "eq")
    {
        *File << '@' << label << SAVE_A_TO_LCL << GET_TWO_OPERATOR
              << "D=M-D\n@PUSH_TRUE\nD;JEQ\n@SP\nA=M\nM=0\n"
              << INC_STACK_POINTER << '(' << label << ")\n";
    }
    else if(_Command == "gt")
    {
        *File << '@' << label << SAVE_A_TO_LCL << GET_TWO_OPERATOR
              << "D=M-D\n@PUSH_TRUE\nD;JGT\n@SP\nA=M\nM=0\n"
              << INC_STACK_POINTER << '(' << label << ")\n";
    }
    else if(_Command == "lt")
    {
        *File << '@' << label << SAVE_A_TO_LCL << GET_TWO_OPERATOR
              << "D=M-D\n@PUSH_TRUE\nD;JLT\n@SP\nA=M\nM=0\n"
              << INC_STACK_POINTER << '(' << label << ")\n";
    }
    else if(_Command == "and")
    {
        *File << GET_TWO_OPERATOR << "M=M&D\n" << INC_STACK_POINTER;
    }
    else if(_Command == "or")
    {
        *File << GET_TWO_OPERATOR << "M=M|D\n" << INC_STACK_POINTER;
    }
    else if(_Command == "not")
    {
        *File << GET_ONE_OPERATOR << "M=!M\n" << INC_STACK_POINTER;
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
            *File << '@' << _Index << "\nD=A\n@SP\nA=M\nM=D\n" << INC_STACK_POINTER;
        }
        else if(_Segment == "local")
            *File << "@LCL\nD=M\n@" << _Index << '\n' << PUSH_SEGMENT_OFFSET << INC_STACK_POINTER;
        else if(_Segment == "argument")
            *File << "@ARG\nD=M\n@" << _Index << '\n' << PUSH_SEGMENT_OFFSET << INC_STACK_POINTER;
        else if(_Segment == "this")
            *File << "@THIS\nD=M\n@" << _Index << '\n' << PUSH_SEGMENT_OFFSET << INC_STACK_POINTER;
        else if(_Segment == "that")
            *File << "@THAT\nD=M\n@" << _Index << '\n' << PUSH_SEGMENT_OFFSET << INC_STACK_POINTER;
        else
            *File << '@' << SEGMENT_MAP[_Segment] << "\nD=A\n@" << _Index << '\n'
                  << PUSH_SEGMENT_OFFSET << INC_STACK_POINTER;
    }
    else if(_Command == C_POP)
    {
        if(_Segment == "pointer")
            *File << POP_SP_TO_D << '@' << SEGMENT_MAP[_Segment] + _Index << "\nM=D\n";
        else
        {
            if(_Segment == "local")
                *File << POP_SP_TO_D << "@LCL\nA=M\n";
            else if(_Segment == "argument")
                *File << POP_SP_TO_D << "@ARG\nA=M\n";
            else if(_Segment == "this")
                *File << POP_SP_TO_D << "@THIS\nA=M\n";
            else if(_Segment == "that")
                *File << POP_SP_TO_D << "@THAT\nA=M\n";
            else
                *File << POP_SP_TO_D << '@' << SEGMENT_MAP[_Segment] << '\n';

            for(int i = 0; i != _Index; ++i)
                *File << "A=A+1\n";
            *File << "M=D\n";
        }
    }
}