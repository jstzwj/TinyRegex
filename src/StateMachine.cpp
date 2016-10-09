#include "StateMachine.h"

namespace tinyregex
{
	StateMachine::StateMachine()
	{
		startnode = 1;
		state[1] = std::map<char_t, int>();
		endnode.insert(1);
		nodeTop = 1;
	}
	void StateMachine::addNode()
	{
		state[nodeTop++] = std::map<char_t, int>();
	}
	void StateMachine::linkNode(int from, int to, char_t condition)
	{
		state[from][condition] = to;
	}
	void StateMachine::selfAdd()
	{
		linkNode(startnode,*(endnode.begin() ),0);
	}
	void StateMachine::opAnd(const StateMachine & rhs)
	{
		//Æ´½Ó
		std::map<int, std::map<char_t, int> >::const_iterator i;
		std::map<char_t, int>::const_iterator j;

		for (i = rhs.state.cbegin(); i != rhs.state.cend(); ++i)
		{
			for (j = i->second.cbegin(); j != i->second.cend(); ++j)
			{
				state[i->first + nodeTop][j->first] = j->second + nodeTop;
			}
		}
		std::set<int>::const_iterator k;
		for (k = endnode.cbegin(); k != endnode.end(); ++k)
		{
			linkNode(*k, nodeTop+rhs.startnode, 0);
		}
		endnode = rhs.endnode;
		nodeTop += rhs.nodeTop;
	}
	void StateMachine::opOr(const StateMachine & rhs)
	{
		std::map<int, std::map<char_t, int> >::const_iterator i;
		std::map<char_t, int>::const_iterator j;

		for (i = rhs.state.cbegin(); i != rhs.state.cend(); ++i)
		{
			for (j = i->second.cbegin(); j != i->second.cend(); ++j)
			{
				state[i->first + nodeTop][j->first] = j->second + nodeTop;
			}
		}
		linkNode(startnode, nodeTop + rhs.startnode, 0);
		std::set<int>::const_iterator k;
		for (k = rhs.endnode.cbegin(); k != rhs.endnode.end(); ++k)
		{
			linkNode(*k+nodeTop, *(endnode.begin()), 0);
		}
		nodeTop += rhs.nodeTop;
	}

	
}




