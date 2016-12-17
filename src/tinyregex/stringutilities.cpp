#include "stringutilities.h"

namespace tyre
{
    StringUtilities::StringUtilities()
    {

    }
    bool StringUtilities::isStr(const string_t &pattern,
                                      int curpos,
                                      const string_t &str,
                                      int str_begin,
                                      int str_end)
    {
        int pattern_postmp=curpos;
        int str_postmp=str_begin;
        int pattern_len=pattern.length();
        while(pattern_postmp<pattern_len&&str_postmp<=str_end)
        {
            if(pattern[pattern_postmp]!=str[str_postmp])
            {
                break;
            }
            ++str_postmp;
            ++pattern_postmp;
        }
        if(str_postmp==str_end+1)
        {
            //curpos=str_postmp;
            return true;
        }
        else
        {
            return false;
        }
    }
    bool StringUtilities::isStr(const tyre::string_t &pattern, int curpos, const tyre::string_t &str)
    {
        int pattern_postmp=curpos;
        int str_postmp=0;
        int pattern_len=pattern.length();
        int str_len=str.length();
        while(pattern_postmp<pattern_len&&str_postmp<str_len)
        {
            if(pattern[pattern_postmp]!=str[str_postmp])
            {
                break;
            }
            ++str_postmp;
            ++pattern_postmp;
        }
        if(str_postmp==str_len)
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

