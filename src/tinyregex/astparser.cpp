#include "astparser.h"
namespace tyre
{

    AstParser::AstParser()
    {

    }

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
                curpos=savePos;
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

        return left;
    }

    ExpBase *AstParser::parseUnit(const string_t &pattern, int &curpos)
    {
        int savePos=curpos;
        ExpBase * function=nullptr;
        function=this->parseCharSet(pattern,curpos);
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
            ExpBase * exp=this->parseExp(pattern,curpos);
            if(exp!=nullptr)
            {
                if(isChar(pattern,curpos,T(')'))==true)
                {
                    ExpFunction * function=new ExpFunction;
                    function->subexp=exp;
                    function->type=FunctionType::CAPTURE;
                    return function;
                }
                else
                {
                    throw T("error: expected right bracket after \'")+
                        pattern.substr(savePos,this->getTokenLength(pattern,savePos))+
                        T("\'\r\nposition: ")+toString(curpos);
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
            return loop;
        }
        else if(isChar(pattern,curpos,T('+')))
        {
            loop=new ExpLoop;
            loop->min=1;
            loop->max=-1;
            return loop;
        }
        else if(isChar(pattern,curpos,T('?')))
        {
            loop=new ExpLoop;
            loop->min=0;
            loop->max=1;
            return loop;
        }
        else if(isChar(pattern,curpos,T('[')))
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
            if(!isChar(pattern,curpos,T(']')))
            {
                throw T("error: expected right bracket near \'")+
                        toString(pattern[curpos])+T("\'\r\nposition: ")+toString(curpos);
            }
        }
        return nullptr;
    }

    ExpBase *AstParser::parseCharSet(const string_t &pattern, int &curpos)
    {
        int savePos=curpos;
        if((unsigned int)curpos>=pattern.length())
        {
            return nullptr;
        }
        if(this->isChar(pattern,curpos,T('[')))
        {
            //暂时不做
            return nullptr;
        }
        else
        {
            if(this->isChar(pattern,curpos,T('\\')))
            {
                ExpCharRange * range=new ExpCharRange;
                switch(pattern[curpos])
                {
                //escape sequence
                case T('r'):
                    range->rangles.push_back(Range(T('\r'),T('\r')));
                    break;
                case T('t'):
                    range->rangles.push_back(Range(T('\t'),T('\t')));
                    break;
                case T('n'):
                    range->rangles.push_back(Range(T('\n'),T('\n')));
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
                    range->rangles.push_back(Range(pattern[curpos],pattern[curpos]));
                    break;
                //real ordinary charset
                case T('s'):
                    range->rangles.push_back(Range(T('\r'),T('\r')));
                    range->rangles.push_back(Range(T('\t'),T('\t')));
                    range->rangles.push_back(Range(T('\n'),T('\n')));
                    break;
                case T('S'):
                    range->rangles.push_back(Range(T('\r'),T('\r'),true));
                    range->rangles.push_back(Range(T('\t'),T('\t'),true));
                    range->rangles.push_back(Range(T('\n'),T('\n'),true));
                    break;
                case T('d'):
                    range->rangles.push_back(Range(T('0'),T('9')));
                    break;
                case T('D'):
                    range->rangles.push_back(Range(T('0'),T('9'),true));
                    break;
                default:
                    range->rangles.push_back(Range(pattern[curpos],pattern[curpos],false));
                }
                return range;
            }
            else
            {
                ExpCharRange * range=new ExpCharRange;
                range->rangles.push_back(Range(pattern[curpos],pattern[curpos],false));
                ++curpos;
                return range;
            }
        }
    }


}
