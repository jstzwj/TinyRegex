#ifndef ASTNODE_H
#define ASTNODE_H
#include<vector>
#include"base.h"
namespace tyre
{
    class ExpBase
    {
    public:
        ExpBase(){}
        virtual void release(){}
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
        virtual ~ExpFunction(){}
    };
    class ExpUnit:public ExpBase
    {
    public:
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
        virtual ~ExpUnit(){}
    };
    //表示一个字符串的范围
    class Range
    {
    public:
        Range(){}
        Range(char_t begin,char_t end,bool inverse=false)
            :charBegin(begin),charEnd(end),isInverse(inverse) {}
        char_t charBegin;
        char_t charEnd;
        bool isInverse;
    };

    class ExpCharRange:public ExpBase
    {
    public:
        std::vector<Range> rangles;
        virtual void release(){delete this;}
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
        virtual ~ExpLoop(){}
    };

}


#endif // ASTNODE_H
