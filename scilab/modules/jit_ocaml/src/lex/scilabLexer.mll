{

  open Lexing
  open ScilabParser

  let end_cmt lexbuf =
    lexbuf.lex_curr_pos <- lexbuf.lex_start_pos;
    lexbuf.lex_curr_p <- { lexbuf.lex_curr_p with
      pos_cnum = lexbuf.lex_start_p.pos_cnum;
    }


  let print_pos pos =
    Printf.printf "%i %i %i" pos.pos_lnum pos.pos_bol pos.pos_cnum

  let print_lexbuf lexbuf =
    Printf.printf "st :"; print_pos lexbuf.lex_start_p;
    Printf.printf "; curr :"; print_pos lexbuf.lex_curr_p;
    Printf.printf "; st_pos :%i" lexbuf.lex_start_pos;
    Printf.printf "; curr_pos :%i \n" lexbuf.lex_curr_pos

  let str_cmt = ref ""

  let str = ref ""

  let is_transposable = ref false

}

let newline = ('\010' | '\013' | "\013\010")
let blank = [' ' '\009' '\012']

let spaces    = [' ' '\t']

let integer   = ['0'-'9']+
let number    = ['0'-'9']+(".")['0'-'9']*
let little    = (".")['0'-'9']+

let utf2  = ['\xC2'-'\xDF']['\x80'-'\xBF']

let utf31 = ['\xE0']['\xA0'-'\xBF']['\x80'-'\xBF']
let utf32 = ['\xE1'-'\xEC']['\x80'-'\xBF']['\x80'-'\xBF']
let utf33 = ['\xED']['\x80'-'\x9F']['\x80'-'\xBF']
let utf34 = ['\xEE'-'\xEF']['\x80'-'\xBF']['\x80'-'\xBF']
let utf41 = ['\xF0']['\x90'-'\xBF']['\x80'-'\xBF']['\x80'-'\xBF']
let utf42 = ['\xF1'-'\xF3']['\x80'-'\xBF']['\x80'-'\xBF']['\x80'-'\xBF']
let utf43 = ['\xF4']['\x80'-'\x8F']['\x80'-'\xBF']['\x80'-'\xBF']

let utf3 = utf31 | utf32 | utf33 | utf34
let utf4 = utf41 | utf42 | utf43

let utf  =  utf2 | utf3 | utf4

let id1  = ['a'-'z''A'-'Z''_''%''#''?''$'] | utf
let id2  = ['a'-'z''A'-'Z''_''0'-'9''#''?''$'] | utf
let id   = id1 id2*
(* let id   = ['a'-'z''A'-'Z''_''%''#''?''$']['a'-'z''A'-'Z''_''0'-'9''#''?''$'] * *)

let boolnot    = "@" | "~"
let booltrue   = "%t" | "%T"
let boolfalse  = "%f" | "%F"
let booland    = "&"
let boolandand = "&&"

let lbrack = "["
let rbrack = "]"

let lparen = "("
let rparen = ")"

let lbrace = "{"
let rbrace = "}"

let dollar = "$"

let semicolon = ";"
let comma     = ","
let colon     = ":"

let startlinecomment  = "//"
let startblockcomment = "/*"
let endblockcomment   = "*/"

let dquote = "\""
let quote  = "'"

let dot        = "."
let dotquote   = ".'"
let dottimes   = ".*"
let dotrdivide = "./"
let dotldivide = ".\\"
let dotpower   = ".^" | ".**"

let plus    = "+"
let minus   = "-"
let rdivide = "/"
let ldivide = "\\"
let times   = "*"
let power   = "^" | "**"

let equal        = "=="
let notequal     = "~=" | "@=" | "<>"
let lowerthan    = "<"
let greaterthan	 = ">"
let lowerequal   = "<="
let greaterequal = ">="

let assign = "="


rule token = parse
  | blank                        { is_transposable := false; token lexbuf }
  | newline                      { (* Printf.printf "EOL\n"; *) is_transposable := false; Lexing.new_line lexbuf; EOL}
  | startlinecomment             { is_transposable := false; str_cmt := ""; comment lexbuf }
  | dquote                       { is_transposable := false; str := ""; doublestr lexbuf }
  | quote                        { if !is_transposable 
                                   then QUOTE 
                                   else begin str := ""; simplestr lexbuf end}
  | "if"                         { is_transposable := false; IF }
  | "then"                       { is_transposable := false; THEN }
  | "else"                       { is_transposable := false; ELSE }
  | "elseif"                     { is_transposable := false; ELSEIF }
  | "end"                        { is_transposable := false; END }
  | "select"                     { is_transposable := false; SELECT }
  | "switch"                     { is_transposable := false; SWITCH }
  | "otherwise"                  { is_transposable := false; OTHERWISE }
  | "case"                       { is_transposable := false; CASE }
  | "while"                      { is_transposable := false; WHILE }
  | "do"                         { is_transposable := false; DO }
  | "try"                        { is_transposable := false; TRY }
  | "catch"                      { is_transposable := false; CATCH }
  | "return"                     { is_transposable := false; RETURN }
  | "break"                      { is_transposable := false; BREAK }
  | "continue"                   { is_transposable := false; CONTINUE }
  | "="                          { is_transposable := false; ASSIGN }
  | "for"                        { is_transposable := false; FOR }
  | "hidden"                     { is_transposable := false; HIDDEN }
  | "function"                   { is_transposable := false; FUNCTION }
  | "#function"                  { is_transposable := false; HIDDENFUNCTION }
  | "endfunction"                { is_transposable := false; ENDFUNCTION }
  | dot                          { is_transposable := false; DOT }
  | plus                         { is_transposable := false; PLUS }
  | minus                        { is_transposable := false; MINUS }
  | rdivide                      { is_transposable := false; RDIVIDE }
  | ldivide                      { is_transposable := false; LDIVIDE }
  | times                        { is_transposable := false; TIMES }
  | power                        { is_transposable := false; POWER }
  | equal                        { is_transposable := false; EQ }
  | notequal                     { is_transposable := false; NE }
  | lowerthan                    { is_transposable := false; LT }
  | greaterthan                  { is_transposable := false; GT }
  | lowerequal                   { is_transposable := false; LE }
  | greaterequal                 { is_transposable := false; GE }
  | comma                        { is_transposable := false; COMMA }
  | semicolon                    { is_transposable := false; SEMI }
  | colon                        { is_transposable := false; COLON }
  | integer as inum              { is_transposable := true;
                                   let num = float_of_string inum in
                                   Printf.printf "varint[%f]" num; VARINT num }
  | number as nnum               { is_transposable := true;
                                   let num = float_of_string nnum in
                                   NUM num }
  | little as lnum               { is_transposable := true;
                                   let num = float_of_string lnum in
                                   NUM num }
  | lparen                       { is_transposable := false; LPAREN }
  | rparen                       { is_transposable := true; RPAREN }
  | lbrace                       { is_transposable := false; LBRACE }
  | rbrace                       { is_transposable := true; RBRACE }
  | lbrack                       { is_transposable := false; LBRACK }
  | rbrack                       { is_transposable := true; RBRACK }
  | dollar                       { is_transposable := false; DOLLAR }
  | boolnot                      { is_transposable := false; NOT }
  | booltrue                     { is_transposable := true; BOOLTRUE }
  | boolfalse                    { is_transposable := true; BOOLFALSE }
  | booland                      { is_transposable := false; AND }
  | boolandand                   { is_transposable := false; ANDAND }
  | id as str                    { is_transposable := true;Printf.printf "ID[%s]" str; ID str }
  | eof                          { is_transposable := false; EOF }
  | _ as c                       { Printf.printf "Lexing error : Unknow character \'%c\'" c;exit 1}

and comment = parse
  | newline                      { (* Printf.printf "//%s" !str_cmt; *) end_cmt lexbuf; COMMENT !str_cmt}
  | eof                          { COMMENT !str_cmt ; }
  | _ as c                       { str_cmt := !str_cmt^(String.make 1 c); comment lexbuf }

and doublestr = parse
  | dquote                       { STR !str}
  | dquote dquote                { str := !str^"\""; doublestr lexbuf }
  | dquote quote                 { str := !str^"\'"; doublestr lexbuf }
  | quote dquote                 { str := !str^"\""; doublestr lexbuf }
  | quote quote                  { str := !str^"\'"; doublestr lexbuf }
  | quote                        { failwith "Error : Heterogeneous string detected, starting with \" and ending with \'." }
  | newline                      { failwith "Error : unexpected newline in a string." }
  | eof                          { failwith "Error : unexpected end of file in a string." }
  | _ as c                       { str := !str^(String.make 1 c); doublestr lexbuf }

and simplestr = parse
  | quote                        { STR !str}
  | dquote dquote                { str := !str^"\""; simplestr lexbuf }
  | dquote quote                 { str := !str^"\'"; simplestr lexbuf }
  | quote dquote                 { str := !str^"\""; simplestr lexbuf }
  | quote quote                  { str := !str^"\'"; simplestr lexbuf }
  | dquote                       { failwith "Error : Heterogeneous string detected, starting with \' and ending with \"." }
  | newline                      { failwith "Error : unexpected newline in a string." }
  | eof                          { failwith "Error : unexpected end of file in a string." }
  | _ as c                       { str := !str^(String.make 1 c); simplestr lexbuf }

(* and matrix = parse *)
(*   | spaces+ *)
(*       { Printf.printf " "; *)
(*         matrix lexbuf} *)
(*   | integer as inum *)
(*       { let num = int_of_string inum in *)
(*         Printf.printf "%d" num; *)
(*         matrix lexbuf} *)
(*   | number as nnum *)
(*       { let num = float_of_string nnum in *)
(*         Printf.printf "%f" num; *)
(*         matrix lexbuf} *)
(*   | little as lnum *)
(*       { let num = float_of_string lnum in *)
(*         Printf.printf "%f" num; *)
(*         matrix lexbuf} *)
(*   | rbrack *)
(*       {token lexbuf} *)


















