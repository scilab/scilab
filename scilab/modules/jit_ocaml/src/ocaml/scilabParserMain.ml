let file = ref ""
let test_flag = ref false
let args = [("-t", Arg.Unit (fun () -> test_flag := true), ": run tests")]
let usage = "Usage: ./main -t [fichier]  (stdin par default)"

let run_test file =
  let ch = if file = "" then stdin else open_in file in
  try
    let lexbuf = Lexing.from_channel ch in
    let ast = ScilabParser.program ScilabLexer.token lexbuf in
    begin
      match ast with
        | ScilabAst.Exp exp -> print_endline (ScilabAstPrinter.to_string exp)
        | _ -> ()
    end;
    flush stdout
  with Not_found ->
    Printf.printf "catch exn\n"

let run_tests dirname =
  let files = Sys.readdir dirname in
  Printf.printf "# tests to run : %i\n\n" (Array.length files);
  Array.iter (fun file ->
    let file = Filename.concat dirname file in
    Printf.printf "Testing %s :\n" file;
    if Filename.check_suffix file ".sci" then
      run_test file
  ) files

let _ =
  Arg.parse args (fun s -> file := s) usage;
  if !test_flag
  then
    begin
      let dir_tests =
        ["test/";
         (* "/home/michael/dev_sci/richelieu/code_samples/declarations/"; *)
         (* "../../test/good/"; *)] in
      List.iter (run_tests) dir_tests
    end
  else run_test !file





