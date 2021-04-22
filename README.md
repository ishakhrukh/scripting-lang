# scripting_lang
A scripting language inspired off of python.
## IMPORTANT:
This project is in the pre-alpha phase, which means it will have a ton of bugs and minimal features.
## Current Bugs
Current bugs:
* If a variable's value is too long, the program throws a segfault.
* Random things seem to cause a `realloc() invalid next size` when initializing a string.
* Nothing else discovered yet
## Syntax
#### Please note that ***Syntax*** will move to the wiki after this project grows more
### Temporary stuff
* Type `$stack` to view the global stack.
* Type `$fstack` to view the addresses and names of functions.
* Type `$?` to view the return value of the last function call.
* Type `exit` to exit the shell.
* These commands are temporary and will most likely be replaced by functions in the future, hence the title.
### Variable Declaration
<<<<<<< HEAD
To declare a variable, use `<name> = <value>` (e.g. `pi = 3.14`, `str = 'hello world'`, `hi = somefunction(123)`). The interpreter ignores whitespace. Both `'` and `"` may be used as string delimiters. Don't make variable values too long (see ***Bugs***).
### Functions
To declare a function, use `fun <name>(<args>) =` (e.g.`fun sum(a, b) =`). If the function doesn't take any arguments, you may use `fun somefunction =` or `fun somefunction() =`. Once again, whitespace is ignored. Every block after the `=` must be manually indented by pressing `tab`, or it will signal the end of the function. You can return a value by using `return <value>`, where `<value>` may be a literal or a variable.
=======
* To declare a variable, use `<name> = <value>` (e.g. `pi = 3.14`, `str = 'hello world'`). The interpreter ignores whitespace. Both `'` and `"` may be used as string delimiters. Don't make variable values too long (see ***Bugs***).
* To declare a function, use `fun <name>(<args>) =` (e.g. `fun say_hi() =`, `fun sum(a, b) =`). Once again, whitespace is ignored. Every block after the `=` must be manually indented by pressing `tab`, or it will signal the end of the function. A function may not be empty, but if it has to, use a dummy variable like `foo = 'bar'`.
>>>>>>> origin/main
