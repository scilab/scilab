(*
 *  Modelicac
 *
 *  Copyright (C) 2005 - 2007 Imagine S.A.
 *  For more information or commercial use please contact us at www.amesim.com
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *)

{
open Parser

exception Eof

let check_reserved = function
    | "algorithm" -> ALGORITHM
    | "and" -> AND
    | "annotation" -> ANNOTATION
    | "assert" -> ASSERT
    | "block" -> BLOCK
    | "class" -> CLASS
    | "connect" -> CONNECT
    | "connector" -> CONNECTOR
    | "constant" -> CONSTANT
    | "discrete" -> DISCRETE
    | "else" -> ELSE
    | "elseif" -> ELSEIF
    | "elsewhen" -> ELSEWHEN
    | "encapsulated" -> ENCAPSULATED
    | "end" -> END
    | "equation" -> EQUATION
    | "extends" -> EXTENDS
    | "external" -> EXTERNAL
    | "false" -> FALSE
    | "final" -> FINAL
    | "flow" -> FLOW
    | "for" -> FOR
    | "function" -> FUNCTION
    | "if" -> IF
    | "import" -> IMPORT
    | "in" -> IN
    | "inner" -> INNER
    | "input" -> INPUT
    | "loop" -> LOOP
    | "model" -> MODEL
    | "not" -> NOT
    | "or" -> OR
    | "outer" -> OUTER
    | "output" -> OUTPUT
    | "package" -> PACKAGE
    | "parameter" -> PARAMETER
    | "partial" -> PARTIAL
    | "protected" -> PROTECTED
    | "public" -> PUBLIC
    | "record" -> RECORD
    | "redeclare" -> REDECLARE
    | "replaceable" -> REPLACEABLE
    | "terminate" -> TERMINATE
    | "then" -> THEN
    | "true" -> TRUE
    | "type" -> TYPE
    | "when" -> WHEN
    | "while" -> WHILE
    | "within" -> WITHIN
    | s -> IDENT s

}

let blank = [' ' '\t' '\r']
let digit = ['0'-'9']
let nondigit = ['_' 'A'-'Z' 'a'-'z']
let nondigit2 =  ['_' 'A'-'Z' 'a'-'z' '[' ']' '.']
let schar = [^'\"' '\\']
let sescape = "\\\'" | "\\\"" | "\\?" | "\\\\" | "\\a" | "\\b" | "\\f" |
              "\\n" | "\\r" | "\\t" | "\\v"

let unsigned_integer = digit+

let unsigned_number =
  unsigned_integer ('.' unsigned_integer?)?
    (('e' | 'E')('+' | '-')? unsigned_integer)?

rule token = parse

    | blank
                { token lexbuf }

    | ['\n']
                { token lexbuf }


    | "/*" ( [^ '*'] | '*'+ [^ '*' '/'] )* '*'+ '/'
                { let _ = Lexing.lexeme lexbuf in
                  token lexbuf }

    | "//" [^ '\n']* '\n'
                { let _ = Lexing.lexeme lexbuf in
                  token lexbuf }

    | unsigned_integer
                { let lxm = Lexing.lexeme lexbuf in
                    UNSIGNED_INTEGER lxm }

    | unsigned_number
                { let lxm = Lexing.lexeme lexbuf in
                    UNSIGNED_NUMBER lxm }

    | nondigit (nondigit | digit)*
                { let lxm = Lexing.lexeme lexbuf in
                    check_reserved lxm }

    | '`' (nondigit2 | digit)+ '`'
                { let lxm = Lexing.lexeme lexbuf in
                    IDENT (String.sub lxm 1 (String.length lxm - 2)) }

    | '\"' (schar | sescape)* '\"'
                { let lxm = Lexing.lexeme lexbuf in
                    STRING (String.sub lxm 1 (String.length lxm - 2)) }

    | '('           { LP }
    | ')'           { RP }
    | '['           { LSB }
    | ']'           { RSB }
    | '{'           { LCB }
    | '}'           { RCB }

    | '.'           { DOT }
    | ','           { CM }
    | ';'           { SC }
    | ':'           { CL }

    | '+'           { PLUS }
    | '-'           { MINUS }
    | '*'           { STAR }
    | '/'           { SLASH }
    | '^'           { EXP }

    | '='           { EQ }
    | ":="          { COLEQ }

    | '<'           { LT }
    | '>'           { GT }
    | "<="          { LE }
    | ">="          { GE }
    | "=="          { EE }
    | "<>"          { NE }

    | eof           { EOF }
