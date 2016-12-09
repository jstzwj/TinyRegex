#ifndef AUTOMATON_H
#define AUTOMATON_H
#include<vector>
#include"base.h"

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
        LOOP,
        END
    };
    //表示一个字符串的范围
    class CharRange
    {
    public:
        CharRange(){}
        CharRange(char_t begin,char_t end,bool inverse=false)
            :charBegin(begin),charEnd(end),isInverse(inverse) {}
        char_t charBegin;
        char_t charEnd;
        bool isInverse;

        bool isSubSet(int element)
        {
            bool result;
            if(element>=charBegin&&element<=charEnd)
            {
                result=true;
            }
            if(isInverse==true)
            {
                result=!result;
            }
            return result;
        }
    };
    class LoopRange
    {
    public:
        LoopRange(){}
        LoopRange(int begin,int end,bool inverse=false)
            :loopBegin(begin),loopEnd(end),isInverse(inverse) {}
        int loopBegin;
        int loopEnd;
        bool isInverse;

        bool isSubSet(int element)
        {
            bool result;
            if(element>=loopBegin&&element<=loopEnd)
            {
                result=true;
            }
            else if(element>=loopBegin&&loopEnd==-1)
            {
                result=true;
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
        State * source;
        State * target;
        CharRange range;
        int passCounter;
        TransitionType type;
    };
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
        int edgeLock;

        bool match(const string_t str,int pos)
        {
            bool result(false);
            if((unsigned int)pos==str.length())
            {
                if(isEndState==true)
                {
                    return true;
                }
            }
            if((unsigned int)pos>str.length())
            {
                return false;
            }
            for(unsigned int i=0;i<out.size();++i)
            {
                switch(out[i]->type)
                {
                case TransitionType::CHARS:
                    if(out[i]->range.isSubSet(str[pos])||
                            out[i]->type==TransitionType::EMPTY)
                    {
                        if(out[i]->target->match(str,pos+1))
                        {
                            result = true;
                        }
                    }
                    break;
                case TransitionType::EMPTY:
                    if(out[i]->target->match(str,pos))
                    {
                        result = true;
                    }
                    break;
                default:
                    break;
                }
            }

            return result;
        }
    };
    class Automaton
    {
    public:
        Automaton();
        std::vector<State *> states;
        std::vector<Transition *> transitions;
        //interfaces
        State * addState();
        Transition *addCharRange(State * start,State * end,CharRange range);
        Transition *addTransition(State * start,State * end);
        Transition *addEmptyTransition(State * start,State * end);
        Transition *addLoop(State * start, State * end);//empty
        Transition *addLoop(State *start, State *end, CharRange range);
    };
}


#endif // AUTOMATON_H
