#ifndef ASTNODE_H
#define ASTNODE_H
#include<vector>
#include"base.h"
#include"automaton.h"
#include"nfagraph.h"
namespace tyre
{
    class ExpBase
    {
    public:
        ExpBase(){}
        virtual void release(){}
        virtual NfaGraph generate(Automaton * graph)=0;
        virtual ~ExpBase(){}
    };
    class ExpOr:public ExpBase
    {
    public:
        ExpOr():ExpBase(),left(nullptr),right(nullptr){}
        ExpBase * left;
        ExpBase * right;
        virtual void release()
        {
            if(left!=nullptr)
                left->release();
            if(right!=nullptr)
                right->release();
            delete this;
        }

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

        virtual ~ExpOr(){}
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
                left->release();
            if(right!=nullptr)
                right->release();
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
        virtual ~ExpAnd(){}
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
                subexp->release();
            delete this;
        }
        virtual NfaGraph generate(Automaton * graph)
        {
            NfaGraph result;

            result=subexp->generate(graph);

            return result;
        }
        virtual ~ExpFunction(){}
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
        std::vector<Range> rangles;
        virtual void release(){delete this;}
        virtual NfaGraph generate(Automaton * graph)
        {
            NfaGraph result;
            result.begin=graph->addState();
            result.end=graph->addState();
            //以后加入集合划分
            graph->addCharRange(result.begin,result.end,*rangles.begin());
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
                exp->release();
            delete this;
        }
        virtual NfaGraph generate(Automaton * graph)
        {
            NfaGraph result;
            result=exp->generate(graph);
            graph->addLoop(result.begin,result.end,min,max);
            return result;
        }
        virtual ~ExpLoop(){}
    };

}


#endif // ASTNODE_H
