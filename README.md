# scripting_lang
A scripting language inspired by python.
## btw
I created `scripting-lang` to exercise my C skills and it isn't meant to be used as an actual scripting language because of how incapable it is.
## Notice:
Most interpreters work by parsing the input into *abstract syntax trees*, then generating bytecode from it, which gets interpreted by a virtual machine. Currently, this interpreter directly executes the syntax trees instead of generating bytecode, which may seem faster, but in most cases, it's slower. Therefore, I will be working on adding a bytecode generator and a VM, which means re-writing the **whole interpreter section**, so there will be no or few updates for the following month.
## What's New in Alpha Version 0.2.1
* Added `for` loops, read about them in ***Tutorial***
* Partially implemented lists, read about them in ***Tutorial***
* Updated built-in function `_nextLine()` (now `_nextLine([optional]prompt)`). 
* Updated built-in function `_memdump()` to work with lists/arrays
* Bug fixes
## Bugs
Current bugs:
* If a variable's value is too long, the program throws a segfault.
* Random things seem to cause a `realloc(): invalid next size` when initializing a string, though it's rare.
## Tutorial
### Variables
To create a variable, use the following syntax: `<name> = <value>`. `<value>` may be a literal, another variable, or a function call. When initializing string, both `'` and `"` can be used as delimiters, though I'd prefer `'`. Variables are mutable, so you can change their values even after they were assigned. Here are some examples:

```
a = 10
b = [9, 'string', 3.14, 8, "another string", 34]
c = a
c = b
d = somefunction()
```
### Lists
Lists are used to store multiple values in one variable. Currently, you can't do anything with lists besides initializing them, but more features will come in later updates. They are created with square brackets (as shown in the examples above) and must have at least one item. You cannot assign variables to lists or pass lists as parameters (at least, not yet). However, you can access list elements. Here is an example of a list:

`myList = [189, 200, 'some string', 3.14]`
### For loops
There are two types of `for` loops. The first one will repeat a group of statements a certain number of times.  The second one will iterate through an iterable element, but it is not available because lists aren't completed yet. The syntax for `for` loops goes as follows (code in brackets is optional):

`for <counter> = <start> to <end> [step <step>]:`

Every piece of code after the `:` must be indented with 4 spaces, which can be done quickly by pressing `tab` if on a terminal, or it will signal the end of the loop's body.

Part|Description
-|-
`<counter>`|Required. The iterator of the loop. Can only be initialized to a constant (you will be able to initialize it to other variables in the next update).
`<start>`|Required. The initial value of `<counter>`
`<end>`|Required. The number of which `<counter>` iterates **up to but not including**.
`<step>`|Optional. The amount by which `counter` is incremented each time.

#### Examples
```
for i = 1 to 11:
    _write(1, 'hi there\n')
```
```
for i = 0 to 9 step 2:
    _write(1, i);
```

### Functions
Functions are blocks of code that run when they are called. Functions can take arguments and they can return values. To define a function, use the `fun` keyword. Every piece of code after that must be indented with 4 spaces, which can be done quickly by pressing `tab` if on a terminal, or it will signal the end of the function's body. This can be done quickly by pressing `tab` on the shell.
```
fun print_something() =
    _write(1, 'hello world\n')
```
If a function does not take any arguments, then `(` and `)` aren't necessary, so this would be valid too:
```
fun print_something =
    _write(1, 'hello world\n')
```
To call a function, use the function name followed by parentheses:

`print_something()`

Arguments are specified after the function name, between the parentheses (e.g. `fun print(str) =`). You can add as many as you want by separating them with commas (e.g. `fun someFunction(arg, another_arg) =`). When you call function, you pass the values inside the parentheses (e.g. `print('hello')`, `someFunction(3, someVariable)`). Try out the following code (the `>>>` and `...` are part of the shell, don't type those in):
```
>>> fun print(str) =
...     _write(1, str)
...
>>> print('hello\n')
```
### Built-in Functions
Built-in functions are pre-defined functions that one can call like any other function. In the examples above, we used a built-in function: `_write(1, str)`. Here is a documentation of all the built-in functions:
#### `_write(dest, block)`

`dest`: filename (`string`) or file descriptor (`int`) `1` for `stdout`, `2` for 
`stderr`

returns: nothing

example usage: `write(1, 'hello world\n')`
#### `_read(src)`
`src`: filename (`string`)

returns: contents of `src` as `string`

example usage: `_read('some-file.txt')`
#### `_nextLine([optional]prompt)`
`prompt`: Optional prompt.

returns: next line read from `stdin` as `string`

example usage: `response = _nextLine()`, `name = _nextLine('enter your name: ')`
#### `_memdump()`
returns: nothing

example usage: `_memdump()`
#### `_exit(status)`
`status`: exit status (`int`)

returns: nothing

example usage: `_exit(0)`
## Sample Scripts
**The `>>>` and `...` are a part of the prompt, don't mix them up for a piece of the code**
```
>>> fun print(str) =
...     _write(1, str)
...
>>> print('hello world\n')
>>> file = 1
>>> text = 'some text'
>>> _write(file, text)
```
```
>>> for i = 0 to 11 step 2:
...     _write(1, 'i = ')
...     _write(1, i)
...     _write(1, '\n')
...
```
```
>>> fun say_hi(name) =
...     _write(1, 'hello, ')
...     _write(1, name)
...     _write(1, '\n')
...
>>> name = _nextLine('enter your name: ')
>>> say_hi(name)
```
## Update log
### 0.2.1:
* Added `for` loops, read about them in ***Tutorial***
* Partially implemented lists, read about them in ***Tutorial***.
* Updated built-in function `_nextLine()` (now `_nextLine([optional]prompt)`). 
* Updated built-in function `_memdump()` to work with lists/arrays
* Bug fixes
### 0.1.1:
* bug fix with `_write(dest, block)`
### 0.1:
* Fixed some args bugs. Before, you could only pass literals as args when calling inside functions. (There are still some args bugs lurking though. If you find any bugs, please complain in `issues` at the GitHub page)
* new built-in function `_write(dest, block)`. 
* new built-in function `_read(src)`. 
* new built-in function `_nextLine()`. 
* new built-in function `_memdump()`. 
* new built-in function `_exit(status)`. 
* removed `$fstack` command
* removed `$stack` command
* removed `exit` command
