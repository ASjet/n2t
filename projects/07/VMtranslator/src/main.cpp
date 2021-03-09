#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "utility.h"
#include "VMparser.h"
#include "CodeWriter.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
const char FILE_SEPARATOR = '\\';
#else
const char FILE_SEPARATOR = '/';
#endif
////////////////////////////////////////////////////////////////////////////////
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Get local path and output filename
    string local_path = string(1,'.') + string(1,FILE_SEPARATOR);
    string asm_filename("a.asm");
    string asm_filepath = local_path + asm_filename;

    std::ofstream output_asm_file;
    std::ifstream input_vm_file;
    output_asm_file.open(asm_filepath,std::ios::out);

    // Write initialize assembly code
    output_asm_file << "@256\nD=A\n@SP\nM=D\n@MAIN\n0;JMP\n"
                    << "(PUSH_TRUE)\n@SP\nA=M\nM=0\nM=M-1\n@SP\nM=M+1\n"
                    << "@LCL\nA=M\n0;JMP\n(MAIN)\n";

    CodeWriter writer(output_asm_file);
    Parser parser;

    for(int i = 1; i != argc; ++i)
    {
        // Get the filename of current file
        int offset=0,size=0;
        for(int j = 0; argv[i][j] != '\0'; ++j)
        {
            if(argv[i][j] == FILE_SEPARATOR)
                offset = j+1;
            if(argv[i][j] == '.')
                size = j-offset;
        }
        string vm_filename = string(argv[i]).substr(offset,size);

        input_vm_file.open(argv[i],std::ios::in);
        writer.setFileName(vm_filename);
        parser.constructor(input_vm_file);

        while(parser.hasMoreCommands())
        {
            if(parser.advance())
            {
                switch(parser.commandType())
                {
                    case C_PUSH:
                    case C_POP:
                        writer.writePushPop(parser.commandType(),parser.arg1(),parser.arg2());
                        break;
                    case C_ARITHMETIC:
                        writer.writeArithmetic(parser.arg1());
                        break;
                    default:
                        exit(-1);
                }
            }
            else
                break;
        }
        input_vm_file.close();
        SEGMENT_MAP["static"] += 10;
    }
    output_asm_file.close();
}