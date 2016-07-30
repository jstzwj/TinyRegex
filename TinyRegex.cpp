#include "TinyRegex.h"


namespace tinyregex
{
	TinyRegex::TinyRegex()
	{
	}

	tinyregex::TinyRegex::TinyRegex(const string_t & p)
		:pattern(p)
	{
		//编译表达式
		std::vector<Token> out;
		out = regexParse();
	}

	void TinyRegex::assign(const string_t & p)
	{
		if (p != pattern)
		{
			std::vector<Token> out;
			pattern = p;
			out=regexParse();
		}
	}


	TinyRegex::~TinyRegex()
	{
	}

	std::vector<Token> TinyRegex::regexParse()
	{
		std::stack<Token> operatorStack;
		std::vector<Token> ans;
		std::vector<byte> beforeEnv;	//保留
		int len = pattern.length();
		int envNum=0;
		//存入优先级最低的结束符
		operatorStack.push(make_token(T("\0"),32,true));
		//遍历
		for (int i = 0; i < len;++i)
		{
			if (pattern[i] == T('\\'))	//1
			{
				Token token;
				token.isOperator = false;
				if (i<len-2&&pattern[i + 1] == T('c'))
				{
					token.content = pattern.substr(i,3);
				}
				else if (i<len - 5 && pattern[i + 1] == T('u'))
				{
					token.content = pattern.substr(i, 6);
				}
				else if (i<len - 3 && pattern[i + 1] == T('x'))
				{
					token.content = pattern.substr(i, 4);
				}
				else
				{
					if (i < len - 1)
					{
						token.content = pattern.substr(i, 2);
					}
					else
					{
						//TODO error
					}
				}
				ans.push_back(token);
				//添加和&
				addAnd(i,envNum, operatorStack, ans);
			}
			else if (pattern[i]==T('('))//10	确保被压入栈中，直到后括号出现才被弹出
			{
				Token token;
				if (i<pattern.length()-1&&pattern[i + 1] == T('?'))
				{
					if (i < pattern.length() - 2&&pattern[i+2]==T(':'))
					{
						token.content = T("(?:");
						token.priority = 10;
						token.isOperator = true;
					}
					else if (i < pattern.length() - 2 && pattern[i + 2] == T('='))
					{
						token.content = T("(?=");
						token.priority = 10;
						token.isOperator = true;
					}
					else
					{
						//TODO error
						//return;
					}
				}
				else
				{
					token.content = T("(");
					token.priority = 10;
					token.isOperator = true;
				}
				operatorStack.push(token);
				//压入当前环境值
				beforeEnv.push_back(envNum);
				//重置当前环境值
				envNum = 0;
			}
			else if (pattern[i] == T(')'))
			{
				while (!operatorStack.empty())
				{
					string_t s = operatorStack.top().content;
					ans.push_back(operatorStack.top());
					operatorStack.pop();
					if (s==T("(")||s==T("(?:")||s==T("(?="))
					{
						break;
					}
				}
				//不是因为栈空退出
				if (!operatorStack.empty())
				{
					//TODO
				}
				else
				{
					//TODO error
				}
				//检查环境值
				if (envNum == 1 || envNum == 0)
				{
					//是字符、环境值加一
					envNum = beforeEnv.back();
					//添加和&
					addAnd(i,envNum, operatorStack, ans);
					//删除最外层环境
					if (!beforeEnv.empty())
					{
						beforeEnv.pop_back();
					}
					else
					{
						//TODO error

					}
				}
				else
				{
					//TODO error

				}
			}
			else if (pattern[i] == T('['))
			{
				Token token;
				int j;
				for (j = i; j < len&&pattern[j] != T(']');++j){}
				if (pattern[j]==']')
				{
					token.content = pattern.substr(i,j+1-i);
					token.isOperator = false;
					ans.push_back(token);
					i = j + 1;	//跳过整个方括号
				}
				else
				{
					//TODO error
				}
				//添加和&
				addAnd(i,envNum, operatorStack, ans);
			}
			else if (pattern[i]==T('*'))	//2
			{
				Token token;
				token.content = T('*');
				token.priority = 2;
				token.isOperator = true;
				//优先级
				while (!operatorStack.empty() && operatorStack.top().priority <= 2)
				{
					ans.push_back(operatorStack.top());
					operatorStack.pop();
				}
				operatorStack.push(token);
			}
			else if (pattern[i] == T('+'))	//2
			{
				Token token;
				token.content = T('+');
				token.priority = 2;
				token.isOperator = true;
				//优先级
				while (!operatorStack.empty() && operatorStack.top().priority <=2)
				{
					ans.push_back(operatorStack.top());
					operatorStack.pop();
				}
				operatorStack.push(token);
			}
			else if (pattern[i] == T('?'))	//2
			{
				Token token;
				token.content = T('?');
				token.priority = 2;
				token.isOperator = true;
				//优先级
				while (!operatorStack.empty() && operatorStack.top().priority <= 2)
				{
					ans.push_back(operatorStack.top());
					operatorStack.pop();
				}
				operatorStack.push(token);
			}
			else if (pattern[i] == T('{'))	//2
			{
				Token token;
				int j;
				for (j = i; j < len&&pattern[j] != T('}'); ++j) {}
				if (pattern[j] == '}')
				{
					token.content = pattern.substr(i, j + 1 - i);
					token.priority = 2;
					token.isOperator = true;
					while (!operatorStack.empty() && operatorStack.top().priority <= 2)
					{
						ans.push_back(operatorStack.top());
						operatorStack.pop();
					}
					operatorStack.push(token);
					i = j + 1;
				}
				else
				{
					//TODO error
				}
				
			}
			else if (pattern[i] == T('^'))
			{
				Token token;
				token.content = T('^');
				token.isOperator = false;
				ans.push_back(token);
				//添加和&
				addAnd(i,envNum, operatorStack, ans);
			}
			else if (pattern[i] == T('$'))
			{
				Token token;
				token.content = T('$');
				token.isOperator = false;
				ans.push_back(token);
				//添加和&
				addAnd(i,envNum, operatorStack, ans);
			}
			else if (pattern[i] == T('|'))	//4
			{
				Token token;
				token.content = T('|');
				token.priority = 4 ;
				token.isOperator = true;
				//优先级
				while (!operatorStack.empty()&&operatorStack.top().priority < 4)
				{
					ans.push_back(operatorStack.top());
					operatorStack.pop();
				}
				operatorStack.push(token);
				//环境值减一
				--envNum;
			}
			else
			{
				Token token;
				token.content = pattern[i];
				token.isOperator = false;
				ans.push_back(token);
				//添加和&
				addAnd(i,envNum,operatorStack,ans);
			}
			
		}
		//操作符全部弹出
		while (!operatorStack.empty())
		{
			ans.push_back(operatorStack.top());
			operatorStack.pop();
		}
		for (Token &eachs:ans)
		{
			std::wcout << eachs.content<<T("\t");
		}
		system("pause");
		return ans;
	}

	void TinyRegex::addAnd(int &i,int &envNum,std::stack<Token> & operatorStack,std::vector<Token> &ans)
	{
		//是字符、环境值加一
		++envNum;
		//环境值需要插入1以保持平衡
		if (envNum >= 2)
		{
			Token token;
			token.content = T('&');
			token.isOperator = true;
			token.priority = 3;
			if (pattern[i+1]==T('*')|| pattern[i + 1] == T('+') || pattern[i + 1] == T('?'))
			{
				//优先级
				while (!operatorStack.empty() && operatorStack.top().priority <= 3)
				{
					ans.push_back(operatorStack.top());
					operatorStack.pop();
				}
				string_t temp;
				temp.push_back(pattern[i + 1]);
				ans.push_back(make_token(temp,2,true));
				ans.push_back(token);
				++i;
			}
			else
			{
				ans.push_back(token);
			}
			//环境值减一
			--envNum;
		}
	}


	bool regex_match(const string_t & source, const TinyRegex & pattern, TinyResult & result)
	{
		return false;
	}

}
