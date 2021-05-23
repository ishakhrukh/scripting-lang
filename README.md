# scripting_lang
A scripting language inspired off of python.
## IMPORTANT:
This project is in the pre-alpha phase, which means it will have a ton of bugs and minimal features (please report bugs at `issues` on the github repo). Please read ***Tutorial*** and if possible, ***Sample Scripts***. Don't expect this scripting language to do much, like arithmetic operations, if conditions, etc.
## What's New in Alpha Release 0.1.1
* Bug fix with `_write(dest, block)`

Q: Why did it take you so long just to write a simple bug fix?

A: I was planning on adding new features with the update, but somehow things got messed up and ruined the whole program, so I have to start writing those features from scratch. I was planning on this bug fix for a long time, so it got released, but everything else didn't. Hopefully, it won't take long to release the new features.
## Bugs
Current bugs:
* You can't pass a variable that was assigned by a function as an argument because (currently) there's no way to tell what the type of the variable is.
* If a variable's value is too long, the program throws a segfault.
* Random things seem to cause a `realloc() invalid next size` when initializing a string (rare).
## Builtins
### `_write(dest, block)`

`dest`: filename (`string`) or file descriptor (`int`) `1` for `stdout`, `2` for 
`stderr`

returns: nothing

example usage: `write(1, 'hello world\n')`
### `_read(src)`
`src`: filename (`string`)

returns: contents of `src` as `string`

example usage: `_read('some-file.txt')`
### `_nextLine()`
returns: next line read from `stdin` as `string`

example usage: `response = _nextLine()`
### `_memdump()`
returns: nothing

example usage: `_memdump()`
### `_exit(status)`
`status`: exit status (`int`)

returns: nothing

example usage: `_exit(0)`
## Tutorial
#### Please note that ***Tutorial*** will move to the wiki after this project grows more
### Declaration
* To declare a variable, use `<name> = <value>` (e.g. `thenumber = 42`, `str = 'hello world'`). The interpreter ignores whitespace. Both `'` and `"` may be used as string delimiters. Don't make variable values too long (see ***Bugs***).
* To declare a function, use `fun <name>(<args>) =` (e.g.`fun sum(a, b) =`). If the function doesn't take any arguments, you may use either `fun somefunction =` or `fun somefunction() =`. Once again, whitespace is ignored. Every block after the `=` must be manually indented by pressing `tab`, or it will signal the end of the function. You can return a value from a function by using the `return` keywords, which can return a constant or a variable (eg. `return 3.14`, `return pi`, `return 'three point one four'`). Arithmetic operations aren't supported yet, so don't write stuff like `return a + b`.
### Other Stuff
* To call a function use `<function name>(<args>)` (eg. `somefunction()`, `response = _nextLine()`, `bob('hello', 123)`)
## Sample Scripts
### This section contains some sample scripts that passed testing
### IMPORTANT
**The `>>>` and `...` are part of the prompt, don't mix them up for a piece of the code**
```
>>> fun return_a_string(str) =
...     return str
... 
>>> hello = return_a_string('this is a string')
>>> str = 'this is another string'
>>> hi = "I am a string too"
>>> _memdump()
```
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
>>> fun foo(arg) =
...     return arg
...
>>> fun bar(arg) =
...     something = foo(arg)
...     return something
...
>>> a = bar('test string')
>>> _memdump()
```
## Update log
### 0.1.1:
* Emergency bug fix with `_write(dest, block)`
### 0.1:
* Fixed some args bugs. Before, you could only pass literals as args when calling inside functions. (There are still some args bugs lurking though. If you find any bugs, please complain in `issues` at the github page)
* new builtin function `_write(dest, block)`. Read ***Builtins*** for more info
* new builtin function `_read(src)`. Read ***Builtins*** for more info
* new builtin function `_nextLine()`. Read ***Builtins*** for more info
* new builtin function `_memdump()`. Read ***Builtins*** for more info
* new builtin function `_exit(status)`. Read ***Builtins*** for more info.
* removed ***Temporary Stuff*** section from readme.md
* removed `$fstack` command
* removed `$stack` command
* removed `exit` command