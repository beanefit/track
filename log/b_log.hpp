#ifndef __B_LOG_HPP__
#define __B_LOG_HPP__

#include <fstream>
#include <cstdarg>

class b_log
{
public:

    b_log();
    ~b_log();

    void log (const char* msg, ...);

private:
    FILE* _logfile;
};

b_log :: b_log ()
{
    _logfile = fopen ("log", "w");
}

b_log :: ~b_log ()
{
    fclose (_logfile);
}

void b_log :: log (const char *msg, ...)
{
    va_list args;
    va_start (args, msg);
    vfprintf (_logfile, msg, args);
    va_end (args);
}

#endif //__B_LOG_HPP__