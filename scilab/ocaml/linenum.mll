
{

let linenum = ref 0
let linebeg = ref 0

}

rule skip_line = parse
  | [^ '\n' '\r']* ('\n' | '\r' | "\r\n")
      { incr linenum;
        linebeg := Lexing.lexeme_start lexbuf;
        Lexing.lexeme_end lexbuf }
  | [^ '\n' '\r']* eof
      { incr linenum;
        linebeg := Lexing.lexeme_start lexbuf;
        raise End_of_file }

{

let for_position file loc =
  let ic = open_in_bin file in
  let lb = Lexing.from_channel ic in
  linenum := 1;
  linebeg := 0;
  begin try
    while skip_line lb <= loc do () done
  with End_of_file -> ()
  end;
  close_in ic;
  (!linenum - 1, !linebeg)

}
