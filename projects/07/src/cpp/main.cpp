#include <iostream>
#include <string>
#include <vector>
#include "utility.h"

////////////////////////////////////////////////////////////////////////////////
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    string str = string("    that is a sentence!   ");
    vector<string> words;
    stringSplit(str,' ',words);
    for(auto word : words)
        cout << word << '<' << endl;
}