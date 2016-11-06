#pragma once
#include"tr_type.h"
#include"TinyResult.h"
#include<memory>
namespace tinyregex
{
	//表示一个连续的字符集合
	class CharRange
	{
	public:
		CharRange() 
		:start(0),end(0){}
		CharRange(char_t in_start, char_t in_end) 
		:start(in_start),end(in_end){}
		char_t start;
		char_t end;
	};
	//表示重复次数
	class LoopRange
	{
	public:
		LoopRange() 
		:start(0),end(0){}
		LoopRange(int in_start, int in_end)
			:start(in_start), end(in_end) {}
		int start;
		int end;
	};
	//ast节点
	typedef std::shared_ptr<ExpBase> ExpPtr;
	class ExpBase
	{
	public:
		ExpBase() {}
		virtual ~ExpBase() = 0;
		virtual void release()=0;

	};
	class AnyChar :public ExpBase
	{
	public:
		AnyChar() :ExpBase() {}
		virtual ~AnyChar() {}
		virtual void release()
		{
			delete this;
		}

	};
	class Char :public AnyChar
	{
	public:
		Char():AnyChar(){}
		virtual ~Char() {}
		virtual void release()
		{
			delete this;
		}

		char_t c;
	};

	class SeqChar :public AnyChar
	{
	public:
		SeqChar() :AnyChar() {}
		virtual ~SeqChar() {}
		virtual void release()
		{
			delete this;
		}

		CharRange range;
	};

	class CharSet:public AnyChar
	{
	public:
		CharSet() :AnyChar() {}
		virtual ~CharSet() {}
		virtual void release()
		{
			delete this;
		}

		bool isNot;
		std::vector<CharRange> range;
	};

	class Loop :public ExpBase
	{
	public:
		Loop() :ExpBase() {}
		virtual ~Loop() {}
		virtual void release()
		{
			delete this;
		}

		LoopRange range;
		ExpPtr left;	//单目
	};


	class Number :public ExpBase
	{
	public:
		Number() :ExpBase() {}
		virtual ~Number() {}
		virtual void release()
		{
			delete this;
		}

		int num;
	};
	
	class ExpOp:public ExpBase
	{
	public:
		ExpOp() {}
		virtual ~ExpOp() = 0;
		virtual void release() = 0;

	};

	class OpAnd :public ExpOp
	{
	public:
		OpAnd() :ExpOp() {}
		virtual ~OpAnd() {}
		virtual void release()
		{
			left->release();
			right->release();
			delete this;
		}
		ExpPtr left;
		ExpPtr right;
	};

	class OpOr :public ExpOp
	{
	public:
		OpOr() :ExpOp() {}
		virtual ~OpOr() {}
		virtual void release()
		{
			left->release();
			right->release();
			delete this;
		}
		ExpPtr left;
		ExpPtr right;
	};

	class Exp :public ExpBase
	{
	public:
		Exp() :ExpBase() {}
		virtual ~Exp() {}
		virtual void release()
		{
			left->release();
			right->release();
			delete this;
		}

		ExpPtr left;
		ExpPtr right;
	};

	//用于表示重复
	class Repeat
	{
	public:
		Repeat():start(1),end(2),isPositive(true){}
		Repeat(int s, int e, bool ip):start(s), end(e), isPositive(ip) {}
		int start;
		int end;	//超尾
		bool isPositive;
	};
	//宽字符转数字
	inline int wctoi(wchar_t c)
	{
		int wi[10] = { 0,1,2,3,4,5,6,7,8,9 };
		if (c>=L'0'&&c<=L'9')
		{
			return wi[c-L'0'];
		}
		else
		{
			return -1;
		}
	}
	//字符转数字
	inline int wctoi(char c)
	{
		int wi[10] = { 0,1,2,3,4,5,6,7,8,9 };
		if (c >= '0'&&c <= '9')
		{
			return wi[c - '0'];
		}
		else
		{
			return -1;
		}
	}
	//是否是数字
	inline bool isNum(wchar_t c)
	{
		return c >= L'0'&&c <= L'9';
	}
	//字符串转数字
	int ArrayToInt(wchar_t *& c)
	{
		int result(0);
		while (isNum(*c))
		{
			result *= 10;
			result += wctoi(*c);
			++c;
		}
		return result;
	}
	//
	bool isChar(char_t *&str,char_t c)
	{
		if (*str==c)
		{
			++str;
			return true;
		}
		else
		{
			return false;
		}
	}
	bool isStr(char_t *&str, char_t * c)
	{
		char_t *tmp = str;
		while (*c!=0)
		{
			if (*str!=0)
			{
				if (*str!=*c)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		if (*c==0)
		{
			return true;
		}
		else
		{
			str = tmp;
			return false;
		}
	}
	//正则表达式编译
	class TinyRegex
	{
	private:
		string_t pattern;
		std::vector<CharRange> charrange;
		ExpPtr out;
		string_t error;
	public:
		TinyRegex();
		TinyRegex(const string_t &p);
		void assign(const string_t &p);
		~TinyRegex();
		ExpPtr ParseChar(char_t *& str);
		ExpPtr ParseSeqChar(char_t *& str);
		ExpPtr ParseCharSet(char_t *& str);
		ExpPtr ParseAnyChar(char_t *& str);
		ExpPtr ParseLoop(char_t *& str);
		ExpPtr ParseUnit(char_t *& str);
		ExpPtr ParseFunction(char_t *& str);
		ExpPtr ParseSequence(char_t *& str);
		ExpPtr ParseExp(char_t *& str);
	private:
		ExpPtr regexParse();
		void genCharSetTable(const std::vector<CharRange> &charrange,const std::vector<Token> &re);
		inline void addAnd(int &i, int &envNum, std::vector<Token> & operatorStack, std::vector<Token> &ans);
	};

	bool regex_match(const string_t &source,const TinyRegex &pattern,TinyResult & result);
}

