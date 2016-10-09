#pragma once
#include<map>
#include<set>
#include"tr_type.h"

namespace tinyregex
{
	class StateMachine
	{
	public:
		typedef std::map<int, std::map<char_t, int> > StateMap;
		std::map<int, std::map<char_t,int> > state;
		int startnode;
		std::set<int> endnode;
		int nodeTop;


		void addNode();
		void linkNode(int from,int to,char_t condition);

		void selfAdd();
		void opAnd(const StateMachine &rhs);
		void opOr(const StateMachine &rhs);
		StateMachine();

	};

}

