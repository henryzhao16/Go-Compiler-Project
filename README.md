# Go Compiler Project

## Description 

A compiler program that transforms a subset of Golang to C. Written in C, along with the use of Flex/Bison tools. 

It is divided in 6 phases:
* Scanner
* Parser
* Weeder
* Type checker
* Code generator

To run the program and compile a target go file to C code:

```
./build.sh
./run.sh codegen [file path]
```

## Language Features Support

1. Keywords 
```
break default func interface select case defer go map struct chan else goto package switch const fallthrough if range type continue for import return var print println append
```
2. Operators
```
+ & += &= && == != ( ) - | -= |= || < <= [ ] * ˆ *= ˆ= <- > >= { } / << /= <<= ++ = := , ; % >> %= >>= -- ! ... . : &ˆ &ˆ=
```

3. Comments 
```
// 
/* */
```

4. Literals
```
Integer literals
Floating-point literals
Rune literals
```
5. Identifiers

6. Semicolons

7. Package declaration

8. Top-level declarations
```
Variable declaration | Type declarations | Function declarations
```

9. Types
```
int | float64 | bool | rune | string | arrays
```

10. Statements 
```
Block | Expression | Assignment | Declaration | Increment | Decrement | Print | Println | Return | If | If Else | Switch | Case | Default | For | Break | Continue
```

11. Expressions
```
Identifier | ( Expression ) | Literals | Unaries | Binary operations | Function calls | Type Casts
```

## Reference
Design pattern inspired from JOOS (Java Object-Oriented Subset), designed by Laurie Hendren of McGill University and Michael Schwartzbach from Aarhus University in Denmark, at https://github.com/comp520/JOOS.
