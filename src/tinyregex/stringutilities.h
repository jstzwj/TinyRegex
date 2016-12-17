#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H

#include"base.h"

namespace tyre
{
    class StringUtilities
    {
    public:
        StringUtilities();
        //static member function
        static bool isStr(const string_t & pattern,int curpos,const string_t & str);
        static bool isStr(const string_t &pattern, int curpos, const string_t &str, int str_begin, int str_end);
        static char_t toUpper(char_t c);
        static char_t toLower(char_t c);

    };
}


#endif // STRINGUTILITIES_H
