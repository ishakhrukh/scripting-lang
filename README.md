# scripting_lang
A scripting language inspired off of python.
## IMPORTANT:
This project is in the pre-alpha phase, which means it will have a ton of bugs and minimal features. So far, the only features available are variable and function declarations along with some temporary commands that will be removed later (see *Temporary Stuff* in ***Syntax***).
## Current Bugs
Current bugs:
* If a variable's value is too long, the program throws a segfault (`SIGSEGV`).
* Nothing else for now
## Syntax
### Temporary stuff
* Type `$stack` to view the global stack.
* Type `$fstack` to view the addresses and names of functions.
* Type `exit` to exit the shell.
* These commands are temporary and will most likely be replaced by functions in the future, hence the title.
### Variable Declaration
* To declare a variable, use `<name> = <value>` (e.g. `pi = 3.14`, `str = 'hello world'`). The interpreter ignores whitespace. Both `'` and `"` may be used as string delimiters. Don't make variable values too long (see ***Bugs***).
* To declare a function, use `fun <name>(<args>) =` (e.g. `fun say_hi() =`, `fun sum(a, b) =`). Once again, whitespace is ignored. Every block after the `=` must be manually indented by pressing `tab`, or it will signal the end of the function. A function may not be empty, but if it has to, use `pass`.