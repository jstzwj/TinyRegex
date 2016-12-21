#include "astparser.h"
namespace tyre
{

    AstParser::AstParser()
        :flag(SyntaxFlag::DEFAULT){}
    AstParser::AstParser(SyntaxFlag _flag)
        :flag(_flag){}

    ExpBase *AstParser::parse(const string_t &pattern)
    {
        int savePos=0;
        ExpBase * result=this->parseExp(pattern,savePos);
        return result;
    }

    ExpBase *AstParser::parseExp(const string_t &pattern, int &curpos)
    {
        int savePos=curpos;
        ExpBase *left=this->parseSequence(pattern,curpos);
        /*
        if(left==nullptr)
        {
            //error
            curpos=savePos;
            return nullptr;
        }
        */
        while(true)
        {
            if(isChar(pattern,curpos,T('|')))
            {
                ExpBase *right=this->parseSequence(pattern,curpos);
                if(right!=nullptr)
                {
                    ExpOr *result=new ExpOr;
                    result->left=left;
                    result->right=right;
                    left=result;
                }
                else
                {
                    //error
                    curpos=savePos;
                    break;
                }
            }
            else
            {
                //end
                break;
            }
        }

        return left;
    }

    ExpBase *AstParser::parseSequence(const string_t &pattern, int &curpos)
    {
        int savePos=curpos;
        ExpBase *left=this->parseUnit(pattern,curpos);
        /*
        if(left==nullptr)
        {
            //error
            curpos=savePos;
            return nullptr;
        }
        */
        while(true)
        {
            ExpBase *right=this->parseUnit(pattern,curpos);
            if(right!=nullptr)
            {
                ExpAnd *result=new ExpAnd;
                result->left=left;
                result->right=right;
                left=result;
            }
            else
            {
                //end
                break;
            }
        }

        return left;
    }

    ExpBase *AstParser::parseUnit(const string_t &pattern, int &curpos)
    {
        int savePos=curpos;
        ExpBase * function=nullptr;
        function=this->parseCharSet(pattern,curpos);
        if(function!=nullptr)
        {
            ExpBase * loopBase=this->parseLoop(pattern,curpos);
            if(loopBase!=nullptr)
            {
                ExpLoop *loop=dynamic_cast<ExpLoop *>(loopBase);
                loop->exp=function;
                return loop;
            }
            else
            {
                return function;
            }
        }
        else
        {
            function=this->parseFunction(pattern,curpos);
            if(function!=nullptr)
            {
                ExpLoop *loop=dynamic_cast<ExpLoop *>(this->parseLoop(pattern,curpos));
                if(loop!=nullptr)
                {
                    loop->exp=function;
                    return loop;
                }
                else
                {
                    return function;
                }
            }
            else
            {
                curpos=savePos;
                return nullptr;
            }
        }

    }

    ExpBase *AstParser::parseFunction(const string_t &pattern, int &curpos)
    {
        int savePos=curpos;
        if(isChar(pattern,curpos,T('('))==true)
        {
            ExpBase * exp=nullptr;
            ExpFunction * function=new ExpFunction;
            if(isStr(pattern,curpos,T("?:"))==true||
                    ((flag&SyntaxFlag::NOSUBS)!=0))//if enable nosubs option
            {
                exp=this->parseExp(pattern,curpos);
                function->subexp=exp;
                function->type=FunctionType::NOCAPTURE;
            }
            else if(isStr(pattern,curpos,T("?<")))
            {
                function->name=parseName(pattern,curpos);
                if(!isChar(pattern,curpos,T('>')))
                {
                    delete function;
                    throw RegexError(ErrorCode::error_name);
                }
                exp=this->parseExp(pattern,curpos);
                function->subexp=exp;
                function->type=FunctionType::NAMEDCAPTURE;
            }
            else
            {
                exp=this->parseExp(pattern,curpos);
                function->subexp=exp;
                function->type=FunctionType::CAPTURE;
            }
            if(exp!=nullptr)
            {
                if(isChar(pattern,curpos,T(')'))==true)
                {
                    return function;
                }
                else
                {
                    delete function;
                    throw RegexError(ErrorCode::error_paren);
                }

            }
        }
        return nullptr;
    }

    ExpBase *AstParser::parseLoop(const string_t &pattern, int &curpos)
    {
        ExpLoop * loop=nullptr;
        //* and +
        if(isChar(pattern,curpos,T('*')))
        {
            loop=new ExpLoop;
            loop->min=0;
            loop->max=-1;
            if(isChar(pattern,curpos,T('?')))
            {
                loop->lazy=true;
            }
            else
            {
                loop->lazy=false;
            }
            return loop;
        }
        else if(isChar(pattern,curpos,T('+')))
        {
            loop=new ExpLoop;
            loop->min=1;
            loop->max=-1;
            if(isChar(pattern,curpos,T('?')))
            {
                loop->lazy=true;
            }
            else
            {
                loop->lazy=false;
            }
            return loop;
        }
        else if(isChar(pattern,curpos,T('?')))
        {
            loop=new ExpLoop;
            loop->min=0;
            loop->max=1;
            if(isChar(pattern,curpos,T('?')))
            {
                loop->lazy=true;
            }
            else
            {
                loop->lazy=false;
            }
            return loop;
        }
        else if(isChar(pattern,curpos,T('{')))
        {
            loop=new ExpLoop;
            int min=getPositiveInt(pattern,curpos);
            if(min!=-1)
            {
                loop->min=min;
            }
            else
            {
                loop->min=0;
            }
            if(isChar(pattern,curpos,T(',')))
            {
                int max=getPositiveInt(pattern,curpos);
                if(max!=-1)
                {
                    loop->max=max;
                }
                else
                {
                    loop->max=-1;
                }
            }
            else
            {
                loop->max=min;
            }
            if(!isChar(pattern,curpos,T('}')))
            {
                delete loop;
                throw RegexError(ErrorCode::error_brace);
            }
        }
        if(loop!=nullptr)
        {
            if(isChar(pattern,curpos,T('?')))
            {
                loop->lazy=true;
            }
            else
            {
                loop->lazy=false;
            }
        }
        return loop;
    }

    bool AstParser::parseSeqChar(const string_t &pattern, int &curpos,CharRange *charRange)
    {

        if((unsigned int)(curpos)<pattern.length()-2&&curpos>=0)
        {
            if(pattern[curpos+1]==T('-'))
            {
                charRange->charBegin=pattern[curpos];
                charRange->charEnd=pattern[curpos+2];
                curpos+=3;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    std::vector<CharRange> AstParser::parseChar(const string_t &pattern, int &curpos)
    {
        std::vector<CharRange> ranges;
        if(this->isChar(pattern,curpos,T('\\')))
        {
            switch(pattern[curpos])
            {
            //escape sequence
            case T('r'):
                ranges.push_back(CharRange(T('\r'),T('\r')));
                consume(pattern,curpos);
                break;
            case T('t'):
                ranges.push_back(CharRange(T('\t'),T('\t')));
                consume(pattern,curpos);
                break;
            case T('n'):
                ranges.push_back(CharRange(T('\n'),T('\n')));
                consume(pattern,curpos);
                break;
            case T('f'):
                ranges.push_back(CharRange(T('\f'),T('\f')));
                consume(pattern,curpos);
                break;
            case T('v'):
                ranges.push_back(CharRange(T('\v'),T('\v')));
                consume(pattern,curpos);
                break;
            //key words
            case T('\\'):
            case T('('):
            case T(')'):
            case T('['):
            case T(']'):
            case T('{'):
            case T('}'):
            case T('<'):
            case T('>'):
            case T('='):
            case T('+'):
            case T('?'):
            case T('.'):
            case T('!'):
            case T('^'):
            case T('$'):
            case T('|'):
            case T('*'):
                ranges.push_back(CharRange(pattern[curpos],pattern[curpos]));
                consume(pattern,curpos);
                break;
            //real ordinary charset
            case T('s'):
                ranges.push_back(CharRange(T('\r'),T('\r')));
                ranges.push_back(CharRange(T('\t'),T('\t')));
                ranges.push_back(CharRange(T('\n'),T('\n')));
                consume(pattern,curpos);
                break;
            case T('S'):
                ranges.push_back(CharRange(T('\r'),T('\r'),true));
                ranges.push_back(CharRange(T('\t'),T('\t'),true));
                ranges.push_back(CharRange(T('\n'),T('\n'),true));
                consume(pattern,curpos);
                break;
            case T('d'):
                ranges.push_back(CharRange(T('0'),T('9')));
                consume(pattern,curpos);
                break;
            case T('D'):
                ranges.push_back(CharRange(T('0'),T('9'),true));
                consume(pattern,curpos);
                break;
            case T('x'):
            case T('X'):
            {
                consume(pattern,curpos);
                char_t first=getChar(pattern,curpos);
                char_t second=getChar(pattern,curpos);
                if(isHex(first)&&isHex(second))
                {
                    int num=toNum(first)*16+toNum(second);
                    ranges.push_back(CharRange(num,num));
                }
                else
                {
                    throw RegexError(ErrorCode::error_escape);
                }
            }
                break;
            default:
                ranges.push_back(CharRange(pattern[curpos],pattern[curpos],false));
                consume(pattern,curpos);
                break;
            }
        }
        else if(this->isChar(pattern,curpos,T('.')))
        {
            ranges.push_back(CharRange(1,MAX_CHAR));
        }
        else
        {
            ranges.push_back(CharRange(pattern[curpos],pattern[curpos],false));
            consume(pattern,curpos);
        }
        return ranges;
    }

    ExpBase *AstParser::parseCharSet(const string_t &pattern, int &curpos)
    {
        int savePos=curpos;
        if((unsigned int)curpos>=pattern.length())
        {
            curpos=savePos;
            return nullptr;
        }
        if(this->isChar(pattern,curpos,T('[')))
        {
            ExpCharRange * range=new ExpCharRange;
            //isIcase
            if((this->flag&SyntaxFlag::ICASE)!=0)
            {
                range->icase=true;
            }
            //posix character
            if(this->isChar(pattern,curpos,T(':')))
            {
                if(this->isStr(pattern,curpos,T("alnum")))
                {
                    range->ranges.push_back(CharRange(T('a'),T('z')));
                    range->ranges.push_back(CharRange(T('A'),T('Z')));
                    range->ranges.push_back(CharRange(T('0'),T('9')));
                }
                else if(this->isStr(pattern,curpos,T("alpha")))
                {
                    range->ranges.push_back(CharRange(T('a'),T('z')));
                    range->ranges.push_back(CharRange(T('A'),T('Z')));
                }
                else if(this->isStr(pattern,curpos,T("ascii")))
                {
                    range->ranges.push_back(CharRange(0x00,0x7F));
                }
                else if(this->isStr(pattern,curpos,T("blank")))
                {
                    range->ranges.push_back(CharRange(T(' '),T(' ')));
                    range->ranges.push_back(CharRange(T('\t'),T('\t')));
                }
                else if(this->isStr(pattern,curpos,T("cntrl")))
                {
                    range->ranges.push_back(CharRange(0x00,0x1F));
                    range->ranges.push_back(CharRange(0x7F,0x7F));
                }
                else if(this->isStr(pattern,curpos,T("digit")))
                {
                    range->ranges.push_back(CharRange(T('0'),T('9')));
                }
                else if(this->isStr(pattern,curpos,T("graph")))
                {
                    range->ranges.push_back(CharRange(0x21,0x7E));
                }
                else if(this->isStr(pattern,curpos,T("lower")))
                {
                    range->ranges.push_back(CharRange(T('a'),T('z')));
                }
                else if(this->isStr(pattern,curpos,T("print")))
                {
                    range->ranges.push_back(CharRange(0x20,0x7E));
                }
                else if(this->isStr(pattern,curpos,T("punct")))
                {
                    //!"#$%&'()*+,\-./:;<=>?@[\\\]^_`{|}~
                    range->ranges.push_back(CharRange(T('!'),T('!')));
                    range->ranges.push_back(CharRange(T('\"'),T('\"')));
                    range->ranges.push_back(CharRange(T('#'),T('#')));
                    range->ranges.push_back(CharRange(T('$'),T('$')));
                    range->ranges.push_back(CharRange(T('%'),T('%')));
                    range->ranges.push_back(CharRange(T('&'),T('&')));
                    range->ranges.push_back(CharRange(T('\''),T('\'')));
                    range->ranges.push_back(CharRange(T('('),T('(')));
                    range->ranges.push_back(CharRange(T(')'),T(')')));
                    range->ranges.push_back(CharRange(T('*'),T('*')));
                    range->ranges.push_back(CharRange(T('+'),T('+')));
                    range->ranges.push_back(CharRange(T(','),T(',')));
                    range->ranges.push_back(CharRange(T('\\'),T('\\')));
                    range->ranges.push_back(CharRange(T('-'),T('-')));
                    range->ranges.push_back(CharRange(T('.'),T('.')));
                    range->ranges.push_back(CharRange(T('/'),T('/')));
                    range->ranges.push_back(CharRange(T(':'),T(':')));
                    range->ranges.push_back(CharRange(T(';'),T(';')));
                    range->ranges.push_back(CharRange(T('<'),T('<')));
                    range->ranges.push_back(CharRange(T('>'),T('>')));
                    range->ranges.push_back(CharRange(T('?'),T('?')));
                    range->ranges.push_back(CharRange(T('@'),T('@')));
                    range->ranges.push_back(CharRange(T('['),T('[')));
                    range->ranges.push_back(CharRange(T(']'),T(']')));
                    range->ranges.push_back(CharRange(T('^'),T('^')));
                    range->ranges.push_back(CharRange(T('_'),T('_')));
                    range->ranges.push_back(CharRange(T('`'),T('`')));
                    range->ranges.push_back(CharRange(T('{'),T('{')));
                    range->ranges.push_back(CharRange(T('}'),T('}')));
                    range->ranges.push_back(CharRange(T('|'),T('|')));
                    range->ranges.push_back(CharRange(T('~'),T('~')));
                }
                else if(this->isStr(pattern,curpos,T("space")))
                {
                    range->ranges.push_back(CharRange(T(' '),T(' ')));
                    range->ranges.push_back(CharRange(T('\t'),T('\t')));
                    range->ranges.push_back(CharRange(T('\r'),T('\r')));
                    range->ranges.push_back(CharRange(T('\n'),T('\n')));
                    range->ranges.push_back(CharRange(T('\v'),T('\v')));
                    range->ranges.push_back(CharRange(T('\f'),T('\f')));
                }
                else if(this->isStr(pattern,curpos,T("upper")))
                {
                    range->ranges.push_back(CharRange(T('A'),T('Z')));
                }
                else if(this->isStr(pattern,curpos,T("word")))
                {
                    range->ranges.push_back(CharRange(T('a'),T('z')));
                    range->ranges.push_back(CharRange(T('A'),T('Z')));
                    range->ranges.push_back(CharRange(T('0'),T('9')));
                    range->ranges.push_back(CharRange(T('_'),T('_')));
                }
                else if(this->isStr(pattern,curpos,T("xdigit")))
                {
                    range->ranges.push_back(CharRange(T('a'),T('f')));
                    range->ranges.push_back(CharRange(T('A'),T('F')));
                    range->ranges.push_back(CharRange(T('0'),T('9')));
                }
                else
                {
                    delete range;
                    throw RegexError(ErrorCode::error_ctype);
                }
                if(!this->isStr(pattern,curpos,T(":]")))
                {
                    delete range;
                    throw RegexError(ErrorCode::error_brack);
                }
            }
            else    //char set
            {
                //isInverse
                if(this->isChar(pattern,curpos,T('^')))
                {
                    range->isInverse=true;
                }
                CharRange curRange;
                while ((unsigned int)(curpos)<pattern.length())
                {
                    if(!isChar(pattern,curpos,T(']')))
                    {
                        if(parseSeqChar(pattern,curpos,&curRange))
                        {
                            range->ranges.push_back(curRange);
                        }
                        else
                        {
                            std::vector<CharRange> charset=parseChar(pattern,curpos);
                            range->ranges.insert(range->ranges.end(),charset.begin(),charset.end());
                        }
                    }
                    else
                    {
                        break;
                    }
                    if((unsigned int)(curpos)>=pattern.length())
                    {
                        curpos=savePos;
                        delete range;
                        throw RegexError(ErrorCode::error_brack);
                    }
                }
            }
            return range;
        }
        else
        {

            //name or number capture reference
            if(this->isStr(pattern,curpos,T("\\k")))
            {
                int curpos_saver=curpos;
                if(isChar(pattern,curpos,T('\'')))
                {
                    string_t name=parseName(pattern,curpos);
                    if(isChar(pattern,curpos,T('\'')))
                    {
                        ExpNamedCaptureReference * capRef=new ExpNamedCaptureReference;
                        capRef->name=name;
                        return capRef;
                    }
                    else
                    {
                        //"\k'" error
                        curpos=curpos_saver;
                        throw RegexError(ErrorCode::error_escape);
                    }
                }
                else
                {
                    int num=getPositiveInt(pattern,curpos);
                    if(num!=-1)
                    {
                        ExpCaptureReference * capRef=new ExpCaptureReference;
                        capRef->pos=num;
                        return capRef;
                    }
                    else
                    {
                        //"\k'" error
                        curpos=curpos_saver;
                        throw RegexError(ErrorCode::error_escape);
                    }
                }
            }
            else if(this->isChar(pattern,curpos,T('^')))
            {
                if((this->flag&SyntaxFlag::MULTILINE)==0)
                {
                    ExpBeginString * range=new ExpBeginString;
                    return range;
                }
                else
                {
                    ExpBeginLine * range=new ExpBeginLine;
                    return range;
                }
            }
            else if(this->isChar(pattern,curpos,T('$')))
            {
                if((this->flag&SyntaxFlag::MULTILINE)==0)
                {
                    ExpEndString * range=new ExpEndString;
                    return range;
                }
                else
                {
                    ExpEndLine * range=new ExpEndLine;
                    return range;
                }
            }
            else
            {
                //not char and rollback
                switch(pattern[curpos])
                {
                case T('('):
                case T(')'):
                case T('+'):
                case T('*'):
                case T('?'):
                case T('{'):
                case T('}'):
                case T('|'):
                    curpos=savePos;
                    return nullptr;
                default:
                    break;
                }
                //special char set
                ExpCharRange *range=new ExpCharRange;
                //isIcase
                if((this->flag&SyntaxFlag::ICASE)!=0)
                {
                    range->icase=true;
                }
                range->ranges=parseChar(pattern,curpos);
                return range;
            }
        }
    }

    string_t AstParser::parseName(const string_t &pattern, int &curpos)
    {
        string_t result;
        while(pattern[curpos]!=T('>')&&
              pattern[curpos]!=T('\''))
        {
            result.push_back(pattern[curpos]);
            ++curpos;
        }
        return result;
    }

    bool AstParser::isChar(const string_t &pattern, int &curpos, char_t c)
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

    bool AstParser::isStr(const string_t &pattern, int &curpos, const string_t &str)
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
            curpos=pattern_postmp;
            return true;
        }
        else
        {
            return false;
        }
    }

    int AstParser::getPositiveInt(const string_t &pattern, int &curpos)
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

    int AstParser::getTokenLength(const string_t &pattern, int &curpos)
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

    char_t AstParser::getChar(const string_t &pattern, int &curpos)
    {
        if((unsigned)curpos<pattern.length())
        {
            return pattern[curpos++];
        }
        else
        {
            return 0;
        }
    }

    bool AstParser::consume(const string_t &pattern, int &curpos)
    {
        if(static_cast<unsigned int>(curpos)<pattern.length())
        {
            ++curpos;
            return true;
        }
        else
        {
            return false;
        }
    }


}
