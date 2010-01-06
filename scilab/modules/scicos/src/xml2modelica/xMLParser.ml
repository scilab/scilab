type token =
  | IDENT of (string)
  | UNSIGNED_INTEGER of (string)
  | UNSIGNED_NUMBER of (string)
  | STRING of (string)
  | MODEL_TAG
  | MODEL_END_TAG
  | NAME_TAG
  | NAME_END_TAG
  | ELEMENTS_TAG
  | ELEMENT_STRUCT_TAG
  | ELEMENT_STRUCT_SUBNODES_TAG
  | ELEMENT_STRUCT_SUBNODES_END_TAG
  | ELEMENT_STRUCT_END_TAG
  | ELEMENT_TERMINAL_TAG
  | ELEMENT_TERMINAL_KIND_TAG
  | ELEMENT_TERMINAL_KIND_END_TAG
  | ELEMENT_TERMINAL_ID_TAG
  | ELEMENT_TERMINAL_ID_END_TAG
  | ELEMENT_TERMINAL_END_TAG
  | ELEMENTS_END_TAG
  | EQUATIONS_TAG
  | EQUATIONS_END_TAG
  | WHEN_CLAUSES_TAG
  | WHEN_CLAUSES_END_TAG
  | ELEMENT_TERMINAL_OUTPUT_TAG
  | ELEMENT_TERMINAL_SELECT_TAG
  | ELEMENT_TERMINAL_COMMENT_VALUE_TAG of (string)
  | ELEMENT_TERMINAL_INITIAL_VALUE_VALUE_TAG of (string)
  | ELEMENT_TERMINAL_NOMINAL_VALUE_VALUE_TAG of (string)
  | ELEMENT_TERMINAL_FIXED_VALUE_TAG of (string)
  | EQUATION_VALUE_TAG of (string)
  | WHEN_CLAUSE_VALUE_TAG of (string)
  | ERROR
  | EOF

open Parsing;;
# 28 "xMLParser.mly"


  open XMLTree

# 44 "xMLParser.ml"
let yytransl_const = [|
  261 (* MODEL_TAG *);
  262 (* MODEL_END_TAG *);
  263 (* NAME_TAG *);
  264 (* NAME_END_TAG *);
  265 (* ELEMENTS_TAG *);
  266 (* ELEMENT_STRUCT_TAG *);
  267 (* ELEMENT_STRUCT_SUBNODES_TAG *);
  268 (* ELEMENT_STRUCT_SUBNODES_END_TAG *);
  269 (* ELEMENT_STRUCT_END_TAG *);
  270 (* ELEMENT_TERMINAL_TAG *);
  271 (* ELEMENT_TERMINAL_KIND_TAG *);
  272 (* ELEMENT_TERMINAL_KIND_END_TAG *);
  273 (* ELEMENT_TERMINAL_ID_TAG *);
  274 (* ELEMENT_TERMINAL_ID_END_TAG *);
  275 (* ELEMENT_TERMINAL_END_TAG *);
  276 (* ELEMENTS_END_TAG *);
  277 (* EQUATIONS_TAG *);
  278 (* EQUATIONS_END_TAG *);
  279 (* WHEN_CLAUSES_TAG *);
  280 (* WHEN_CLAUSES_END_TAG *);
  281 (* ELEMENT_TERMINAL_OUTPUT_TAG *);
  282 (* ELEMENT_TERMINAL_SELECT_TAG *);
  289 (* ERROR *);
    0 (* EOF *);
    0|]

let yytransl_block = [|
  257 (* IDENT *);
  258 (* UNSIGNED_INTEGER *);
  259 (* UNSIGNED_NUMBER *);
  260 (* STRING *);
  283 (* ELEMENT_TERMINAL_COMMENT_VALUE_TAG *);
  284 (* ELEMENT_TERMINAL_INITIAL_VALUE_VALUE_TAG *);
  285 (* ELEMENT_TERMINAL_NOMINAL_VALUE_VALUE_TAG *);
  286 (* ELEMENT_TERMINAL_FIXED_VALUE_TAG *);
  287 (* EQUATION_VALUE_TAG *);
  288 (* WHEN_CLAUSE_VALUE_TAG *);
    0|]

let yylhs = "\255\255\
\001\000\002\000\003\000\005\000\008\000\008\000\009\000\009\000\
\010\000\004\000\012\000\011\000\013\000\014\000\018\000\018\000\
\016\000\016\000\015\000\015\000\017\000\017\000\019\000\019\000\
\020\000\020\000\006\000\021\000\021\000\022\000\007\000\007\000\
\023\000\023\000\024\000\000\000"

let yylen = "\002\000\
\002\000\003\000\004\000\003\000\000\000\002\000\003\000\003\000\
\002\000\003\000\003\000\009\000\003\000\003\000\000\000\001\000\
\000\000\001\000\000\000\001\000\000\000\001\000\000\000\001\000\
\000\000\001\000\003\000\000\000\002\000\001\000\000\000\003\000\
\000\000\002\000\001\000\002\000"

let yydefred = "\000\000\
\000\000\000\000\000\000\036\000\000\000\000\000\000\000\000\000\
\001\000\000\000\002\000\005\000\000\000\010\000\000\000\028\000\
\000\000\000\000\000\000\004\000\006\000\000\000\033\000\003\000\
\000\000\000\000\000\000\000\000\027\000\030\000\029\000\000\000\
\005\000\009\000\007\000\000\000\000\000\008\000\032\000\035\000\
\034\000\000\000\000\000\000\000\000\000\011\000\013\000\000\000\
\020\000\000\000\014\000\018\000\000\000\022\000\000\000\016\000\
\000\000\024\000\000\000\026\000\012\000"

let yydgoto = "\002\000\
\004\000\005\000\007\000\008\000\013\000\017\000\024\000\015\000\
\021\000\026\000\028\000\034\000\037\000\045\000\050\000\053\000\
\055\000\057\000\059\000\061\000\022\000\031\000\032\000\041\000"

let yysindex = "\002\000\
\255\254\000\000\010\255\000\000\019\000\021\255\017\255\023\255\
\000\000\025\255\000\000\000\000\013\255\000\000\004\255\000\000\
\019\255\010\255\010\255\000\000\000\000\250\254\000\000\000\000\
\024\255\027\255\030\255\028\255\000\000\000\000\000\000\253\254\
\000\000\000\000\000\000\043\255\031\255\000\000\000\000\000\000\
\000\000\029\255\033\255\045\255\020\255\000\000\000\000\034\255\
\000\000\026\255\000\000\000\000\022\255\000\000\032\255\000\000\
\035\255\000\000\036\255\000\000\000\000"

let yyrindex = "\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\047\255\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\237\254\000\000\000\000\000\000\
\000\000\242\254\000\000\000\000\001\255\000\000\011\255\000\000\
\012\255\000\000\037\255\000\000\000\000"

let yygindex = "\000\000\
\000\000\000\000\000\000\239\255\000\000\000\000\000\000\022\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"

let yytablesize = 62
let yytable = "\019\000\
\025\000\027\000\001\000\003\000\017\000\019\000\019\000\019\000\
\019\000\019\000\017\000\017\000\017\000\018\000\017\000\029\000\
\006\000\019\000\009\000\021\000\039\000\010\000\011\000\020\000\
\030\000\021\000\021\000\021\000\040\000\015\000\023\000\012\000\
\014\000\016\000\033\000\015\000\015\000\023\000\018\000\035\000\
\046\000\023\000\019\000\043\000\036\000\048\000\038\000\044\000\
\047\000\049\000\054\000\051\000\031\000\052\000\042\000\025\000\
\000\000\000\000\056\000\058\000\000\000\060\000"

let yycheck = "\019\001\
\018\000\019\000\001\000\005\001\019\001\025\001\026\001\027\001\
\028\001\029\001\025\001\026\001\027\001\010\001\029\001\022\001\
\007\001\014\001\000\000\019\001\024\001\001\001\006\001\020\001\
\031\001\025\001\026\001\027\001\032\001\019\001\019\001\009\001\
\008\001\021\001\011\001\025\001\026\001\026\001\010\001\013\001\
\012\001\023\001\014\001\001\001\015\001\001\001\019\001\017\001\
\016\001\030\001\029\001\018\001\006\001\028\001\033\000\019\001\
\255\255\255\255\027\001\025\001\255\255\026\001"

let yynames_const = "\
  MODEL_TAG\000\
  MODEL_END_TAG\000\
  NAME_TAG\000\
  NAME_END_TAG\000\
  ELEMENTS_TAG\000\
  ELEMENT_STRUCT_TAG\000\
  ELEMENT_STRUCT_SUBNODES_TAG\000\
  ELEMENT_STRUCT_SUBNODES_END_TAG\000\
  ELEMENT_STRUCT_END_TAG\000\
  ELEMENT_TERMINAL_TAG\000\
  ELEMENT_TERMINAL_KIND_TAG\000\
  ELEMENT_TERMINAL_KIND_END_TAG\000\
  ELEMENT_TERMINAL_ID_TAG\000\
  ELEMENT_TERMINAL_ID_END_TAG\000\
  ELEMENT_TERMINAL_END_TAG\000\
  ELEMENTS_END_TAG\000\
  EQUATIONS_TAG\000\
  EQUATIONS_END_TAG\000\
  WHEN_CLAUSES_TAG\000\
  WHEN_CLAUSES_END_TAG\000\
  ELEMENT_TERMINAL_OUTPUT_TAG\000\
  ELEMENT_TERMINAL_SELECT_TAG\000\
  ERROR\000\
  EOF\000\
  "

let yynames_block = "\
  IDENT\000\
  UNSIGNED_INTEGER\000\
  UNSIGNED_NUMBER\000\
  STRING\000\
  ELEMENT_TERMINAL_COMMENT_VALUE_TAG\000\
  ELEMENT_TERMINAL_INITIAL_VALUE_VALUE_TAG\000\
  ELEMENT_TERMINAL_NOMINAL_VALUE_VALUE_TAG\000\
  ELEMENT_TERMINAL_FIXED_VALUE_TAG\000\
  EQUATION_VALUE_TAG\000\
  WHEN_CLAUSE_VALUE_TAG\000\
  "

let yyact = [|
  (fun _ -> failwith "parser")
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'definition) in
    Obj.repr(
# 65 "xMLParser.mly"
                                                                ( _1 )
# 207 "xMLParser.ml"
               : XMLTree.t))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'model_definition) in
    Obj.repr(
# 69 "xMLParser.mly"
                                                                ( _2 )
# 214 "xMLParser.ml"
               : 'definition))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'name) in
    let _2 = (Parsing.peek_val __caml_parser_env 2 : 'elements_definition) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'equations_definition) in
    let _4 = (Parsing.peek_val __caml_parser_env 0 : 'when_clauses_definition) in
    Obj.repr(
# 74 "xMLParser.mly"
                                                                ( { name = _1; elements = _2; equations = _3; when_clauses = _4 } )
# 224 "xMLParser.ml"
               : 'model_definition))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'element_list) in
    Obj.repr(
# 78 "xMLParser.mly"
                                                                ( List.rev _2 )
# 231 "xMLParser.ml"
               : 'elements_definition))
; (fun __caml_parser_env ->
    Obj.repr(
# 82 "xMLParser.mly"
                                                                ( [] )
# 237 "xMLParser.ml"
               : 'element_list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'element_list) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'element_definition) in
    Obj.repr(
# 83 "xMLParser.mly"
                                                                ( _2 :: _1 )
# 245 "xMLParser.ml"
               : 'element_list))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'struct_contents) in
    Obj.repr(
# 87 "xMLParser.mly"
                                                                ( Struct _2 )
# 252 "xMLParser.ml"
               : 'element_definition))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'terminal_contents) in
    Obj.repr(
# 89 "xMLParser.mly"
                                                                ( Terminal _2 )
# 259 "xMLParser.ml"
               : 'element_definition))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'name) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'subnodes_definition) in
    Obj.repr(
# 93 "xMLParser.mly"
                                                                ( { struct_name = _1; subnodes = _2 } )
# 267 "xMLParser.ml"
               : 'struct_contents))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    Obj.repr(
# 97 "xMLParser.mly"
                                                                    ( _2 )
# 274 "xMLParser.ml"
               : 'name))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'element_list) in
    Obj.repr(
# 102 "xMLParser.mly"
                                                                ( List.rev _2 )
# 281 "xMLParser.ml"
               : 'subnodes_definition))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 8 : 'name) in
    let _2 = (Parsing.peek_val __caml_parser_env 7 : 'kind) in
    let _3 = (Parsing.peek_val __caml_parser_env 6 : 'id) in
    let _4 = (Parsing.peek_val __caml_parser_env 5 : 'fixed_value) in
    let _5 = (Parsing.peek_val __caml_parser_env 4 : 'initial_value_value) in
    let _6 = (Parsing.peek_val __caml_parser_env 3 : 'nominal_value_value) in
    let _7 = (Parsing.peek_val __caml_parser_env 2 : 'comment_value) in
    let _8 = (Parsing.peek_val __caml_parser_env 1 : 'output) in
    let _9 = (Parsing.peek_val __caml_parser_env 0 : 'select) in
    Obj.repr(
# 108 "xMLParser.mly"
                                          ( { terminal_name = _1;
                                              kind = _2;
                                              id = _3; 
                                              comment = _7;
                                              initial_value = _5;
                                              nominal = _6;
                                              output = _8;
                                              select = _9;
                                              fixed = _4 } )
# 304 "xMLParser.ml"
               : 'terminal_contents))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    Obj.repr(
# 121 "xMLParser.mly"
                                                                ( match _2 with
                                                                    | "input" -> Input
                                                                    | "fixed_parameter" -> FixedParameter
                                                                    | "parameter" -> Parameter
                                                                    | "variable" -> Variable
                                                                    | "discrete_variable" -> DiscreteVariable
                                                                    | _ -> raise Parsing.Parse_error )
# 317 "xMLParser.ml"
               : 'kind))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    Obj.repr(
# 132 "xMLParser.mly"
                                                                ( _2 )
# 324 "xMLParser.ml"
               : 'id))
; (fun __caml_parser_env ->
    Obj.repr(
# 136 "xMLParser.mly"
                                                                ( "" )
# 330 "xMLParser.ml"
               : 'comment_value))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 137 "xMLParser.mly"
                                                                ( _1 )
# 337 "xMLParser.ml"
               : 'comment_value))
; (fun __caml_parser_env ->
    Obj.repr(
# 141 "xMLParser.mly"
                                                                ( "" )
# 343 "xMLParser.ml"
               : 'initial_value_value))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 142 "xMLParser.mly"
                                                                ( _1 )
# 350 "xMLParser.ml"
               : 'initial_value_value))
; (fun __caml_parser_env ->
    Obj.repr(
# 146 "xMLParser.mly"
                                                                ( "" )
# 356 "xMLParser.ml"
               : 'fixed_value))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 147 "xMLParser.mly"
                                                                ( _1 )
# 363 "xMLParser.ml"
               : 'fixed_value))
; (fun __caml_parser_env ->
    Obj.repr(
# 151 "xMLParser.mly"
                                                                ( "" )
# 369 "xMLParser.ml"
               : 'nominal_value_value))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 152 "xMLParser.mly"
                                                                ( _1 )
# 376 "xMLParser.ml"
               : 'nominal_value_value))
; (fun __caml_parser_env ->
    Obj.repr(
# 156 "xMLParser.mly"
                                                                ( false )
# 382 "xMLParser.ml"
               : 'output))
; (fun __caml_parser_env ->
    Obj.repr(
# 157 "xMLParser.mly"
                                                                ( true )
# 388 "xMLParser.ml"
               : 'output))
; (fun __caml_parser_env ->
    Obj.repr(
# 161 "xMLParser.mly"
                                                                ( false )
# 394 "xMLParser.ml"
               : 'select))
; (fun __caml_parser_env ->
    Obj.repr(
# 162 "xMLParser.mly"
                                                                ( true )
# 400 "xMLParser.ml"
               : 'select))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'equation_list) in
    Obj.repr(
# 166 "xMLParser.mly"
                                                                ( List.rev _2 )
# 407 "xMLParser.ml"
               : 'equations_definition))
; (fun __caml_parser_env ->
    Obj.repr(
# 170 "xMLParser.mly"
                                                                ( [] )
# 413 "xMLParser.ml"
               : 'equation_list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'equation_list) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'equation_definition) in
    Obj.repr(
# 171 "xMLParser.mly"
                                                                ( _2 :: _1 )
# 421 "xMLParser.ml"
               : 'equation_list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 175 "xMLParser.mly"
                                                                ( _1 )
# 428 "xMLParser.ml"
               : 'equation_definition))
; (fun __caml_parser_env ->
    Obj.repr(
# 179 "xMLParser.mly"
                                                                ( [] )
# 434 "xMLParser.ml"
               : 'when_clauses_definition))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'when_clause_list) in
    Obj.repr(
# 180 "xMLParser.mly"
                                                                ( List.rev _2 )
# 441 "xMLParser.ml"
               : 'when_clauses_definition))
; (fun __caml_parser_env ->
    Obj.repr(
# 184 "xMLParser.mly"
                                                                ( [] )
# 447 "xMLParser.ml"
               : 'when_clause_list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'when_clause_list) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'when_clause_definition) in
    Obj.repr(
# 185 "xMLParser.mly"
                                                                ( _2 :: _1 )
# 455 "xMLParser.ml"
               : 'when_clause_list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 189 "xMLParser.mly"
                                                                ( _1 )
# 462 "xMLParser.ml"
               : 'when_clause_definition))
(* Entry xML_definition_eof *)
; (fun __caml_parser_env -> raise (Parsing.YYexit (Parsing.peek_val __caml_parser_env 0)))
|]
let yytables =
  { Parsing.actions=yyact;
    Parsing.transl_const=yytransl_const;
    Parsing.transl_block=yytransl_block;
    Parsing.lhs=yylhs;
    Parsing.len=yylen;
    Parsing.defred=yydefred;
    Parsing.dgoto=yydgoto;
    Parsing.sindex=yysindex;
    Parsing.rindex=yyrindex;
    Parsing.gindex=yygindex;
    Parsing.tablesize=yytablesize;
    Parsing.table=yytable;
    Parsing.check=yycheck;
    Parsing.error_function=parse_error;
    Parsing.names_const=yynames_const;
    Parsing.names_block=yynames_block }
let xML_definition_eof (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (Parsing.yyparse yytables 1 lexfun lexbuf : XMLTree.t)
;;
# 192 "xMLParser.mly"


let parse filename token_fun lexbuf =
  try
    xML_definition_eof token_fun lexbuf
  with
    | Parsing.Parse_error ->
        let linenum, linebeg =
          Linenum.for_position filename (Lexing.lexeme_start lexbuf)
        and linenum', linebeg' =
          Linenum.for_position filename (Lexing.lexeme_end lexbuf)
        in
        let first_char = Lexing.lexeme_start lexbuf - linebeg
        and first_char' = Lexing.lexeme_end lexbuf - linebeg'
        in
        Printf.printf
          "Syntax error.\n\
          Start: %d and %d\n\
          Characters %d and %d\n\
          Line %d, character %d.\n\
          Line %d, character %d.\n"
          linebeg
          linebeg'
          (Lexing.lexeme_start lexbuf) (Lexing.lexeme_end lexbuf)
          linenum first_char
          linenum' first_char';
        raise Exit
# 515 "xMLParser.ml"
