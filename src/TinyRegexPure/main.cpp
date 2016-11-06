#include"TinyRegexPure.h"
#include"tr_type.h"
int main()
{
	tinyregex::TinyRegexPure re(T("(a|(?:b|ce))d"));
	system("pause");
	return 0;
}