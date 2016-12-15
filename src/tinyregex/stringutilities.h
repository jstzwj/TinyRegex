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

    };
}


#endif // STRINGUTILITIES_H
