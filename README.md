tinyregex
===
A tiny regular expression library.

#Language
*[English](http://) 
*[中文](http://)

#Environment
This library is writen in c++ 11. It was tested in MSVC15 and GNUC4.9.2 compiler.

#Syntax supported
## pattern
| pattern | describe |
|:-------:|:--------:|
|   `^`   | the begin of the string | 
|   `$`   | the end of the string | 
|  `ab`   | matches `ab` |
| `a|b`   | `a` or `b` |
|  `a{x,y}`   | matches `a` which repeats from `x` to `y`|
|  `a*`   | equivalent to `a{0,}` |
|  `a?`   | equivalent to `a{0,1}` |
|  `a+`   | equivalent to `a{1,}` |
|  `(...)`| indicate the start and end of a group|
|  `[a-b]`| any a character between `a` and `b` |
|  `[^a-b]`| characters that are not within the range |
|  `.`    | matches any a character |
|   `a+?` | matches the repeated `a` {1,} as less as possible|
|   `a??` | matches the repeated `a` {0,1} as less as possible|
|   `a*?` | matches the repeated `a` {0,} as less as possible|
|   `a{x,y}?` | matches the repeated `a` {x,y} as less as possible|
|  `\`    | escapes special characters|

supported escaping sequences:

| escapes | describe |
|:-------:|:--------:|
|   `\t`  |  tab |
|   `\n`  | newline |
|   `\r`  | return |
|   `\s`  |  space |
|   `\S`  |  not space|
|   `\d`  |  numbers   |
|   `\D`  |  not numbers |
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
|   `\kNumber`  |  refer a capture |
|   `\k'...'`  |  refer a named capture |


supported capture:

| capture | describe |
|:-------:|:--------:|
|  `(...)`  |  capture to an array |
|  `(?<...>...)` | named capture |
|  `(?:...)` | no capture |

Posix Character Classes
| class | describe |
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


##Match and compile option

```cpp
    enum SyntaxFlag
    {
        DEFAULT=0x0,        
        //supported

        //DFA_AUTOMATON=0x1,  //&~DFA_AUTOMATON
        //removed

        ICASE=0x2,          //Character matching should be performed without regard to case. 
        //supported

        NOSUBS=0x4,         //All marked sub-expressions (expr) are treated as non-marking sub-expressions 
        //supported

        OPTIMIZE=0x8,
        COLLATE=0x10,
        MULTILINE=0x20,	//Specifies that ^ shall match the beginning of a line and $ shall match the end of a line.
        //supported
	
        ECMASCRIPT=0x40,
        //partial supported

        BASIC=0x80,
        EXTENDS=0x100,
        AWK=0x200,
        GREP=0x400,
        EGREP=0x800
    };
    enum MatchFlag
    {
        MATCH_DEFAULT=0x0,
        //supported

        MATCH_NOT_BOL=0x1,  //The first character in will be treated as if it is not at the beginning of a line
        //supported

        MATCH_NOT_EOL=0x2,  //The last character in will be treated as if it is not at the end of a line 
        //supported

        MATCH_NOT_NULL=0x4, //Do not match empty sequences
        //supported

        MATCH_CONTINUOUS=0x8,  //Only match a sub-sequence that begins at first
        MATCH_BFS=0x10, //&~MATCH_DFS
    };
 ```

 These enum constants decide that the engine how to compile, search and match.
 



##Development process
 
		*Capture		√
		*DFA
		*bfs match and search
		*Named capture		√
		*string replace
		*optimize
		*posix regexp

 These features are needed added in.


##How to use it

match -- --
```cpp
	#include"tinyregex.h"
	using namespace tyre;
	int main()
	{
		TinyRegex r(T("hello world"));//T("") indicates the char set of tinyregex
		r.match(T("hello world"));//return true
		r.match(T("hello world"));//return false
		return 0;
	}

```



About char set:</br>
use utf-16: add "#define TINYREGEX_UNICODE" before "base.h";</br>
use ascii: remove "#define TINYREGEX_UNICODE" in "base.h";</br>


