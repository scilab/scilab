(*
 *  Translator from Modelica 2.x to flat Modelica
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

(** Modelica language lexer. *)

(** Implementation based on {i Modelica language specification 2.0 } *)

{

(** Modelica lexer. *)

(** Implementation based on {i Modelica language specification 2.0 } *)

open Parser

let check_reserved = function
    | "algorithm" -> ALGORITHM
    | "and" -> AND
    | "annotation" -> ANNOTATION
    | "block" -> MODEL
    | "break" -> BREAK
    | "class" -> CLASS
    | "connect" -> CONNECT
    | "connector" -> CONNECTOR
    | "constant" -> CONSTANT
    | "discrete" -> DISCRETE
    | "each" -> EACH
    | "else" -> ELSE
    | "elseif" -> ELSEIF
    | "elsewhen" -> ELSEWHEN
    | "encapsulated" -> ENCAPSULATED
    | "enumeration" -> ENUMERATION
    | "end" -> END
    | "equation" -> EQUATION
    | "expandable" -> EXPANDABLE
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
    | "noEvent" -> NOEVENT
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
    | "restricts" -> RESTRICTS
    | "return" -> RETURN
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
let qchar = [^'`' '\\']
let schar = [^'\"' '\\']
let sescape = "\\\'" | "\\\"" | "\\?" | "\\\\" | "\\a" | "\\b" | "\\f" |
              "\\n" | "\\r" | "\\t" | "\\v"

let comment = "/*" ( [^ '*'] | '*'+ [^ '*' '/'] )* '*'+ '/'
let line_comment = "//" [^ '\n']* '\n'

let separators = (blank | ['\n'] | comment | line_comment)+

let qident = '`' (qchar | sescape)+ '`'

let ident = nondigit (nondigit | digit)* | qident

let unsigned_integer = digit+

let fractional_constant = unsigned_integer? '.' unsigned_integer | unsigned_integer '.'

let exponent_part = ('e' | 'E') ('+' | '-')? unsigned_integer

let unsigned_real = fractional_constant exponent_part? | unsigned_integer exponent_part

rule token = parse

  | blank
              { token lexbuf }

  | ['\n']
              { token lexbuf }


  | comment
              { token lexbuf }

  | line_comment
              { token lexbuf }

  | unsigned_integer as lxm
              { UNSIGNED_INTEGER lxm }

  | unsigned_real as lxm
              { UNSIGNED_REAL lxm }

  | "initial" separators "algorithm"
              { INITIAL_ALGORITHM }

  | "initial" separators "equation"
              { INITIAL_EQUATION }

  | "end" separators "for"
              { END_FOR }

  | "end" separators "if"
              { END_IF }

  | "end" separators "when"
              { END_WHEN }

  | "end" separators "while"
              { END_WHILE }

  | "end" separators (ident as lxm)
              { END_IDENT lxm }

  | ident as lxm
              { check_reserved lxm }

  | '\"' ((schar | sescape)* as lxm) '\"'
              { STRING lxm }

  | '('       { LP }
  | ')'       { RP }
  | '['       { LSB }
  | ']'       { RSB }
  | '{'       { LCB }
  | '}'       { RCB }

  | '.'       { DOT }
  | ','       { CM }
  | ';'       { SC }
  | ':'       { CL }

  | '+'       { PLUS }
  | '-'       { MINUS }
  | '*'       { STAR }
  | '/'       { SLASH }
  | '^'       { EXP }

  | '='       { EQ }
  | ":="      { COLEQ }

  | '<'       { LT }
  | '>'       { GT }
  | "<="      { LE }
  | ">="      { GE }
  | "=="      { EE }
  | "<>"      { NE }

  | eof       { EOF }

  | _         { raise (SyntacticError
                  {err_msg = ["_IllegalCharacter"];
                   err_info = [];
                   err_ctx =
                     {location = {start = Lexing.lexeme_start lexbuf;
                                  enddd = Lexing.lexeme_end lexbuf;
                                  filename = !inputfile}}}) }
