# tinyregex
A tiny regular expression library.

#Environment
This library is writed under c++ 11 compiler. It was tested in MSVC15 and GNUC4.9.2 compiler.

#Syntax supported
## pattern
| pattern | describe |
|:-------:|:--------:|
|   `^`   | the begin of the string | 
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
