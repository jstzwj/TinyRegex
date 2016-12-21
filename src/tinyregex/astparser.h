#ifndef ASTPARSER_H
#define ASTPARSER_H
#include<vector>
#include"base.h"
#include"astnode.h"
#include"regexerror.h"
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
    private:
        SyntaxFlag flag;
    public:
        AstParser();
        AstParser(SyntaxFlag _flag);
        //parser
        ExpBase * parse(const string_t & pattern);
        ExpBase * parseExp(const string_t & pattern,int &curpos);
        ExpBase * parseSequence(const string_t & pattern,int &curpos);
        ExpBase * parseUnit(const string_t & pattern,int &curpos);
        ExpBase * parseFunction(const string_t & pattern,int &curpos);
        ExpBase * parseLoop(const string_t & pattern,int &curpos);
        bool parseSeqChar(const string_t & pattern, int &curpos, CharRange *charRange);
        ExpBase * parseCharSet(const string_t & pattern,int &curpos);
        string_t parseName(const string_t & pattern,int &curpos);
        //tools
        bool isChar(const string_t & pattern,int &curpos,char_t c);
        bool isStr(const string_t & pattern,int &curpos,const string_t & str);
        bool isEnd(const string_t & pattern,int &curpos)
        {
            return pattern.length()==static_cast<unsigned int>(curpos);
        }
        bool isHex(char_t c)
        {
            if(c>=T('0')&&c<=T('9')||c>=T('a')&&c<=T('f')||c>=T('A')&&a<=T('F'))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        int getPositiveInt(const string_t & pattern,int &curpos);
        int getTokenLength(const string_t & pattern,int &curpos);
        char_t getChar(const string_t & pattern,int &curpos);
        int toNum(char_t c)
        {
            return c-T('0');
        }
        bool consume(const string_t & pattern,int &curpos);

    };
}


#endif // ASTPARSER_H
