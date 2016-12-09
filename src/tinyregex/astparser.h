#ifndef ASTPARSER_H
#define ASTPARSER_H
#include<vector>
#include"base.h"
#include"astnode.h"
#include"message.h"
/**
 *  The BNF of regex:
 *  Char : #A character#
 *  Number : #A number#
 *  SeqChar : Char "-" Char
 *  CharSet : "[" ["^"] {SeqChar|Char} "]" | "." | Char
 *  Loop : "{" [Number] "," [Number] "}" | "+" | "*" | NULL
 *  Unit :  (CharSet | "("Exp")") Loop
 *  Sequence : Unit { Unit }
 *  Exp : Sequence { "|" Sequence }
 */
namespace tyre
{
    /**
     * @brief The AstParser class
     * @author wangjun
     * @date 2016-12-9
     * @details The class is used to parse the regular expression to the abstract syntax tree.
     */
    class AstParser
    {
    public:
        AstParser();
        //error message
        std::vector<Message> errorMsg;
        //parser
        ExpBase * parse(const string_t & pattern);
        ExpBase * parseExp(const string_t & pattern,int &curpos);
        ExpBase * parseSequence(const string_t & pattern,int &curpos);
        ExpBase * parseUnit(const string_t & pattern,int &curpos);
        ExpBase * parseFunction(const string_t & pattern,int &curpos);
        ExpBase * parseLoop(const string_t & pattern,int &curpos);
        bool parseSeqChar(const string_t & pattern, int &curpos, CharRange *charRange);
        ExpBase * parseCharSet(const string_t & pattern,int &curpos);
        //tools
        bool isChar(const string_t & pattern,int &curpos,char_t c)
        {
            if((unsigned int)curpos>=pattern.length()||curpos<0)
            {
                return false;
            }
            if(pattern[curpos]==c)
            {
                ++curpos;
                return true;
            }
            else
            {
                return false;
            }
        }
        bool isStr(const string_t & pattern,int &curpos,const string_t & str)
        {
            int postmp=curpos;
            int pattern_len=pattern.length();
            int str_len=str.length();
            while(postmp<pattern_len&&postmp<str_len)
            {
                if(pattern[postmp]!=str[postmp])
                {
                    break;
                }
                ++postmp;
            }
            if(postmp==str_len)
            {
                curpos=postmp;
                return true;
            }
            else
            {
                return false;
            }
        }
        bool isEnd(const string_t & pattern,int &curpos)
        {
            return pattern.length()==static_cast<unsigned int>(curpos);
        }
        int getPositiveInt(const string_t & pattern,int &curpos)
        {
            int result=0;
            if(curpos<0)
            {
                return -1;
            }
            if(!(pattern[curpos]>=T('0')&&pattern[curpos]<=T('9')))
            {
                return -1;
            }
            while((unsigned int)curpos<pattern.length())
            {
                if(!(pattern[curpos]>=T('0')&&pattern[curpos]<=T('9')))
                {
                    break;
                }
                else
                {
                    result*=10;
                    result+=pattern[curpos]-T('0');
                }
                ++curpos;
            }
            return result;
        }

        int getTokenLength(const string_t & pattern,int &curpos)
        {
            int pos=curpos;
            while(
                  (pattern[pos]>T('A')&&pattern[pos]<T('Z'))||
                  (pattern[pos]>T('a')&&pattern[pos]<T('z'))||
                  (pattern[pos]>=T('0')&&pattern[pos]<=T('9'))||
                  (pattern[pos]==T('_'))
                  )
            {
                ++pos;
            }
            return pos-curpos;
        }


        void consume(const string_t & pattern,int &curpos)
        {
            if(static_cast<unsigned int>(curpos)<pattern.length())
            {
                ++curpos;
            }
        }

    };
}


#endif // ASTPARSER_H
