#include"TinyRegex.h"
#include"tr_type.h"
int main()
{
	tinyregex::TinyRegex re(T("(a|(?:b|ce))d"));
	system("pause");
	return 0;
}