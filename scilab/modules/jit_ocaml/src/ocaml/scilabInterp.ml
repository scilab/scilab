open ScilabAst
open ScilabContext
open ScilabValue.TYPES

type interp_result = ScilabValue.t array

exception InterpFailed

exception TooManyArguments of string * int * int

exception BreakExn
exception ContinueExn
exception ReturnExn


type env = {
  can_break : bool;
  can_continue : bool;
  can_return : bool;
}

let toplevel_env = {
  can_break = false;
  can_return = false;
  can_continue = false;
}

let macro_env = {
  can_break = false;
  can_return = true;
  can_continue = false;
}

let rec interp env exp =
  match exp.exp_desc with

  | AssignExp a ->
    let left =  a.assignExp_left_exp in
    let right = a.assignExp_right_exp in
    begin match left.exp_desc with
    | Var { var_desc = SimpleVar symbol } ->

      begin
        match interp env right with
          [| v |] ->
            let ctx = getInstance () in
            put ctx symbol v;
            [||]

          (*
            In Scilab 6, this is an error.
            In Scilab 5, no error
          *)
        | _ -> [||]
      end
    | _ ->
      Printf.fprintf stderr "ScilabInterp: Don't know how to asssign to:\n%s" (ScilabAstPrinter.to_string left);
      raise InterpFailed

    end

  | ConstExp (CommentExp _) -> [||]

  | ConstExp (BoolExp { boolExp_value = b }) ->
    [| ScilabValue.bool b |]

  | ConstExp (StringExp { stringExp_value = b }) ->
    [| ScilabValue.string b |]

  | ConstExp (IntExp { intExp_value = n }) ->
    [| ScilabValue.int n |]

  | ConstExp (DoubleExp { doubleExp_value = n }) ->
    [| ScilabValue.double n |]

  | SeqExp list ->
    interp_sexp env list

  | ControlExp BreakExp ->
    if env.can_break then raise BreakExn else [||]

  | ControlExp ContinueExp ->
    if env.can_continue then raise ContinueExn else [||]

  | ControlExp (ForExp forExp) ->
    let varDec = forExp.forExp_vardec in (* name, init, kind *)
    let body = forExp.forExp_body in
    let name = varDec.varDec_name in
    let init = varDec.varDec_init in

    let init = interp env init in
    begin
      match init with
        [||] -> [||]
      | _ ->
        match init.(0) with
          List (start, step, stop) ->
            let ctx = getInstance () in

            let v = ref start in
            let cond = ref (ScilabValue.lt !v stop) in

            let env = { env with can_break = true; can_return = true } in

            while !cond  do
              try
                put ctx name !v;

                ignore_interp env body;

                v :=  ScilabValue.add !v step;
                cond := (ScilabValue.lt !v stop)
              with
              | BreakExn -> cond := false
              | ContinueExn -> cond := true
            done;

            [||]

          (* TODO : what else can be used to iterate on it ? *)
        | x ->
          Printf.fprintf stderr "ScilabInterp: Don't know how to for:\n%s" (ScilabValue.to_string x);
          raise InterpFailed

    end

  | ListExp listExp ->
    let start = interp_one env listExp.listExp_start in
    let step = interp_one env listExp.listExp_step in
    let stop = interp_one env listExp.listExp_end in
      (* TODO: set all values to the same scalar type *)
    [| List (start, step, stop) |]

  | MathExp (OpExp (OpExp_plus, args)) ->
    let left = interp_one env args.opExp_left in
    let right = interp_one env args.opExp_right in
    [| ScilabValue.add left right |]

  | Var { var_desc = SimpleVar sy } ->
    let ctx = getInstance () in
    [| get ctx sy |]

  | Dec (FunctionDec f) ->
    let ctx = getInstance () in
    let sy = f.functionDec_symbol in
    let name = symbol_name sy in
    let f args =
      begin_scope ();
      let nparameters = Array.length f.functionDec_args.arrayListVar_vars in
      let nvalues = Array.length args in
      if nvalues > nparameters then
        raise (TooManyArguments (name, nparameters, nvalues));
      for i = 0 to nvalues-1 do
        let var = f.functionDec_args.arrayListVar_vars.(i) in
        match var.var_desc with
          SimpleVar sy ->
            put ctx sy args.(i)
        | _ -> assert false
      done;

      begin
        try
          ignore_interp macro_env f.functionDec_body
        with ReturnExn -> ()
      end;
      let returns = Array.map (fun var ->
        match var.var_desc with
          SimpleVar sy ->
            get ctx sy
        | _ -> assert false
      ) f.functionDec_returns.arrayListVar_vars in
      end_scope ();
      returns
    in
    put ctx sy (Macro (name, f));
    [||]

  | CallExp c ->
    let name = c.callExp_name in
    let args = c.callExp_args in
    let name = interp_one env name in
    let args = Array.map (interp_one env) args in
    begin
      match name with
      | Macro (function_name, f) -> f args
      | _ -> assert false (* TODO: extraction *)
    end

  | _ ->
    Printf.fprintf stderr "ScilabInterp: Don't know how to exec:\n%s" (ScilabAstPrinter.to_string exp);
    raise InterpFailed

and interp_sexp env list =
  match list with
    [] -> [||]
  | [ exp ] -> interp env exp
  | exp :: tail ->
    ignore_interp env exp;
    interp_sexp env tail

and ignore_interp env exp =
  let (_ : ScilabValue.t array) = interp env exp in
  ()

and interp_one env exp =
  match interp env exp with
    [| |] -> ScilabValue.empty
  | t -> t.(0)

let interp exp =
  ScilabContext.clear ();
  let results = interp toplevel_env exp in
  let len = Array.length results in
  if len = 0 then ScilabValue.empty
  else results.(0)


let unicode_of_ascii s =
  let len = String.length s in
  let u = String.make (len*4) '\000' in
  for i = 0 to len-1 do
    let pos = i * 4 in
    u.[pos] <- s.[i];
  done;
  u

let declare_macro macro_name f =
  let ctx = getInstance () in
  let macro_name = unicode_of_ascii macro_name in
  put ctx (new_symbol macro_name)
    (Macro (macro_name, f))

let _ =
  declare_macro "global"
    (fun args ->
      let ctx = getInstance () in
      Array.iter (fun v ->
        match v with
        | String s ->
          ScilabContext.global ctx (new_symbol s)
        | _ -> failwith "global with Wrong type of argument"
      ) args;
      [||]
    )

