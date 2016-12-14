tinyregex
===
A tiny regular expression library.

#Environment
This library is writed under c++ 11 compiler. It was tested in MSVC15 and GNUC4.9.2 compiler.

#Syntax supported
## pattern
| pattern | describe |
|:-------:|:--------:|
|   `^`   | the begin of the string | 
|   `$`   | the end of the string | 
|  `ab`   | matches `ab` |
| `a|b`   | `a` or `b` |
|  `a{x,y}`   | matches `a` which repeats from `x` to `y`|
|  `a*`   | equal to `a{0,}` |
|  `a?`   | equal to `a{0,1}` |
|  `a+`   | equal to `a{1,}` |
|  `(...)`| capture the content inside|
|  `[a-b]`| any a character between `a` and `b` |
|  `.`    | matches any a character |
|   `a+?` | matches the repeated `a` as less as possible|
|  `\`    | escapes special characters|

supported escaping sequences:

| escapes | describe |
|:-------:|:--------:|
|   `\t`  |  tab |
|   `\n`  | newline |
|   `\r`  | return |
|   `\s`  |  space |
|   `\\`  |  `\`   |
|   `\(`  |  `(`   |
|   `\)`  |  `)`   |
| `\[`    |   `[`  |
| `\]`    |   `]`  |
|  `\-`   |   `-`  |
|  `\.`   |   `.`  |
|   `\+`  |   `+`  |
|   `\$`  |   `$`  |


##Match and compile option

```cpp
    enum CompileFlag
    {
        NFA_AUTOMATON=0x1,  //&~DFA_AUTOMATON
    };
    enum MatchFlag
    {
        MATCH_DEFAULT=0x0,
        MATCH_NOT_BOL=0x1,  //The first character in will be treated as if it is not at the beginning of a line 
        MATCH_NOT_EOL=0x2,  //The last character in will be treated as if it is not at the end of a line 
        MATCH_NOT_NULL=0x4, //Do not match empty sequences
        MATCH_CONTINUOUS=0x8,  //Only match a sub-sequence that begins at first
        MATCH_BFS=0x10, //&~MATCH_DFS
    };
 ```

 These enum constants decide that the engine how to compile, search and match.
 



##Development process
 
		*Capture
		*DFA
		*bfs match and search
 They are needed added in.
