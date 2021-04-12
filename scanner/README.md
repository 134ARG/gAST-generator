# scanner-with-lisp-grammar
general scanner accepting s-expression definition for tokens

### **Method and Design**

---

The program consists of two main parts: script procession and source file procession. Here the script refers to the regex descriptive file for token, the source refers to the source file to be scanned.

#### **Script procession**

##### **Using S-expression to represent regular expression**

Script file contains regex-defined tokens. However, the regular expression here is not written in regular regex grammar. The regex used by the scanner is a lisp-like grammar utilizing S-expression (S means symbolic for some historical reasons). The S-expression has several features:

1. Each S-expression is enclosed by braces `()`;

2. Elements in expressions are split by blanks (space, tab, or other blank characters);

3. Elements of s-expression can be an `Atom` (here is a string or character) or another S-expression;

4. The first element of a S-expression must be an `Atom`, and it is a function name;

5. All the other elements are arguments of the function referred by the `Atom`.

Since basic regex operations are: catenation, alternation, and Kleene closure, any regex can be converted to S-expression by applying three functions: `cat` (catenation), `or` (alternation), `mul` (Kleene closure). For example, a regular expression 

​		`1234(5|6)7*`

can be converted to:

​		`(cat 1234 (or 5 6) (mul 7))`

Here we define that function `cat` and `or` can receive arbitrarily number of arguments, while `mul` only receives one `Atom` or S-expression as the argument.

Since in productive environment, regex has more operations and grammars like `+` (repeat at least once),` -` (indicating a character range) so the actual functions implemented in S-expression expressed regex are:

​		`cat or mul pls range`

Thus, all of the token definitions be written in regex with such grammar. In the root directory of the project, I provided a sample script file which describing tokens for a micro C language. Here list some typical examples:

```lisp
	scanf
-> (cat scanf)
	[0-9]+ 
-> (pls (range 0 9))
	[a-zA-Z]+[[0-9]|[a-zA-Z]|_]* 
-> (cat (pls (or (range a z) (range A Z)))
		(mul (or (range 0 9) (or (range a z) (range A Z)) _)))
```

You can also add other definitions for new tokens. 

The reasons why choosing S-expression as the form of regex are:

1. Grammar of S-expression is uniform and simple thus it is easy to scan and parse;

2. S-expression is itself an AST tree, making it appropriate for recursive procession;

3. It is convenient to convert other languages to S-expression.

##### **Details about functions in S-expression expressed regex**

​    Note that arguments can be **any** characters except left brace '(' and right brace ')'. In other words, you can directly write '+',' –', '*', '"', etc. as the argument. If you want to include braces in the S-expression please use '\\' to escape them. '\\' can also be used to escape **any** other character including blanks and control characters. Examples are line 16, 17 in sample script `micro_c.tokens`.

​	For function `cat`: It receives arbitrarily number of arguments. The valid arguments have three types: char, string, and other S-expressions. It generates a regex which all of its arguments are catenated. For example: 

```
   (cat a b c) 

== (cat ab c) 

== (cat a bc) 

== (cat abc) 

== "abc" 
```

The order of its arguments matters since all of its arguments will be catenated in the order which is from first to last.

​	For function `or`: It is similar to the cat, except that it makes all of its arguments as alternatives. For example:

```
   (or a b c) 

== a|b|c
```

Note that not like `cat`, `(or abc)` `(or ab c)` `(or a bc)` are not equivalent. They are 

```
abc|λ  

(ab)|c 

a|(bc)
```

respectively.

​	For function `mul`: it only receives one argument, which should be char, string, or another S-expression. **Surplus arguments will be ignored**. It will return true if the argument repeats zero or more times. For example:

```
(mul a) == a*

(mul abc) == (abc)*
	
(mul abc def) == (abc)*
```

​	For function `pls`: Same as function `mul`. Except that it requires the argument to repeat at least once. For example:

```
(pls a) == a+

(pls abc) == (abc)+
    
(pls abc def) == (abc)+
```

​	For function `range`: it only receives two arguments which can only be **character**. First one is the start character, second one is the end character. For example:

```
(range a z) == [a-z]
	
(range 0 9) == [0-9]
```

##### **Structure of the script file**

​    The structure of the script file is simple. An S-expression representing a regular expression, followed by the token name. This pattern repeats till end of the file. 

##### **Procession of script**

​    This task is carried out by functions defined in **sscanner.c** and **sparser.c**. Here the leading 's' means 'S-expression'. This process is started by `sparse_main()` defined in **sparser.c**.

​    `next_token()` in **sscanner.c** will return next token once called. Token types is defined in **sscanner.h**. There are four kinds of symbols: `EOSCAN`, `SYMBOL`, `LPAREN`, `RPAREN`. `EOSCAN` corresponds to `EOF`, `SYMOBL` is element in S-expression, `LPAREN is` '(', `RPAREN` is ')'. If the next token is `SYMBOL`, `next_token()` will also store the string that matches `SYMBOL` to global variable `Text` which defined in **globals.c**. `next_token()` will call `next_char()` to go through the script file. `next_char(`) read the file line by line to reduce the number of syscalls and it also maintains the global variable ` lineno` which stores the current line number for error report.

​    `parse_s()` in **sparser.c** will complete the parse process. It processes the input token by the rules of S-expression mentioned above to generate the memory expression of S-expressions. These expressions are stored in global variable `regex`, which is a simple stack implemented in **stack.c**. Also, it stores token name into global variable `token_names`, which is also a stack.

After reading the script and generating corresponding regex in memory, the procession of script is finished. Next is the procession of source files.

#### **Procession of source files**

​    This process is completed by functions in **spredicate.c** and **scanner_main.c**.

​    First `scanner_main()` in **scanner_main.c** will call `next_unit()` defined in **sscanner.c**. It is similar to the function `next_token()`, but it directly returns the unit string it reads. Here the unit string is the string separated by blank and control characters in the source file. For example, if the file contains:

```
int main() {
	int a;
}
```

Then the return values of `next_unit()` will be:

```
int
main()
{
int
a;
}
```

Note that 'main()' and 'a;' are returned as a whole since they are not separated by blanks. The return value will be sent to function `languagep()` defined in **spredicate.c** for finding matching token. 'p' here means predicate, and 'languagep' means 'predicate for deciding whether the string is the language of the regex'. `languagep()` will go through all regex to find the rule that can match the input as long as possible. If there are several rules match the same length of the input, the first one defined will be returned. The `languagep()` will return the token id for accessing the token name. If not found, it will return `-1`. Also if the input is partly matched, `scanner_main()` will keep the scanned index for matching the remaining part of the input.

​    Once successfully found matching token, `scanner_main()` will output the result.

### **Compile & Run**

---

1. Extract source files

2. `cd ./SOURCE_FILE_DIRECTORY`

3. `cmake ./` (require cmake 3.17 or higher)

4. `make`

   The binary named CScanner should be under that directory. Then

5. `./CScanner -i INPUT_MICRO_C_SOURCE_FILE`

   ##### CMD arguments for CScanner:

   `-i`: Source file for scanning. Must be specified.

   `-o`: Optional. Output file for result. If not specified, results will be printed to `stdout`.

   `-s`: Optional. Definition file. If not specified, the program will read the default script named "`micro_c.tokens`" under current directory. This script is attached under the same directory of the source files.

### Sample

---

​    Input file `tests.file`:

```C
void destruct_globals() {
    for (int i = 0; i < regexlength; i++) {
        free_expression(get(&regex, i));
    }
    free_content(&token_names);
    destruct(&regex);
    destruct(&token_names);
}
```

​	Command (suppose that `micro_c.tokens` is under the working directory):

`$ ./CScanner -i ./tests.file`

​	Output:

```
void: VOID
destruct_globals: ID
(: LPAR
): RPAR
{: LBRACE
for: ID
(: LPAR
int: INT
i: ID
=: ASSIGN
0: INT_NUM
;: SEMI
i: ID
<: LT
regexlength: ID
;: SEMI
i: ID
+: PLUS
+: PLUS
): RPAR
{: LBRACE
free_expression: ID
(: LPAR
get: ID
(: LPAR
&: AND_OP
regex: ID
,: COMMA
i: ID
): RPAR
): RPAR
;: SEMI
}: RBRACE
free_content: ID
(: LPAR
&: AND_OP
token_names: ID
): RPAR
;: SEMI
destruct: ID
(: LPAR
&: AND_OP
regex: ID
): RPAR
;: SEMI
destruct: ID
(: LPAR
&: AND_OP
token_names: ID
): RPAR
;: SEMI
}: RBRACE
```

