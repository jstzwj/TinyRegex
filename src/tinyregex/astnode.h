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
        virtual NfaGraph generate(Automaton * graph)=0;
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
     * @details
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
        virtual NfaGraph generate(Automaton * graph)
        {
            NfaGraph leftGraph,rightGraph,result;

            result.begin=graph->addState();
            result.end=graph->addState();

            leftGraph=left->generate(graph);
            rightGraph=right->generate(graph);

            graph->addEmptyTransition(result.begin,leftGraph.begin);
            graph->addEmptyTransition(result.begin,rightGraph.begin);

            graph->addEmptyTransition(leftGraph.end,result.end);
            graph->addEmptyTransition(rightGraph.end,result.end);

            return result;
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
        virtual NfaGraph generate(Automaton * graph)
        {
            NfaGraph leftGraph,rightGraph,result;

            leftGraph=left->generate(graph);
            rightGraph=right->generate(graph);

            graph->addEmptyTransition(leftGraph.end,rightGraph.begin);

            result.begin=leftGraph.begin;
            result.end=rightGraph.end;
            return result;
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
        virtual NfaGraph generate(Automaton * graph)
        {
            NfaGraph result;

            result=subexp->generate(graph);

            return result;
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
        ExpCharRange():isInverse(false){}
        std::vector<CharRange> rangles;
        bool isInverse;
        virtual void release(){delete this;}
        virtual NfaGraph generate(Automaton * graph)
        {
            NfaGraph result;
            result.begin=graph->addState();
            result.end=graph->addState();
            //加入集合划分
            for(unsigned int i=0;i<rangles.size();++i)
            {
                graph->addCharRange(result.begin,result.end,rangles[i]);
            }
            return result;
        }
        virtual ~ExpCharRange(){}
    };
    class ExpLoop:public ExpBase
    {
    public:
        ExpLoop():ExpBase(),exp(nullptr){}
        int min;
        int max;
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
        virtual NfaGraph generate(Automaton * graph)
        {
            NfaGraph result;
            if(min>=1)
            {
                result=exp->generate(graph);
                for(int i=1;i<min;++i)
                {
                    NfaGraph copy=exp->generate(graph);
                    graph->addEmptyTransition(result.end,copy.begin);
                    result.end=copy.end;
                }
            }
            else
            {
                State *emptyNode=graph->addState();
                result.begin=emptyNode;
                result.end=emptyNode;
            }


            if(max==-1)
            {
                NfaGraph infiniteLoop=exp->generate(graph);
                graph->addEmptyTransition(result.end,infiniteLoop.begin);
                graph->addEmptyTransition(infiniteLoop.end,result.end);
            }
            else
            {
                State *tmpEnd=graph->addState();
                for(int i=min;i<max;++i)
                {
                    NfaGraph copy=exp->generate(graph);
                    graph->addEmptyTransition(copy.begin,tmpEnd);
                    graph->addEmptyTransition(result.end,copy.begin);
                    result.end=copy.end;
                }
                graph->addEmptyTransition(result.end,tmpEnd);
                result.end=tmpEnd;
            }
            //graph->addLoop(result.begin,result.end);
            return result;
        }
        virtual ~ExpLoop()
        {
            if(exp!=nullptr)
                delete exp;
        }
    };

}


#endif // ASTNODE_H
