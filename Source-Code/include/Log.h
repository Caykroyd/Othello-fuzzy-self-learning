#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Log
{
    public:
        Log();
        ~Log();

        void Write(string str);
        void Start(string file);
    protected:
        ofstream output;
    private:
};

#endif // LOG_H
