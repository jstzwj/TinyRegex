#ifndef AUTOMATON_H
#define AUTOMATON_H
#include<vector>
#include<list>
#include<map>
#include"base.h"
#include"regexresult.h"
#include"attribute.h"
#include"regexerror.h"
#include"stringutilities.h"

namespace tyre
{

    class NfaGraph;
    class Automaton;
    class State;
    class Transition;
    enum TransitionType
    {
        CHARS,
        EMPTY,
        LAZY,
        BEGINSTRING,
        ENDSTRING,
        BEGINLINE,
        ENDLINE,
        BEGINCAPTURE,
        ENDCAPTURE,
        CaptureReference,
        NameCaptureReference,
        END
    };
    /**
     * @brief The CharRange class
     * @date 2016-12-10
     * @author wangjun
     * @details This class represent a range of character.
     */
    class CharRange
    {
    public:
        /**
         * @brief CharRange
         * @details Initialize charBegin,charEnd and isInverse.
         */
        CharRange()
            :charBegin(0),charEnd(0),isInverse(false){}
        /**
         * @brief CharRange
         * @param begin --The begin of character range.The range contains it.
         * @param end   --The end of character range.The range contains it.
         * @param inverse
         * @details This constructor function provides the params for charBegin and charEnd.IsInverse is a default param.
         */
        CharRange(char_t begin,char_t end,bool inverse=false)
            :charBegin(begin),charEnd(end),isInverse(inverse) {}

        char_t charBegin;
        char_t charEnd;
        bool isInverse;
        bool isElement(char_t element)
        {
            bool result(false);
            if(element>=charBegin&&element<=charEnd)
            {
                result=true;
            }
            else
            {
                result=false;
            }
            if(isInverse==true)
            {
                result=!result;
            }
            return result;
        }
        //we assume that charEnd is bigger than charBegin
        std::vector<CharRange> interSection(char_t begin,char_t end)
        {
            std::vector<CharRange> result;
            if(isInverse)
            {
                //empty intersection
                if(begin>charBegin&&end<charEnd)
                {
                    return result;
                }
                if(begin<=charBegin)
                {
                    if(end>=charEnd)
                    {
                        result.push_back(CharRange(begin,charBegin));
                        result.push_back(CharRange(charEnd,end));
                    }
                    else
                    {
                        result.push_back(CharRange(begin,charBegin));
                    }
                }
                else
                {
                    if(end>=charEnd)
                    {
                        result.push_back(CharRange(charEnd,end));
                    }
                }
            }
            else
            {
                //empty intersection
                if(end<charBegin||begin>charEnd)
                {
                    return result;
                }
                char_t b,e;
                if(begin<charBegin)
                {
                    b=charBegin;
                }
                else
                {
                    b=begin;
                }
                if(end>charEnd)
                {
                    e=charEnd;
                }
                else
                {
                    e=end;
                }
                result.push_back(CharRange(b,e));
            }
            return result;
        }

        bool isSubSet(char_t begin,char_t end)
        {
            if(isInverse)
            {
                if(begin>charEnd||end<charBegin)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if(begin>=charBegin&&end<=charEnd)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        bool isSubSet(char_t element,bool icase=false)
        {
            if(icase==true&&element>=T('a')&&element<=T('z'))
            {
                return isElement(StringUtilities::toUpper(element))||isElement(element);
            }
            else if(icase==true&&element>=T('A')&&element<=T('Z'))
            {
                return isElement(StringUtilities::toLower(element))||isElement(element);
            }
            else
            {
                return isElement(element);
            }
        }
        void toUpper()
        {
            charBegin-=T('a')-T('A');
            charEnd-=T('a')-T('A');
        }
        void toLower()
        {
            charBegin+=T('a')-T('A');
            charEnd+=T('a')-T('A');
        }
    };
    /**
     * @brief The LoopRange class
     * @date 2016-12-10
     * @author wangjun
     * @details This class represent a range of loop.
     */
    class LoopRange
    {
    public:
        LoopRange(){}
        LoopRange(int begin,int end,bool inverse=false)
            :loopBegin(begin),loopEnd(end),isInverse(inverse) {}
        LoopRange(const LoopRange &)=delete;

        int loopBegin;
        int loopEnd;
        bool isInverse;

        bool isSubSet(int element)
        {
            bool result(false);
            if(element>=loopBegin&&element<=loopEnd)
            {
                result=true;
            }
            else if(element>=loopBegin&&loopEnd==-1)
            {
                result=true;
            }
            else
            {
                result=false;
            }
            if(isInverse==true)
            {
                result=!result;
            }
            return result;
        }
    };


    class Transition
    {
    public:
        Transition()
            :source(nullptr),target(nullptr),range(),captureNum(0),captureName(nullptr){}
        Transition(State * src,State * tar)
            :source(src),target(tar),range(),captureNum(0),captureName(nullptr){}
        Transition(State * src,State * tar,const CharRange &ran)
            :source(src),target(tar),range(ran),captureNum(0),captureName(nullptr){}
        Transition(const Transition &)=delete;

        ~Transition(){if(captureName!=nullptr)delete captureName;}

        State * source;
        State * target;
        CharRange range;

        int captureNum;
        string_t * captureName;

        TransitionType type;
    };
    /**
     * @brief The StateFrame class
     * @attention The class have not been used.
     */
    class StateFrame
    {
    public:
        StateFrame(State * sta,int num)
            :state(sta),outNum(num){}
        State * state;
        int outNum;
    };

    class State
    {
    public:
        std::vector<Transition *> out;
        std::vector<Transition *> in;
        bool isEndState;
        //int edgeLock;

        State()
            :out(),in(),isEndState(false){}
        State(const State &)=delete;

        bool match(const string_t &str, int pos,MatchFlag flag=MatchFlag::MATCH_DEFAULT);
        bool search(const string_t &str, int pos, RegexSubMatch &smatch,MatchFlag flag=MatchFlag::MATCH_DEFAULT);
    private:
        bool matchDfs(const string_t &str, int pos,MatchFlag flag=MatchFlag::MATCH_DEFAULT);
        bool matchBfs(const string_t &str, int pos,MatchFlag flag=MatchFlag::MATCH_DEFAULT);
        bool searchDfs(const string_t &str, int beginpos, int acpos, int pos, RegexSubMatch &smatch, bool isLazy, MatchFlag flag=MatchFlag::MATCH_DEFAULT);
        bool searchBfs(const string_t &str, int acpos, int pos, RegexSubMatch &smatch, bool isLazy, MatchFlag flag=MatchFlag::MATCH_DEFAULT);
    };
    class Automaton
    {
    public:
        Automaton();
        ~Automaton();
        //Automaton(const Automaton &);
        std::vector<State *> states;
        std::vector<Transition *> transitions;
        State * beginState;
        //interfaces
        State * addState();
        Transition *addCharRange(State * start,State * end,CharRange range);
        Transition *addTransition(State * start,State * end);
        Transition *addEmptyTransition(State * start,State * end);
        Transition *addLazyTransition(State * start,State * end);
        Transition *addLoop(State * start, State * end);//empty
        Transition *addLoop(State *start, State *end, CharRange range);
        Transition *addBeginString(State *start, State *end);
        Transition *addEndString(State *start, State *end);
        Transition *addBeginLine(State *start, State *end);
        Transition *addEndLine(State *start, State *end);
        Transition *addBeginCapture(State *start, State *end);
        Transition *addEndCapture(State *start, State *end);
        Transition *addCaptureReference(State *start, State *end,int pos);//reference
        Transition *addCaptureReference(State *start, State *end, const string_t &name);//named reference
        //static member functions
        static Automaton NfaToDfa(const Automaton &automaton);
    };
}


#endif // AUTOMATON_H
