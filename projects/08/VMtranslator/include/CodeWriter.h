#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "VMparser.h"

////////////////////////////////////////////////////////////////////////////////
extern std::map<std::string,int> SEGMENT_MAP;
extern std::string GET_OPERATOR;
extern std::string JUDGE;
////////////////////////////////////////////////////////////////////////////////
class CodeWriter
{
public:
    CodeWriter() = default;
    CodeWriter(std::ofstream &_OutputFile) : File(&_OutputFile), ArithmeticCnt(0) {}
    ~CodeWriter()
    {
        File = nullptr;
    }

    inline void constructor(std::ofstream &_OutputFile);
    void setFileName(std::string _FileName);
    void writeArithmetic(std::string _Command);
    void writePushPop(COMMAND_TYPE _Command, std::string _Segment, int _Index);
    void writeInit();
    void writeLabel(std::string _Label);
    void writeGoto(std::string _Label);
    void writeIf(std::string _Label);
    void writeCall(std::string _FunctionName, int _NumArgs);
    void writeReturn();
    void writeFunction(std::string _FunctionName, int _NumLocals);
    void close();

private:
    std::ofstream *File;
    std::string Name;
    int ArithmeticCnt;
    int CallCnt;
};
#endif