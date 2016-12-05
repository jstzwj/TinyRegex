#ifndef AUTOMATON_H
#define AUTOMATON_H
#include<vector>
#include"base.h"
namespace tyre
{
    enum TransitionType
    {
        CHARS,
        EMPTY,
        LOOP,
        END
    };
    //表示一个字符串的范围
    class Range
    {
    public:
        Range(){}
        Range(char_t begin,char_t end,bool inverse=false)
            :charBegin(begin),charEnd(end),isInverse(inverse) {}
        Range(int begin,int end,bool inverse=false)
            :charBegin(begin),charEnd(end),isInverse(inverse) {}
        int charBegin;
        int charEnd;
        bool isInverse;
        bool isSubSet(int element)
        {
            bool result;
            if(element>=charBegin||element<=charEnd)
            {
                if(charEnd==-1)
                {
                    result=true;
                }
                else
                {
                    result=false;
                }
            }
            if(isInverse==true)
            {
                result=!result;
            }
            return result;
        }
    };
    class State;
    class Transition
    {
    public:
        Transition()
            :source(nullptr),target(nullptr){}
        Transition(State * src,State * tar)
            :source(src),target(tar){}
        Transition(State * src,State * tar,const Range &ran)
            :source(src),target(tar),range(ran){}
        State * source;
        State * target;
        Range range;
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
        bool end;
    };
    class Automaton
    {
    public:
        Automaton();
        std::vector<State *> states;
        std::vector<Transition *> transitions;
        //interfaces
        State * addState();
        Transition *addCharRange(State * start,State * end,Range range);
        Transition *addTransition(State * start,State * end);
        Transition *addEmptyTransition(State * start,State * end);
        Transition *addLoop(State * start,State * end,int beginLoop,int endLoop);
    };
}


#endif // AUTOMATON_H
