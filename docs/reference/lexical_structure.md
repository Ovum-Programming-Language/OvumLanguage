# Lexical Structure

Ovum's source code uses a lexical syntax familiar to C-style and Kotlin-style languages:

## Identifiers

Names for variables, functions, classes, etc., consist of letters, digits, and underscores, and must not begin with a digit. For example: `myVar`, `compute_sum`, `GraphNode`. Identifiers are case-sensitive.

**Naming Convention**: Classes, functions, methods use **PascalCase** (e.g., `Main`, `ToString`, `IsLess`). Keywords/modifiers remain lowercase (`class`, `interface`, `var`, `override`, `pure`, `unsafe`, etc.).

## Keywords

Ovum reserves certain words like `fun`, `class`, `interface`, `var`, `override`, `pure`, `if`, `else`, `for`, `while`, `return`, `unsafe`, `val`, `static`, `public`, `private`, `implements`, `as`, `is`, `null`, `true`, `false`, `typealias`, `destructor`, `call`, etc. These cannot be used as identifiers.

## Literals

* **Numeric literals**: integers (e.g., `42`, `-17`) and floating-point numbers (e.g., `3.14`, `-2.5`)
* **Boolean literals**: `true`, `false`
* **Character literals**: single characters in single quotes (e.g., `'A'`, `'\n'`, `'\t'`)
* **String literals**: text in double quotes (e.g., `"hello"`, `"world"`) with escape sequences like `"\n"` for newline, `"\t"` for tab, `"\\"` for backslash, `"\""` for quote

## Operators and Punctuation

* **Arithmetic**: `+`, `-`, `*`, `/`, `%`
* **Comparison**: `==`, `!=`, `<`, `<=`, `>`, `>=`
* **Boolean logic**: `&&` (logical AND), `||` (logical OR), `!` (negation), `xor` (exclusive OR)
* **Assignment**: `=` (reference assignment), `:=` (copy assignment)
* **Null handling**: `?.` (safe call), `?:` (Elvis)
* **Type operations**: `as` (cast), `is` (type test)
* **Punctuation**: `,` (comma), `;` (semicolon), `:` (colon), `()` (parentheses), `{}` (braces), `[]` (brackets)
* **Namespace resolution**: `::`

## Comments

* **Line comments**: start with `//` and continue to the end of the line
* **Block comments**: start with `/*` and end with `*/`, can span multiple lines. Nested comments are not allowed

## Whitespace

Spaces, tabs, and newlines are generally ignored outside of separating tokens. Indentation is not significant (Ovum is not whitespace-sensitive except that newlines can terminate statements if no semicolon is present).

## Formal Grammar (EBNF)

> Core EBNF; whitespace/comments omitted. Operator precedence in §7.

```ebnf
Program         ::= { Import | Conditional | GlobalDef } ;

Import          ::= "#import" StringLiteral ;
Define          ::= "#define" Identifier [ NumberLiteral ] ;
Undef           ::= "#undef" Identifier ;
Conditional     ::= "#ifdef" Identifier { GlobalDef | Import | Conditional }
                    [ "#else" { GlobalDef | Import | Conditional } ] "#endif"
                 | "#ifndef" Identifier { GlobalDef | Import | Conditional }
                    [ "#else" { GlobalDef | Import | Conditional } ] "#endif" ;

GlobalDef       ::= FunctionDecl | ClassDecl | InterfaceDecl | GlobalVarDecl | TypeAliasDecl ;

FunctionDecl    ::= [ "pure" ] "fun" Identifier "(" [ ParamList ] ")" [ ":" Type ] Block ;
ParamList       ::= Parameter { "," Parameter } ;
Parameter       ::= [ "var" ] Identifier ":" Type ;

ClassDecl       ::= "class" Identifier [ "implements" TypeList ] ClassBody ;
TypeList        ::= Type { "," Type } ;
ClassBody       ::= "{" { ClassMember } "}" ;
ClassMember     ::= FieldDecl | StaticFieldDecl | MethodDecl | DestructorDecl | CallDecl ;

FieldDecl       ::= ( "private" | "public" ) ( "val" | "var" ) Identifier ":" Type [ "=" Expression ] ";" ;
StaticFieldDecl ::= "static" ( "private" | "public" ) ( "val" | "var" ) Identifier ":" Type [ "=" Expression ] ";" ;

MethodDecl      ::= ( "private" | "public" ) [ "override" ] [ "pure" ]
                    "fun" Identifier "(" [ ParamList ] ")" [ ":" Type ] ( Block | ";" ) ;

CallDecl        ::= ( "private" | "public" ) "call" "(" [ ParamList ] ")" [ ":" Type ] ( Block | ";" ) ;

DestructorDecl  ::= ( "private" | "public" ) "destructor" "(" ")" ":" "Void" Block ;

InterfaceDecl   ::= "interface" Identifier InterfaceBody ;  // implicitly extends Object
InterfaceBody   ::= "{" { InterfaceMember } "}" ;
InterfaceMember ::= InterfaceMethod | InterfaceCall ;
InterfaceMethod ::= "fun" Identifier "(" [ ParamList ] ")" [ ":" Type ] ";" ;  // public & virtual
InterfaceCall   ::= "call" "(" [ ParamList ] ")" [ ":" Type ] ";" ;  // public & virtual

GlobalVarDecl   ::= [ "var" ] Identifier ":" Type "=" Expression ";" ;

TypeAliasDecl   ::= "typealias" Identifier "=" Type ";" ;

Type            ::= NullableType | NonNullType ;
NullableType    ::= NonNullType "?" ;
NonNullType     ::= FundamentalType
                 | PrimitiveRefType
                 | "String"
                 | "IntArray" | "FloatArray" | "BoolArray" | "CharArray" | "ByteArray" | "PointerArray"
                 | "ObjectArray"
                 | "StringArray"
                 | Identifier ;              // class/interface names (non-primitive)

FundamentalType ::= "int" | "float" | "bool" | "char" | "byte" | "pointer" ;
PrimitiveRefType ::= "Int" | "Float" | "Bool" | "Char" | "Byte" | "Pointer" ;

Block           ::= "{" { Statement } "}" ;
Statement       ::= VarDeclStmt | ExprStmt | ReturnStmt | IfStmt | WhileStmt | ForStmt | UnsafeStmt | Block ;

VarDeclStmt     ::= [ "var" ] Identifier ":" Type "=" Expression ";" ;
ExprStmt        ::= Expression ";" ;
ReturnStmt      ::= "return" [ Expression ] ";" ;
IfStmt          ::= "if" "(" Expression ")" Statement [ "else" Statement ] ;
WhileStmt       ::= "while" "(" Expression ")" Statement ;
ForStmt         ::= "for" "(" Identifier "in" Expression ")" Statement ;
UnsafeStmt      ::= "unsafe" Block ;

Expression      ::= Assignment ;
Assignment      ::= ElvisExpr [ ("=" | ":=") Assignment ] ;

ElvisExpr       ::= OrExpr [ "?:" ElvisExpr ] ;  // right-assoc

OrExpr          ::= AndExpr { "||" AndExpr } ;
AndExpr         ::= XorExpr { "&&" XorExpr } ;
XorExpr         ::= EqualityExpr { "xor" EqualityExpr } ;

EqualityExpr    ::= RelExpr { ("==" | "!=") RelExpr } ;
RelExpr         ::= AddExpr { ("<" | "<=" | ">" | ">=") AddExpr } ;
AddExpr         ::= MulExpr { ("+" | "-") MulExpr } ;
MulExpr         ::= UnaryExpr { ("*" | "/" | "%") UnaryExpr } ;

UnaryExpr       ::= ("!" | "-" | "&" | "*") UnaryExpr
                 | Postfix ;

Postfix         ::= Primary { PostfixOp } ;
PostfixOp       ::= "." Identifier
                 | "." Identifier "(" [ ArgList ] ")"
                 | "(" [ ArgList ] ")"          // function call or callable object call
                 | "as" Type                    // explicit cast; downcast yields nullable type
                 | "is" Type                    // type test → bool
                 | "?." Identifier [ "(" [ ArgList ] ")" ]  // safe call chain
                 | "?." "(" [ ArgList ] ")"     // safe callable object call
                 ;

Primary         ::= Identifier
                 | Literal
                 | "(" Expression ")"
                 | NamespaceRef
                 | FunctionLiteral ;

FunctionLiteral ::= "fun" "(" [ ParamList ] ")" [ ":" Type ] Block ;

NamespaceRef    ::= Identifier "::" Identifier ;
ArgList         ::= Expression { "," Expression } ;

Literal         ::= NumberLiteral | StringLiteral | CharLiteral | "true" | "false" ;
```
