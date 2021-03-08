#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "VMparser.h"

////////////////////////////////////////////////////////////////////////////////
std::string GET_OPERATOR("@THAT\r\nD=M\r\n@THIS\r\n");
std::string JUDGE("\r\nD=A\r\n@LCL\r\nM=D\r\n@THAT\r\nD=M\r\n@THIS\r\nD=M-D\r\n@PUSH_TRUE_TO_THIS\r\n");
////////////////////////////////////////////////////////////////////////////////
class CodeWriter
{
public:
    CodeWriter() = default;
    CodeWriter(std::ofstream& _OutputFile): _File(_OutputFile),_ArithmeticCnt(0){}

    inline void constructor(std::ofstream & _OutputFile);
    void setFileName(std::string _FileName);
    void writeArithmetic(std::string _Command);
    void writePushPop(COMMAND_TYPE _Command, std::string _Segment, int _Index);
    void close();
private:
    std::ofstream& _File;
    std::string _Name;
    int _ArithmeticCnt;
};
#endif