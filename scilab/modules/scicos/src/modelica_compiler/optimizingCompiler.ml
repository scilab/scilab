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

module type CODEGENERATOR =
  sig
    val generate_code: string -> string -> string -> Optimization.model -> bool
      -> bool -> string option * string option -> unit
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
      "@(#)Modelicac Copyright (C)\n\
        2003-2004 TNI-Valiosys,\n\
        2005-2007 Imagine,\n\
        2007-2009 LMS-Imagine"

    let version = "1.12.1"

    let path = ref ""

    let no_parameter_removal = ref false

    let keep_variables = ref false

    let compile_only = ref false

    let with_jac = ref false

    let only_outputs = ref false

    let init_in = ref None

    let init_out = ref None

    let trace = ref None

    let gen_xml = ref false

    let directories = ref [""]

    let output = ref ""

    let inputs = ref []

    let max_simplifs = ref max_int

    let add_lib_path s =
      directories := !directories @ [s]

    let set_init_in s =
      if !init_in <> None then
        failwith "set_init_in: More than one init_in specified";
      init_in := Some s

    let set_init_out s =
      if !init_out <> None then
        failwith "set_init_out: More than one init_out specified";
      init_out := Some s

(*
    let trace_filename s =
      if !trace <> None then
        failwith "trace_filename: More than one trace file specified";
      trace := Some s
*)

    let check_filename filename =
      if Filename.check_suffix filename "mo" then ()
      else failwith "check_filename: Filename suffix must be 'mo'"

    let set_path s =
      if !path <> "" then failwith "set_path: More than one path specified";
      path := Filename.dirname s

    let set_output s =
      if !output <> "" then failwith "set_output: More than one output specified";
      output := s

    let set_input s = inputs := s :: !inputs

    let set_max_simplifs i =
      max_simplifs := i

    let set_no_simplifs () =
      no_parameter_removal := true;
      keep_variables := true

    let construct_output_filename id =
      if !output = "" then begin
        if !gen_xml then
          output := Printf.sprintf "%s.xml" id
        else
          output := Printf.sprintf "%s.c" id;
        (*output := Filename.chop_suffix !input "mo";*)
        if !compile_only then
          output := Printf.sprintf "%s.moc" id
      end;
      !output

    let parse_args () =
      Arg.parse
        [("-c", Arg.Set compile_only, "compile only (do not instantiate)");
        ("-o", Arg.String set_output,
          "<outputfile>  Set output file name to <outputfile>");
        ("-hpath", Arg.String set_path,
          "<path>  Specify a path to be added to #include directives");
        ("-keep-all-variables", Arg.Set keep_variables,
          "Don\'t remove alias variables");
        ("-no-parameter-removal", Arg.Set no_parameter_removal,
          "Do not remove any parameter");
        ("-no-simplifs", Arg.Unit set_no_simplifs,
          "Same as -keep-all-variables -no-parameter-removal");
        ("-max-simplifs", Arg.Int set_max_simplifs,
          "<passes> Max number of simplifications");
        ("-jac", Arg.Set with_jac,
          "Generate symbolic jacobian matrix
          (may have no effect depending on target)");
        ("-only-outputs", Arg.Set only_outputs,
          "Generate code only for declared outputs
          (may have no effect depending on target)");
        ("-with-init-in", Arg.String set_init_in,
          "<init_input_file>
          Generate initialization code and use
          <init_input_file> to get input data
          (may have no effect depending on target)");
        ("-with-init-out", Arg.String set_init_out,
          "<init_output_file>
          Generate initialization code and use
          <init_output_file> to generate output data
          (may have no effect depending on target)");
(*
        ("-trace", Arg.String trace_filename,
          "<filename>
          Generate tracing information for
          external function calls into <filename>");
*)
        ("-xml", Arg.Set gen_xml,
          "Generate an XML version of the model
          instead of target code");
        ("-L", Arg.String add_lib_path,
          "<directory>
          Add <directory> to the list of directories to be
          searched in while linking")]
        set_input
        ("usage: modelicac [-c] [-o <outputfile>] <inputfile> [other options]")

    let compile filename =
      check_filename filename;
      let ic = open_in filename in
      Printf.printf "Input file name = %s\n" filename; flush stdout;
      let lexbuf = Lexing.from_channel ic in
      Printf.printf "Parsing..."; flush stdout;
      let tree = Parser.parse filename Lexer.token lexbuf in
      Printf.printf " OK\nPrecompiling..."; flush stdout;
      Precompilation.precompile tree

    let run () =
      parse_args ();
      let roots = match !inputs with
        | [] -> failwith "No input file name specified"
        | _ -> List.map compile !inputs in
      Printf.printf " OK\nCompiling..."; flush stdout;
      Compilation.paths := !directories;
      let id, cu = Compilation.compile_main_class roots in
      let filename = construct_output_filename id in
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
            let model =
              if !no_parameter_removal then
                Optimization.create_model_with_parameters !trace iexpr
              else Optimization.create_model !trace iexpr in
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
                "\n%d variable(s) remaining."
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
            if !gen_xml then XMLCodeGeneration.generate_XML filename fun_name model
            else
              G.generate_code
                !path
                filename
                fun_name model
                !with_jac
                !only_outputs
                (!init_in, !init_out)
        | Compilation.CompiledFunction _ ->
            failwith "Attempt to generate code for a function"
      end;
      Printf.printf " OK\nOutput file name = %s\n" !output; flush stdout;
      Printf.printf "Terminated.\n"; flush stdout

  end
