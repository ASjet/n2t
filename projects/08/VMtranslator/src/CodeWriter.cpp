#include "CodeWriter.h"
////////////////////////////////////////////////////////////////////////////////
std::map<std::string, int> SEGMENT_MAP = {
    {"pointer", 3},
    {"temp",5},
    {"static",16}
};
std::string POP_M("@SP\nA=M-1\n");
std::string POP_D_POP_M("@SP\nA=M-1\nD=M\n@SP\nA=M-1\n");
std::string PUSH_SEGMENT_OFFSET("A=A+D\nD=M\n@SP\nA=M\nM=D\n");
std::string POP_D("@SP\nA=M-1\nD=M\n");
std::string PUSH_D("@SP\nA=M\nM=D\n@SP\nM=M+1\n");
std::string SAVE_A_TO_R15("\nD=A\n@R15\nM=D\n");
std::string INC_SP("@SP\nM=M+1\n");
////////////////////////////////////////////////////////////////////////////////
inline void CodeWriter::constructor(std::ofstream& _OutputFile)
{
    File = &_OutputFile;
    ArithmeticCnt = 0;
    CallCnt = 0;
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
        *File << POP_D_POP_M << "M=M+D\n" << INC_SP;
    }
    else if(_Command == "sub")
    {
        *File << POP_D_POP_M << "M=M-D\n" << INC_SP;
    }
    else if(_Command == "neg")
    {
        *File << POP_M << "M=-M\n" << INC_SP;
    }
    else if(_Command == "eq")
    {
        *File << '@' << label << SAVE_A_TO_R15 << POP_D_POP_M
              << "D=M-D\n@PUSH_TRUE\nD;JEQ\n@SP\nA=M\nM=0\n"
              << INC_SP << '(' << label << ")\n";
    }
    else if(_Command == "gt")
    {
        *File << '@' << label << SAVE_A_TO_R15 << POP_D_POP_M
              << "D=M-D\n@PUSH_TRUE\nD;JGT\n@SP\nA=M\nM=0\n"
              << INC_SP << '(' << label << ")\n";
    }
    else if(_Command == "lt")
    {
        *File << '@' << label << SAVE_A_TO_R15 << POP_D_POP_M
              << "D=M-D\n@PUSH_TRUE\nD;JLT\n@SP\nA=M\nM=0\n"
              << INC_SP << '(' << label << ")\n";
    }
    else if(_Command == "and")
    {
        *File << POP_D_POP_M << "M=M&D\n" << INC_SP;
    }
    else if(_Command == "or")
    {
        *File << POP_D_POP_M << "M=M|D\n" << INC_SP;
    }
    else if(_Command == "not")
    {
        *File << POP_M << "M=!M\n" << INC_SP;
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
            *File << '@' << _Index << "\nD=A\n@SP\nA=M\nM=D\n" << INC_SP;
        }
        else if(_Segment == "local")
            *File << "@LCL\nD=M\n@" << _Index << '\n' << PUSH_SEGMENT_OFFSET << INC_SP;
        else if(_Segment == "argument")
            *File << "@ARG\nD=M\n@" << _Index << '\n' << PUSH_SEGMENT_OFFSET << INC_SP;
        else if(_Segment == "this")
            *File << "@THIS\nD=M\n@" << _Index << '\n' << PUSH_SEGMENT_OFFSET << INC_SP;
        else if(_Segment == "that")
            *File << "@THAT\nD=M\n@" << _Index << '\n' << PUSH_SEGMENT_OFFSET << INC_SP;
        else
            *File << '@' << SEGMENT_MAP[_Segment] << "\nD=A\n@" << _Index << '\n'
                  << PUSH_SEGMENT_OFFSET << INC_SP;
    }
    else if(_Command == C_POP)
    {
        if(_Segment == "pointer")
            *File << POP_D << '@' << SEGMENT_MAP[_Segment] + _Index << "\nM=D\n";
        else
        {
            if(_Segment == "local")
                *File << POP_D << "@LCL\nA=M\n";
            else if(_Segment == "argument")
                *File << POP_D << "@ARG\nA=M\n";
            else if(_Segment == "this")
                *File << POP_D << "@THIS\nA=M\n";
            else if(_Segment == "that")
                *File << POP_D << "@THAT\nA=M\n";
            else
                *File << POP_D << '@' << SEGMENT_MAP[_Segment] << '\n';

            for(int i = 0; i != _Index; ++i)
                *File << "A=A+1\n";
            *File << "M=D\n";
        }
    }
}

void CodeWriter::writeInit()
{
    *File << "@256\nD=A\n@SP\nM=D\n@MAIN\n0;JMP\n"
          << "(PUSH_TRUE)\n@SP\nA=M\nM=-1\n@SP\nM=M+1\n@R15\nA=M\n0;JMP\n(MAIN)\n";
    writeCall("Sys.init",0);
}

void CodeWriter::writeLabel(std::string _Label)
{
    *File << '(' << _Label << ")\n";
}

void CodeWriter::writeGoto(std::string _Label)
{
    *File << '@' << _Label << "\n0;JMP\n";
}

void CodeWriter::writeIf(std::string _Label)
{
    *File << POP_D << '@' << _Label << "\nD;JNE\n";
}

void CodeWriter::writeCall(std::string _FunctionName, int _NumArgs)
{
    std::string return_address = Name + std::string(".call") + std::to_string(CallCnt);
    *File << '@' << return_address << "\nD=A\n" << PUSH_D
          << "@LCL\nD=M\n" << PUSH_D << "@ARG\nD=M\n" << PUSH_D
          << "@THIS\nD=M\n" << PUSH_D << "@THAT\nD=M\n" << PUSH_D
          << '@' << _NumArgs + 5 << "\nD=A\n@SP\nD=M-D\n@ARG\nM=D\n"
          << "@SP\nD=M\n@LCL\nM=D\n";
    writeGoto(_FunctionName);
    writeLabel(return_address);
    ++CallCnt;
}

void CodeWriter::writeReturn()
{
    *File << "@LCL\nD=M\n@R13\nM=D\n"
          << "@5\nA=D-A\nD=M\n@R14\nM=D\n"
          << POP_D << "@ARG\nA=M\nM=D\n"
          << "@ARG\nD=M+1\n@SP\nM=D\n"
          << "@R13\nA=M-1\nD=M\n@THAT\nM=D\n"
          << "@R13\nD=M\n@2\nA=D-A\nD=M\n@THIS\nM=D\n"
          << "@R13\nD=M\n@3\nA=D-A\nD=M\n@ARG\nM=D\n"
          << "@R13\nD=M\n@4\nA=D-A\nD=M\n@LCL\nM=D\n"
          << "@R14\nA=M\n0;JMP\n";
}

void CodeWriter::writeFunction(std::string _FunctionName, int _NumLocals)
{
    *File << '(' << _FunctionName << ")\n";
    while(_NumLocals--)
        writePushPop(C_PUSH,"constant",0);
}
