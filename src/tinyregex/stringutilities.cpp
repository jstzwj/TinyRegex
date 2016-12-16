#include "stringutilities.h"

namespace tyre
{
    StringUtilities::StringUtilities()
    {

    }

    bool tyre::StringUtilities::isStr(const tyre::string_t &pattern, int curpos, const tyre::string_t &str)
    {
        int pattern_len=pattern.length();
        int str_len=str.length();
        while(curpos<pattern_len&&curpos<str_len)
        {
            if(pattern[curpos]!=str[curpos])
            {
                break;
            }
            ++curpos;
        }
        if(curpos==str_len)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    char_t StringUtilities::toUpper(char_t c)
    {
        return c-(T('a')-T('A'));
    }

    char_t StringUtilities::toLower(char_t c)
    {
        return c+T('a')-T('A');
    }
}

