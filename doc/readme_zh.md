tinyregex
===
这是一个小型正则表达式引擎。
#语言
*[English](https://github.com/jstzwj/TinyRegex/blob/master/doc/readme_en.md) </br>
*[中文](https://github.com/jstzwj/TinyRegex/blob/master/doc/readme_zh.md)

#环境
这个正则表达式引擎使用了部分c++11的特性。它在MSVC15和GNUC4.9.2编译器上测试通过。

#支持的语法
## 符号
| 符号 | 描述 |
|:-------:|:--------:|
|   `^`   | 字符串的开始 | 
|   `$`   | 字符串的结束 | 
|  `ab`   | 匹配 `ab` |
| `a|b`   | `a` 或 `b` |
|  `a{x,y}`   | 匹配`a`重复x到y次 |
|  `a*`   | 等价于 `a{0,}` |
|  `a?`   | 等价于 `a{0,1}` |
|  `a+`   | 等价于 `a{1,}` |
|  `(...)`| 表示着一个组的开始和结尾|
|  `[a-b]`| 在 `a` 到 `b` 间的任意字符 |
|  `[^a-b]`| 不在a到b范围内的任意一个字符 |
|  `.`    | 匹配任意一个字符 |
|   `a+?` | 尽可能少的匹配 `a` {1,} |
|   `a??` | 尽可能少的匹配 `a` {0,1} |
|   `a*?` | 尽可能少的匹配`a` {0,} |
|   `a{x,y}?` | 尽可能少的匹配 `a` {x,y} |
|  `\`    | 转义表示特殊字符|

支持的转义序列:

| 转义字符 | 描述 |
|:-------:|:--------:|
|   `\t`  |  水平制表 |
|   `\n`  | 换行 |
|   `\r`  | 回车 |
|   `\s`  |  空白字符 |
|   `\S`  |  非空白字符|
|   `\d`  |  数字   |
|   `\D`  |  非数字 |
|   `\\`  |  `\`   |
|   `\(`  |  `(`   |
|   `\)`  |  `)`   |
|   `\{`  |  `{`   |
|   `\}`  |  `}`   |
| `\[`    |   `[`  |
| `\]`    |   `]`  |
|   `\|`  |  `|`   |
|   `\?`  |  `?`   |
|  `\-`   |   `-`  |
|  `\.`   |   `.`  |
|   `\+`  |   `+`  |
|   `\$`  |   `$`  |
|   `\^`  |   `^`  |
|   `\kNumber`  |  引用一个已经捕获的内容 |
|   `\k'...'`  |  引用一个命名捕获的内容 |


支持的捕获语法:

| 捕获 | 描述 |
|:-------:|:--------:|
|  `(...)`  |  一般捕获 |
|  `(?<...>...)` | 命名捕获 |
|  `(?:...)` | 不捕获 |

Posix 字符类:

| 字符类 | 描述 |
|:-------:|:--------:|
|  `[:alnum:]`  |  `[0-9a-zA-Z]` |
|  `[:alpha:]` | `[a-zA-Z]` |
|  `[:ascii:]` | `[\x01-\x7F]` |
|  `[:blank:]`  |  `[ \t]` |
|  `[:cntrl:]` | `[\x01-\x1F]` |
|  `[:digit:]` | `[0-9]` |
|  `[:graph:]`  |  `[^\x01-\x20]` |
|  `[:lower:]` | `[a-z]` |
|  `[:print:]` | `[\t\x20-\xFF]` |
|  `[:punct:]`  |  `[-!"#$%&'( )*+,./:;<=>?@[\\\]^_'{|}~]` |
|  `[:space:]` | `[\n\r\t \x0B]` |
|  `[:upper:]` | `[A-Z]` |
|  `[:xdigit:]`  |  `[0-9a-fA-F]` |


##匹配和编译选项
```cpp
    enum SyntaxFlag
    {
        DEFAULT=0x0,        
        //支持
        //DFA_AUTOMATON=0x1,  //&~DFA_AUTOMATON
        //移除
        ICASE=0x2,          //字符匹配时忽略大小写
        //支持
        NOSUBS=0x4,         //所有捕获表达式视为非捕获表达式
        //支持
        OPTIMIZE=0x8,
        COLLATE=0x10,
        MULTILINE=0x20,	//把^视为一行的开始，把$视为一行的结尾
        //支持	
        ECMASCRIPT=0x40,
        //部分支持
        BASIC=0x80,
        EXTENDS=0x100,
        AWK=0x200,
        GREP=0x400,
        EGREP=0x800
    };
    enum MatchFlag
    {
        MATCH_DEFAULT=0x0,
        //支持
        MATCH_NOT_BOL=0x1,  //第一个字符不会被视为一行的开始
        //支持
        MATCH_NOT_EOL=0x2,  //最后一个字符不会被视为一行的结束
        //支持
        MATCH_NOT_NULL=0x4, //不匹配空序列
        //支持
        MATCH_CONTINUOUS=0x8,  //只匹配第一个子序列
        MATCH_BFS=0x10, //&~MATCH_DFS
    };
 ```

 以上枚举类型表示正则表达式引擎在编译、搜索、匹配时的设置。
 



##开发进度
 
		*Capture		√
		*DFA
		*bfs match and search
		*Named capture		√
		*string replace
		*optimize
		*posix regexp

 这些特性将被加入
 
##如何使用

match -- --
```cpp
	#include"tinyregex.h"
	using namespace tyre;
	int main()
	{
		TinyRegex r(T("hello world"));//T("") indicates the char set of tinyregex
		r.match(T("hello world"));//return true
		r.match(T("HelloWorld"));//return false
		return 0;
	}

```



关于字符集:</br>
使用 utf-16: 添加 "#define TINYREGEX_UNICODE" 在 "base.h"前;</br>
使用 ascii: 移除 "#define TINYREGEX_UNICODE" 在 "base.h"中;</br>

##联系我
e-mail:1103870790@qq.com
