#ifndef ASTNODE_H
#define ASTNODE_H
#include<vector>
#include"base.h"
#include"nfagraph.h"
#include"automaton.h"


namespace tyre
{
    /**
     * @brief The ExpBase class
     * @date 2016-12-9
     * @author wangjun
     * @details This class is the base class of every abstract syntax tree node.
     */
    class ExpBase
    {
    public:
        /**
         * @brief ExpBase constructor
         * @details Empty function
         */
        ExpBase(){}
        /**
         * @brief Release memory
         * @details The method is used to release the memory of the objects implemented from the class.
         *          The abstract method will release pointers which they owned recursively.
         * @deprecated Because the deconstructor will release automatically, calling it manually is not a good way.
         */
        virtual void release()=0;
        /**
         * @brief generate automaton
         * @param A pointer referenced to a graph(class Automaton)
         * @return a NfaGraph object,related to the graph.
         */
        virtual void generate(Automaton * graph,NfaGraph result)=0;
        /**
         * @brief ~ExpBase
         * @details It is used to release the memory.The abstract method will release pointers which they owned recursively.
         */
        virtual ~ExpBase(){}
    };
    /**
     * @brief The ExpOr class
     * @date 2016-12-9
     * @author wangjun
     * @details This class represents the operator '|'.
     */
    class ExpOr:public ExpBase
    {
    public:
        /**
         * @brief ExpOr
         * @inherits ExpBase
         * @details Init the left and right pointer with nullptr.
         */
        ExpOr():ExpBase(),left(nullptr),right(nullptr){}
        /**
         * @brief left and right
         * @details represent two parts beside the operator '|'
         */
        ExpBase * left;
        ExpBase * right;
        /**
         * @brief release
         * @inherits
         */
        virtual void release()
        {
            if(left!=nullptr)
            {
                left->release();
                left=nullptr;
            }
            if(right!=nullptr)
            {
                right->release();
                right=nullptr;
            }
            delete this;
        }
        /**
         * @brief generate
         * @inherits
         */
        virtual void generate(Automaton * graph,NfaGraph result)
        {
            left->generate(graph,result);
            right->generate(graph,result);
        }
        /**
         * @brief ~ExpOr
         * @inherits
         */
        virtual ~ExpOr()
        {
            if(left!=nullptr)
                delete left;
            if(right!=nullptr)
                delete right;
        }
    };
    /**
     * @brief The ExpAnd class
     * @date 2016-12-10
     * @author wangjun
     * @details This class represents the invisible operator '&', which is used to connect two characters.
     */
    class ExpAnd:public ExpBase
    {
    public:
        ExpAnd():ExpBase(),left(nullptr),right(nullptr){}
        ExpBase * left;
        ExpBase * right;
        virtual void release()
        {
            if(left!=nullptr)
            {
                left->release();
                left=nullptr;
            }
            if(right!=nullptr)
            {
                right->release();
                right=nullptr;
            }
            delete this;
        }
        virtual void generate(Automaton * graph,NfaGraph result)
        {
            NfaGraph leftGraph,rightGraph;
            State * midState=graph->addState();

            leftGraph.begin=result.begin;
            leftGraph.end=midState;
            left->generate(graph,leftGraph);

            rightGraph.begin=midState;
            rightGraph.end=result.end;
            right->generate(graph,rightGraph);

        }
        virtual ~ExpAnd()
        {
            if(left!=nullptr)
                delete left;
            if(right!=nullptr)
                delete right;
        }
    };
    /**
    *函数种类：
    *
    */
    enum FunctionType
    {
        CAPTURE,
        NAMEDCAPTURE,
        NOCAPTURE,
        FOREWARD_POSITIVE_CAPTURE,
        FOREWARD_NEGATIVE_CAPTURE,
        BACKWARD_POSITIVE_CAPTURE,
        BACKWARD_NEGATIVE_CAPTURE
    };
    /**
     * @brief The ExpFunction class
     * 函数节点
     *
     */
    class ExpFunction:public ExpBase
    {
    public:
        ExpFunction():ExpBase(),subexp(nullptr){}
        ExpBase * subexp;
        string_t name;
        FunctionType type;
        virtual void release()
        {
            if(subexp!=nullptr)
            {
                subexp->release();
                subexp=nullptr;
            }
            delete this;
        }
        virtual void generate(Automaton * graph,NfaGraph result)
        {
            //以后在这里添加捕获功能
            if(type==FunctionType::CAPTURE)
            {
                State * firstMidState=graph->addState();
                State * secondMidState=graph->addState();
                graph->addBeginCapture(result.begin,firstMidState);
                subexp->generate(graph,NfaGraph(firstMidState,secondMidState));
                graph->addEndCapture(secondMidState,result.end);
            }
            else if(type==FunctionType::NAMEDCAPTURE)
            {
                State * firstMidState=graph->addState();
                State * secondMidState=graph->addState();
                graph->addBeginNamedCapture(result.begin,firstMidState,name);
                subexp->generate(graph,NfaGraph(firstMidState,secondMidState));
                graph->addEndNamedCapture(secondMidState,result.end,name);
            }
            else if(type==FunctionType::NOCAPTURE)
            {
                subexp->generate(graph,result);
            }
            else
            {

            }

        }
        virtual ~ExpFunction()
        {
            if(subexp!=nullptr)
                delete subexp;
        }
    };
    /*
    class ExpUnit:public ExpBase
    {
    public:
        //str不使用
        ExpUnit():ExpBase(),str(nullptr),loop(nullptr){}
        ExpBase * str;
        ExpBase * loop;
        virtual void release()
        {
            if(str!=nullptr)
                str->release();
            if(loop!=nullptr)
                loop->release();
            delete this;
        }
        virtual NfaGraph generate(Automaton * graph)
        {
            NfaGraph result;

            result=loop->generate(graph);
            return result;
        }
        virtual ~ExpUnit(){}
    };
    */

    class ExpCharRange:public ExpBase
    {
    public:
        ExpCharRange():isInverse(false),icase(false){}
        std::vector<CharRange> rangles;
        bool isInverse;
        bool icase;
        virtual void release(){delete this;}
        virtual void generate(Automaton * graph,NfaGraph result)
        {
            //加入集合划分
            for(unsigned int i=0;i<rangles.size();++i)
            {
                if(icase==true)
                {
                    std::vector<CharRange> caseRange;
                    caseRange=rangles[i].interSection(T('A'),T('Z'));
                    for(unsigned i=0;i<caseRange.size();++i)
                    {
                        caseRange[i].toLower();
                        graph->addCharRange(result.begin,result.end,caseRange[i]);
                    }
                    caseRange=rangles[i].interSection(T('a'),T('z'));
                    for(unsigned i=0;i<caseRange.size();++i)
                    {
                        caseRange[i].toUpper();
                        graph->addCharRange(result.begin,result.end,caseRange[i]);
                    }
                }
                graph->addCharRange(result.begin,result.end,rangles[i]);
            }
        }
        virtual ~ExpCharRange(){}
    };
    class ExpBeginString:public ExpBase
    {
    public:
        ExpBeginString(){}
        virtual void release(){delete this;}
        virtual void generate(Automaton * graph,NfaGraph result)
        {
            graph->addBeginString(result.begin,result.end);
        }
        virtual ~ExpBeginString(){}
    };
    class ExpEndString:public ExpBase
    {
    public:
        ExpEndString(){}
        virtual void release(){delete this;}
        virtual void generate(Automaton * graph,NfaGraph result)
        {
            graph->addEndString(result.begin,result.end);
        }
        virtual ~ExpEndString(){}
    };
    class ExpBeginLine:public ExpBase
    {
    public:
        ExpBeginLine(){}
        virtual void release(){delete this;}
        virtual void generate(Automaton * graph,NfaGraph result)
        {
            graph->addBeginLine(result.begin,result.end);
        }
        virtual ~ExpBeginLine(){}
    };
    class ExpEndLine:public ExpBase
    {
    public:
        ExpEndLine(){}
        virtual void release(){delete this;}
        virtual void generate(Automaton * graph,NfaGraph result)
        {
            graph->addEndLine(result.begin,result.end);
        }
        virtual ~ExpEndLine(){}
    };
    class ExpCaptureReference:public ExpBase
    {
    public:
        ExpCaptureReference(){}
        int pos;
        virtual void release(){delete this;}
        virtual void generate(Automaton * graph,NfaGraph result)
        {
            graph->addCaptureReference(result.begin,result.end,pos);
        }
        virtual ~ExpCaptureReference(){}
    };
    class ExpNamedCaptureReference:public ExpBase
    {
    public:
        ExpNamedCaptureReference(){}
        string_t name;
        virtual void release(){delete this;}
        virtual void generate(Automaton * graph,NfaGraph result)
        {
            graph->addCaptureReference(result.begin,result.end,name);
        }
        virtual ~ExpNamedCaptureReference(){}
    };
    class ExpLoop:public ExpBase
    {
    public:
        ExpLoop():ExpBase(),lazy(false),exp(nullptr){}
        int min;
        int max;
        bool lazy;
        ExpBase * exp;
        virtual void release()
        {
            if(exp!=nullptr)
            {
                exp->release();
                exp=nullptr;
            }
            delete this;
        }
        void generateMustLoop(Automaton *graph, NfaGraph result)
        {
            State *beginState(result.begin);
            State *midState(nullptr);
            if(min==0)
            {
                graph->addEmptyTransition(result.begin,result.end);
            }
            for(int i=0;i<min;++i)
            {
                if(i==min-1)
                {
                    midState=result.end;
                }
                else
                {
                    midState=graph->addState();
                }
                exp->generate(graph,NfaGraph(beginState,midState));
                beginState=midState;
            }
        }
        void generateAltLoop(Automaton *graph, NfaGraph result)
        {
            State *beginState(result.begin);
            State *midState(nullptr);
            for(int i=min;i<max;++i)
            {
                if(i==max-1)
                    midState=result.end;
                else
                    midState=graph->addState();
                exp->generate(graph,NfaGraph(beginState,midState));
                graph->addEmptyTransition(beginState,result.end);
                beginState=midState;
            }
        }
        void generateInfiniteLoop(Automaton *graph, NfaGraph result)
        {
            //graph->addEmptyTransition(result.begin,result.end);
            exp->generate(graph,NfaGraph(result.end,result.begin));
        }
        virtual void generate(Automaton * graph,NfaGraph result)
        {
            if(max==-1)
            {
                if(this->lazy==true)
                {
                    State *lazyState=graph->addState();
                    this->generateMustLoop(graph,NfaGraph(result.begin,lazyState));
                    graph->addLazyTransition(lazyState,result.end);
                    this->generateInfiniteLoop(graph,NfaGraph(result.end,result.end));
                }
                else
                {
                    this->generateMustLoop(graph,NfaGraph(result.begin,result.end));
                    this->generateInfiniteLoop(graph,NfaGraph(result.end,result.end));
                }
            }
            else
            {
                State* midState=graph->addState();
                this->generateMustLoop(graph,NfaGraph(result.begin,midState));

                if(this->lazy==true)
                {
                    State *lazyState=graph->addState();
                    graph->addLazyTransition(midState,lazyState);
                    midState=lazyState;
                }
                this->generateAltLoop(graph,NfaGraph(midState,result.end));
            }
        }
        virtual ~ExpLoop()
        {
            if(exp!=nullptr)
                delete exp;
        }
    };

}


#endif // ASTNODE_H
