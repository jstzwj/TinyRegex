#ifndef AUTOMATON_H
#define AUTOMATON_H
#include<vector>
#include<list>
#include<map>
#include"base.h"
#include"regexresult.h"
#include"attribute.h"
#include"regexerror.h"

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
        BEGINCAPTURE,
        ENDCAPTURE,
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

        bool isSubSet(char_t element)
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
            :source(nullptr),target(nullptr){}
        Transition(State * src,State * tar)
            :source(src),target(tar){}
        Transition(State * src,State * tar,const CharRange &ran)
            :source(src),target(tar),range(ran){}
        Transition(const Transition &)=delete;

        State * source;
        State * target;
        CharRange range;
        int passCounter;
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
        Transition *addBeginCapture(State *start, State *end);
        Transition *addEndCapture(State *start, State *end);
        //static member functions
        static Automaton NfaToDfa(const Automaton &automaton);
    };
}


#endif // AUTOMATON_H
