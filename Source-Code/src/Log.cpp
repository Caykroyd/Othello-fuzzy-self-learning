#include "Log.h"

Log::Log()
{
}

Log::~Log()
{
}

void Log::Start(string file)
{
    output.open(file.c_str());
    if(output.fail()) {
        cerr << "Error opening file." << endl;
        return;
    }
}

void Log::Write(string str)
{
    if(output.fail()) {
        cerr << "File not initiated." << endl;
        return;
    }

    output << str;
}
