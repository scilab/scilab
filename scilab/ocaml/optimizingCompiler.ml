
let sccs_id =
  "@(#)Modelica Compiler 1.3.3 - Copyright (C) 2003-2004 Tni-Valiosys"

let name = Sys.argv.(0)

let _ =
    Printf.printf "%s\n" (String.sub sccs_id 4 (String.length sccs_id - 4));
    flush stdout

let path = ref ""

let keep_variables = ref false

let compile_only = ref false

let directories = ref [""]

let output = ref ""

let input = ref ""

let add_lib_path s =
  directories := !directories @ [s]

let check_filename () =
  if Filename.check_suffix !input "mo" then ()
  else failwith "check_filename: Filename suffix must be 'mo'"

let set_path s =
  if !path <> "" then failwith "set_path: More than one path specified";
  path := Filename.dirname s

let set_output s =
  if !output <> "" then failwith "set_output: More than one output specified";
  output := s

let set_input s =
  if !input <> "" then failwith "set_input: More than one input specified";
  input := s

let construct_output_filename () =
  if !output = "" then begin
    output := Filename.chop_suffix !input "mo";
    if !compile_only then
      output := !output ^ "moc"
  end;
  !output

let run () =
  check_filename ();
  let ic = open_in !input in
  Printf.printf "Input file name = %s\n" !input; flush stdout;
  let lexbuf = Lexing.from_channel ic in
  Printf.printf "Parsing..."; flush stdout;
  let tree = Parser.parse Lexer.token lexbuf in
  Printf.printf " OK\nPrecompiling..."; flush stdout;
  let root = Precompilation.precompile tree in
  Printf.printf " OK\nCompiling..."; flush stdout;
  Compilation.paths := !directories;
  let cu = Compilation.compile_main_class root in
  let filename = construct_output_filename () in
  if !compile_only then begin
    Printf.printf " OK\nSaving..."; flush stdout;
    Compilation.write_class_file filename cu
  end else begin match cu with
    | Compilation.CompiledClass lccl ->
        let fun_name = Filename.chop_extension (Filename.basename filename)
        and ccl = Lazy.force lccl in
        Printf.printf " OK\nInstantiating..."; flush stdout;
        let iexpr =
          Instantiation.instantiate_main_class
            Instantiation.ToplevelContext
            []
            ccl
        in
        let model = Optimization.create_model iexpr in
        Printf.printf
          " OK\n%d variables in model.\n"
          (Array.length model.Optimization.variables);
        if not !keep_variables then begin
          Printf.printf "Optimizing..."; flush stdout;
          Optimization.perform_simplifications model;
          Printf.printf
            " OK\n%d variables remaining."
            (Array.fold_left
              (fun n variable ->
                if not variable.Optimization.solved then n + 1 else n)
              0
              model.Optimization.equations);
          Printf.printf "\nFinding subsystems...\n"; flush stdout;
          let compnts = Optimization.find_submodels model in
          Printf.printf "%d subsystems found.\n" (List.length compnts);
          flush stdout;
          List.iter
            (fun compnt ->
              Printf.printf "size = %d\n" (List.length compnt); flush stdout)
            compnts;
        end;
        Printf.printf "Generating code..."; flush stdout;
        ScicosCodeGeneration.generate_code !path filename fun_name model
    | Compilation.CompiledFunction _ ->
        failwith "Attempt to generate code for a function"
  end;
  Printf.printf " OK\nOutput file name = %s\n" !output; flush stdout;
  Printf.printf "Terminated.\n"; flush stdout

let () = Arg.parse
  [("-L", Arg.String add_lib_path,
  "<directory>  Add <directory> to the list of directories to be searched \
  when linking");
   ("-c", Arg.Set compile_only, "compile only (do not instantiate)");
   ("-o", Arg.String set_output,
    "<outputfile>  Set output file name to <outputfile>");
   ("-hpath", Arg.String set_path,
    "<path>  Specify a path to be added to #include directives");
   ("-keep-all-variables", Arg.Set keep_variables,
    "Don\'t remove alias variables");]
  set_input
  ("usage: modelicac [-c] [-o <outputfile>] <inputfile> [other options]")

let () = run ()
