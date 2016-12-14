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

    Transition *Automaton::addLazyTransition(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::LAZY;
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



    Automaton Automaton::NfaToDfa(const Automaton &automaton)
    {
        Automaton dfa;

        return dfa;
    }

    bool State::match(const string_t &str, int pos, MatchFlag flag)
    {
        if((flag&MatchFlag::MATCH_BFS)==0)
        {
            matchDfs(str,pos,flag);
        }
        else
        {
            matchBfs(str,pos,flag);
        }
    }

    bool State::search(const string_t &str, int pos, int *endPos, MatchFlag flag)
    {
        if((flag&MatchFlag::MATCH_BFS)==0)
        {
            return searchDfs(str,pos-1,pos,endPos,flag);
        }
        else
        {
            return searchBfs(str,pos-1,pos,endPos,flag);
        }
    }

    bool State::matchBfs(const string_t &str, int pos, MatchFlag flag)
    {
        return true;
    }

    bool State::searchBfs(const string_t &str, int acpos, int pos, int *endPos, bool isLazy, MatchFlag flag)
    {
        return true;
    }

    bool State::matchDfs(const string_t &str, int pos,MatchFlag flag)
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
                    if(out[i]->target->matchDfs(str,pos+1,flag))
                    {
                        result = true;
                    }
                }
                break;
            case TransitionType::EMPTY:
                if(out[i]->target->matchDfs(str,pos,flag))
                {
                    result = true;
                }
                break;
            case TransitionType::BEGINSTRING:
                if(pos>0)
                {
                    if(str[pos-1]==T('\n')||str[pos-1]==T('\n'))
                    {
                        if(out[i]->target->matchDfs(str,pos,flag))
                        {
                            result = true;
                        }
                    }
                }
                else
                {
                    if(pos==0)
                    {
                        if(out[i]->target->matchDfs(str,pos,flag))
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
                        if(out[i]->target->matchDfs(str,pos,flag))
                        {
                            result = true;
                        }
                    }
                }
                else
                {
                    if((unsigned int)pos==str.length())
                    {
                        if(out[i]->target->matchDfs(str,pos,flag))
                        {
                            result = true;
                        }
                    }
                }
                break;
            default:
                break;
            }
            if(result==true)
            {
                break;
            }
        }
        return result;
    }

    bool State::searchDfs(const string_t &str,int acpos, int pos, int * endPos,bool isLazy,MatchFlag flag)
    {
        bool result(false);
        if((unsigned int)pos>str.length())
        {
            return false;
        }
        if(isEndState==true&&acpos>=0)
        {
            if(isLazy==true)
            {
                *endPos=acpos;
                return true;
            }
            else
            {
                if(*endPos<acpos)
                    *endPos=acpos;
                result=true;
            }
        }
        for(unsigned int i=0;i<out.size();++i)
        {
            switch(out[i]->type)
            {
            case TransitionType::CHARS:
                if(out[i]->range.isSubSet(str[pos]))
                {
                    if(out[i]->target->searchDfs(str,acpos+1,pos+1,endPos,isLazy,flag))
                    {
                        result = true;
                    }

                }
                break;
            case TransitionType::EMPTY:
                if(out[i]->target->searchDfs(str,acpos,pos,endPos,isLazy,flag))
                {
                    result = true;
                }
                break;
            case TransitionType::BEGINSTRING:
                if(pos>0)
                {
                    if(str[pos-1]==T('\n')||str[pos-1]==T('\n'))
                    {
                        if(out[i]->target->searchDfs(str,acpos,pos,endPos,isLazy,flag))
                        {
                            result = true;
                        }
                    }
                }
                else
                {
                    if(pos==0)
                    {
                        if(out[i]->target->searchDfs(str,acpos,pos,endPos,isLazy,flag))
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
                        if(out[i]->target->searchDfs(str,acpos,pos,endPos,isLazy,flag))
                        {
                            result = true;
                        }
                    }
                }
                else
                {
                    if((unsigned int)pos==str.length())
                    {
                        if(out[i]->target->searchDfs(str,acpos,pos,endPos,isLazy,flag))
                        {
                            result = true;
                        }
                    }
                }
                break;
            case TransitionType::LAZY:
                if(out[i]->target->searchDfs(str,acpos,pos,endPos,true,flag))
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

}//namespace tyre

