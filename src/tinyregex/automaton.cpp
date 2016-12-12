#include "automaton.h"


namespace tyre
{
    Automaton::Automaton()
        :beginState(nullptr)
    {

    }

    Automaton::~Automaton()
    {
        for(unsigned int i=0;i<states.size();++i)
        {
            delete states[i];
        }
        for(unsigned int i=0;i<transitions.size();++i)
        {
            delete transitions[i];
        }
    }

    State *Automaton::addState()
    {
        State * newState=new State;
        newState->isEndState=false;

        states.push_back(newState);
        return newState;
    }

    Transition *Automaton::addTransition(State *start, State *end)
    {
        Transition * newTransition=new Transition;

        newTransition->source=start;
        newTransition->target=end;
        newTransition->passCounter=0;

        transitions.push_back(newTransition);
        start->out.push_back(newTransition);
        end->in.push_back(newTransition);
        return newTransition;
    }

    Transition *Automaton::addCharRange(State *start, State *end, CharRange range)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::CHARS;
        newTransition->range=range;
        return newTransition;
    }



    Transition *Automaton::addEmptyTransition(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::EMPTY;
        return newTransition;
    }

    Transition *Automaton::addLoop(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::EMPTY;
        return newTransition;
    }
    Transition *Automaton::addLoop(State *start, State *end, CharRange range)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::CHARS;
        newTransition->range=range;
        return newTransition;
    }

    Transition *Automaton::addBeginString(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::BEGINSTRING;
        return newTransition;
    }

    Transition *Automaton::addEndString(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::ENDSTRING;
        return newTransition;
    }



    Automaton Automaton::NfaSimplification(const Automaton &automaton)
    {
        Automaton result;
        std::map<State *,State *> stateMap;

    }

    bool State::match(const string_t str, int pos)
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
            case TransitionType::BEGINSTRING:
                if(pos>0)
                {
                    if(str[pos-1]==T('\n')||str[pos-1]==T('\n'))
                    {
                        if(out[i]->target->match(str,pos))
                        {
                            result = true;
                        }
                    }
                }
                else
                {
                    if(pos==0)
                    {
                        if(out[i]->target->match(str,pos))
                        {
                            result = true;
                        }
                    }
                }
                break;
            case TransitionType::ENDSTRING:
                if((unsigned int)pos<str.length())
                {
                    //有点反直觉啊。。。以后改
                    if(str[pos]==T('\n')||str[pos]==T('\n'))
                    {
                        if(out[i]->target->match(str,pos))
                        {
                            result = true;
                        }
                    }
                }
                else
                {
                    if((unsigned int)pos==str.length())
                    {
                        if(out[i]->target->match(str,pos))
                        {
                            result = true;
                        }
                    }
                }
                break;
            default:
                break;
            }
        }

        return result;
    }

}//namespace tyre

