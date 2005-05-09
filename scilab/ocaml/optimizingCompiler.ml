module type CODEGENERATOR =
  sig
    val generate_code: string -> string -> string -> Optimization.model -> bool
      -> unit
  end

module type S =
  sig
    val sccs_id: string
    val version: string
    val run: unit -> unit
  end

module Make(G: CODEGENERATOR): S =
  struct

    let sccs_id =
      "@(#)Modelicac - Copyright (C) 2003-2004 TNI-Valiosys, 2005 Imagine"

    let version = "1.5.8"

    let path = ref ""

    let keep_variables = ref false

    let compile_only = ref false

    let with_jac = ref false

    let directories = ref [""]

    let output = ref ""

    let input = ref ""

    let max_simplifs = ref max_int

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

    let set_max_simplifs i =
      max_simplifs := i

    let construct_output_filename () =
      if !output = "" then begin
        output := Filename.chop_suffix !input "mo";
        if !compile_only then
          output := !output ^ "moc"
      end;
      !output

    let parse_args () =
      Arg.parse
        [("-L", Arg.String add_lib_path,
          "<directory>  Add <directory> to the list of directories to be searched \
          while linking");
        ("-c", Arg.Set compile_only, "compile only (do not instantiate)");
        ("-o", Arg.String set_output,
          "<outputfile>  Set output file name to <outputfile>");
        ("-hpath", Arg.String set_path,
          "<path>  Specify a path to be added to #include directives");
        ("-keep-all-variables", Arg.Set keep_variables,
          "Don\'t remove alias variables");
        ("-max-simplifs", Arg.Int set_max_simplifs,
         "<passes> Max number of simplifications");
        ("-jac", Arg.Set with_jac, "Generate symbolic jacobian matrix")]
        set_input
        ("usage: modelicac [-c] [-o <outputfile>] <inputfile> [other options]")

    let run () =
      parse_args ();
      check_filename ();
      let ic = open_in !input in
      Printf.printf "Input file name = %s\n" !input; flush stdout;
      let lexbuf = Lexing.from_channel ic in
      Printf.printf "Parsing..."; flush stdout;
      let tree = Parser.parse !input Lexer.token lexbuf in
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
              " OK\n%d variable(s) in model.\n"
              (Array.length model.Optimization.variables);
            if not !keep_variables then begin
              Printf.printf "Removing trivial relations..."; flush stdout;
              max_simplifs :=
                Optimization.eliminate_trivial_relations !max_simplifs model;
              Printf.printf
                " OK\n%d variable(s) remaining."
                (Array.fold_left
                  (fun n variable ->
                    if not variable.Optimization.solved then n + 1 else n)
                  0
                  model.Optimization.equations); flush stdout;
              Printf.printf "\nOptimizing remaining equations..."; flush stdout;
              Optimization.perform_simplifications !max_simplifs model;
              Printf.printf
                " OK\n%d variable(s) remaining."
                (Array.fold_left
                  (fun n variable ->
                    if not variable.Optimization.solved then n + 1 else n)
                  0
                  model.Optimization.equations); flush stdout;
              Printf.printf "\nFinding subsystems...\n"; flush stdout;
              let compnts = Optimization.find_submodels model in
              Printf.printf "%d subsystem(s) found.\n" (List.length compnts);
              flush stdout;
              List.iter
                (fun compnt ->
                  Printf.printf "size = %d\n" (List.length compnt); flush stdout)
                compnts;
            end;
            Printf.printf "Generating code..."; flush stdout;
            G.generate_code !path filename fun_name model !with_jac
        | Compilation.CompiledFunction _ ->
            failwith "Attempt to generate code for a function"
      end;
      Printf.printf " OK\nOutput file name = %s\n" !output; flush stdout;
      Printf.printf "Terminated.\n"; flush stdout

  end
