(*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012-2013 - OCAMLPRO INRIA - Fabrice LE FESSANT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 *)

open ScilabAst

(* Problems:
  - an identifier of a function in some position is evaluated as a call to the
    function (statements)

   TODO: Try to avoid copy operations, especially in loops. When is a
   copy operation really needed in Scilab ?
*)

module Sci = ScilabInternalType

type interp_result = Sci.t array

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

let binop_of_OpExp_Oper oper =
  match oper with
  | OpExp_plus -> Sci.Plus
  | OpExp_minus -> Sci.Minus
  | OpExp_times -> Sci.Times
  | OpExp_rdivide -> Sci.Rdivide
  | OpExp_ldivide
  | OpExp_power
  | OpExp_unaryMinus
  | OpExp_dottimes
  | OpExp_dotrdivide
  | OpExp_dotldivide
  | OpExp_dotpower
  | OpExp_krontimes
  | OpExp_kronrdivide
  | OpExp_kronldivide
  | OpExp_controltimes
  | OpExp_controlrdivide
  | OpExp_controlldivide
  | OpExp_eq
  | OpExp_ne
  | OpExp_lt
  | OpExp_le
  | OpExp_gt
  | OpExp_ge
    -> assert false

(* We use this to clone a value only if it is shared by something else. *)
let increase_refcount v =
  let r = Sci.refcount v in
  let v =
    if r > 1 then
      Sci.clone v
    else
      if r < 1 then assert false
      else v
  in
  Sci.incr_refcount v;
  v
let decrease_refcount v =
  Sci.decr_refcount v

let _ =
  let ctx = ScilabContext.getInstance () in
  ScilabContext.set_context_create_empty_value ctx Sci.empty_double;
  (*  Sci.set_context_string_of_value ctx f; *)
  ScilabContext.set_context_refcount_setters ctx
    increase_refcount decrease_refcount;
  ()

let rec interp env exp =
  match exp.exp_desc with

(************************************************************ TOTAL   *)

(************************************************************ PARTIAL *)

  | AssignExp a ->
    let left =  a.assignExp_left_exp in
    let right = a.assignExp_right_exp in
    begin match left.exp_desc with
    | Var { var_desc = SimpleVar symbol } ->

      begin

        match interp env right with
        | Some v ->
          let ctx = ScilabContext.getInstance () in

          (* TODO:     v = Matrix => do a copy
             in run_AssignExp, there are special cases for:
             v = IsImplicitList => extract a real matrix
             right = ReturnExp _ => modify values in the previous scope
             | ControlExp ( ReturnExp  _ )
          *)
          ScilabContext.put ctx symbol v;
          None

        (*
          In Scilab 6, this is an error.
          In Scilab 5, no error
        *)

        | _ -> None
      end

    | CellCallExp _ (* TODO *)
    (* we have to compute [c] to know what has to be modified, e.g. a.b(1) = x,
       in particular extraction in a cell field. It is either a variable,
       or a CallExp. *)

    | CallExp _ (* TODO *)
    (* Field assignment avec extraction *)

    | AssignListExp _ (* TODO *)

    | FieldExp _ (* TODO *)

    | _ ->
      Printf.fprintf stderr "ScilabInterp: Don't know how to asssign to:\n%s" (ScilabAstPrinter.to_string left);
      raise InterpFailed

    end

  | ConstExp (CommentExp _) -> None

  | ConstExp (BoolExp { boolExp_value = b }) ->
    Some (Sci.bool b)

  | ConstExp (StringExp { stringExp_value = b }) ->
    Some (Sci.string b)

  | ConstExp (IntExp { intExp_value = n; intExp_prec = IntExp_8 }) ->
    Some (Sci.int8 n)

  | ConstExp (IntExp { intExp_value = n; intExp_prec = IntExp_16 }) ->
    Some (Sci.int16 n)

  | ConstExp (IntExp { intExp_value = n; intExp_prec = IntExp_32 }) ->
    Some (Sci.int32 n)

  | ConstExp (IntExp { intExp_value = n; intExp_prec = IntExp_64 }) ->
    assert false

  | ConstExp (DoubleExp { doubleExp_value = n }) ->
    Some (Sci.double n)

  | ConstExp (FloatExp { floatExp_value = n }) ->
    Some (Sci.float n)

  | ConstExp NilExp -> Some (Sci.empty_double ())

  | SeqExp list ->
    interp_sexp env list

  (* In Scilab 5, a break at toplevel will cause an error, not in Scilab
     6 (but in Scilab 5, a break in a function won't cause an error).
  *)
  | ControlExp BreakExp ->
    if env.can_break then raise BreakExn else None

  | ControlExp ContinueExp ->
    if env.can_continue then raise ContinueExn else None

  | ControlExp (ForExp forExp) ->
    let varDec = forExp.forExp_vardec in (* name, init, kind *)
    let body = forExp.forExp_body in
    let name = varDec.varDec_name in
    let init = varDec.varDec_init in

    let init = interp env init in
    begin
      match init with
        None -> None
      | Some looper ->
        match Sci.get_type looper with
        | Sci.RealImplicitList ->
          let (start, step, stop) = Sci.get_implicitlist looper in
          let ctx = ScilabContext.getInstance () in
          let for_loop_double () =
            let start = Sci.get_double start 0 in
            let step = Sci.get_double step 0 in
            let stop = Sci.get_double stop 0 in
            let r = ref start in
            let v = Sci.double start in
            let cond = ref (!r <= stop) in
            let env = { env with can_break = true; can_return = true } in
            while !cond  do
              try
                ScilabContext.put ctx name v;
                ignore_interp env body;
                r := !r +. step;
                Sci.set_double v 0 !r;
                cond := (!r <= stop)
              with
              | BreakExn -> cond := false
              | ContinueExn -> cond := true
            done;
            None

          in
          let for_loop get plus create set =
            let start = get start 0 in
            let step = get step 0 in
            let stop = get stop 0 in
            let r = ref start in
            let v = create start in
            let cond = ref (!r <= stop) in
            let env = { env with can_break = true; can_return = true } in
            while !cond  do
              try
                ScilabContext.put ctx name v;
                ignore_interp env body;
                r := plus !r step;
                set v 0 !r;
                cond := (!r <= stop)
              with
              | BreakExn -> cond := false
              | ContinueExn -> cond := true
            done;
            None

          in
          begin
            match Sci.get_type start,
              Sci.get_type step,
              Sci.get_type stop with
              | Sci.RealDouble, Sci.RealDouble, Sci.RealDouble ->
                for_loop_double ()
              (* for_loop Sci.get_double (+.) Sci.double Sci.unsafe_set_double *)
              | Sci.RealInt8, _, _
              | Sci.RealDouble, Sci.RealInt8, _
              | Sci.RealDouble, Sci.RealDouble, Sci.RealInt8 ->
                for_loop Sci.get_int8 Sci.add_int8 Sci.int8 Sci.unsafe_set_int8
              | Sci.RealInt16, _, _
              | Sci.RealDouble, Sci.RealInt16, _
              | Sci.RealDouble, Sci.RealDouble, Sci.RealInt16 ->
                for_loop Sci.get_int16 Sci.add_int16 Sci.int8 Sci.unsafe_set_int16
              | Sci.RealInt32, _, _
              | Sci.RealDouble, Sci.RealInt32, _
              | Sci.RealDouble, Sci.RealDouble, Sci.RealInt32 ->
                for_loop Sci.get_int32 Sci.add_int32 Sci.int32 Sci.unsafe_set_int32
              | _ -> assert false
          end

        (* TODO : what else can be used to iterate on it ? *)
        | x ->
          Printf.fprintf stderr "ScilabInterp: Don't know how to for:\n%s" (Sci.to_string looper);
          raise InterpFailed

    end

  | ControlExp (IfExp ifexp) ->
    let test = interp_one env ifexp.ifExp_test in
    if Sci.is_true test then
      ignore_interp env ifexp.ifExp_then
    else begin
      match ifexp.ifExp_else with
        None -> ()
      | Some ifexp_else ->
        ignore_interp env ifexp_else
    end;
    None


  | ListExp listExp ->
    let start = interp_one env listExp.listExp_start in
    let step = interp_one env listExp.listExp_step in
    let stop = interp_one env listExp.listExp_end in
    (* TODO: set all values to the same scalar type *)
    Some (Sci.implicitlist (Sci.clone start) (Sci.clone step) (Sci.clone stop))

  (*
  (* TODO: check fixError problem on this case ! *)
    | MathExp ( OpExp  _ )
  *)

  | MathExp (OpExp (oper, args)) ->
    let left = interp_one env args.opExp_left in
    let right = interp_one env args.opExp_right in
    (* TODO: if implicitlists, generate full matrices *)
    Some (Sci.operation (binop_of_OpExp_Oper oper) left right)

  | Var { var_desc = SimpleVar sy } ->
    let ctx = ScilabContext.getInstance () in
    Some (ScilabContext.get ctx sy)

  | Dec (FunctionDec f) ->
    let ctx = ScilabContext.getInstance () in
    let sy = f.functionDec_symbol in
    let name = ScilabContext.symbol_name sy in
    let f args opt_args iRetCount =
      ScilabContext.begin_scope ctx;
      (* TODO: we can catch exceptions and recover the current scope,
         i.e. remove all scopes above this one. *)
      let nparameters = Array.length f.functionDec_args.arrayListVar_vars in
      let nvalues = Array.length args in


      (* TODO: if the last argument is called "varargin", we can accept many
         more arguments, and copy all of them in this last argument. *)
      if nvalues > nparameters then
        raise (TooManyArguments (name, nparameters, nvalues));
      for i = 0 to nvalues-1 do
        let var = f.functionDec_args.arrayListVar_vars.(i) in
        match var.var_desc with
          SimpleVar sy ->
            (* TODO: copy values here *)
            ScilabContext.put ctx sy args.(i)
        | _ -> assert false
      done;

      (* TODO: define "nargin" and "nargout" so that primitive "argn"
         keeps working. *)

      begin
        try
          ignore_interp macro_env f.functionDec_body
        with ReturnExn -> ()
      end;

      (* TODO: if the return argument is "varargout", we have to
         destructure its content to recreate the list of returned
         values.      *)
      let returns = Array.map (fun var ->
        match var.var_desc with
          SimpleVar sy ->
            ScilabContext.get ctx sy
        | _ -> assert false
      ) f.functionDec_returns.arrayListVar_vars in
      ScilabContext.end_scope ctx;
      Some returns
    in

    ScilabContext.put ctx sy (Sci.ocamlfunction name f);
    None

  (* from here, we probably only need the first argument, so set
     expected_size to 1 in interp_rhs that will interp CallExp itself. *)
  | CallExp _
  | CellCallExp _
    ->
    begin match interp_rhs 1 env exp with
      [||] -> None
    | array -> Some array.(0)
    end

(************************************************************ TODO    *)

  | FieldExp  _
  | ArrayListExp  _
  | AssignListExp  _
  | ControlExp ( ReturnExp  _ )
  | ControlExp ( SelectExp  _ )
  | ControlExp ( TryCatchExp  _ )
  | ControlExp ( WhileExp  _ )
  | MathExp ( MatrixExp  _ )
  | MathExp ( CellExp  _ )
  | MathExp ( NotExp  _ )
  | MathExp ( LogicalOpExp ( _ ,  _ ))
  | MathExp ( TransposeExp  _ )


  | Var  { var_desc =  ColonVar }
  | Var  { var_desc =  DollarVar }
  | Var  { var_desc =  ArrayListVar  _ }
  | Dec ( VarDec  _ )
    ->
    Printf.fprintf stderr "ScilabInterp: Don't know how to exec:\n%s" (ScilabAstPrinter.to_string exp);
      raise InterpFailed

and interp_rhs expected_size env exp =
  match exp.exp_desc with

  | CallExp c ->
    (* TODO: We need to know how many return values arg expected by the
       calling expression. *)

    let name = c.callExp_name in
    let args = c.callExp_args in
    let name = interp_one env name in

    (* TODO:

       do better, especially for optional function arguments.
       If an argument is a AssignExp node, with an SimpleVar on the
       left, the association should be put as an optional argument of
       the function. Note that 'named' arguments can only be specified
       if other arguments are expected. However, it is possible to
       override the value of ANY external variable (including global)
       using such arguments.

       Interestingly, Scilab 5 complains about these arguments if
       there are too many of them, while Scilab 6 accepts as many
       named arguments as possible.  *)
    let args = Array.map (interp_one env) args in
    begin
      match Sci.get_type name with
      | Sci.RealFunction ->
        (*        Printf.eprintf "calling...\n%!"; *)
        begin match Sci.call name args [||] expected_size with
          None ->
            (*            Printf.eprintf "Call -> error\n%!"; *)
            assert false (* An error occurred, what should we do ? *)
        | Some returns ->
          (*          Printf.eprintf "Call -> return\n%!"; *)
          returns
        end
      | _ -> assert false (* TODO: extraction *)
    end

  | CellCallExp  _ (* a{x,y,z) *)
  (* TODO: extract the values of the fields of the cell, and copy them (depending on the
     expected size *)

  | _ ->
    match interp env exp with
      None -> [||]
    | Some v -> [| v |]


and interp_sexp env list =
  match list with
    [] -> None
  | [ exp ] -> interp env exp
  | exp :: tail ->
    (* TODO: if the returned value is a function, we should execute it,
       to handle the case of function calls without (). *)
    ignore_interp env exp;
    (* TODO: set "ans" if there is a return value, AND it is not
       assigned, or just a variable. *)
    interp_sexp env tail

and ignore_interp env exp =
  let (_ : Sci.t option) = interp env exp in
  ()

and interp_one env exp =
  match interp env exp with
    None -> Sci.empty_double ()
  | Some t -> t

let interp exp =
  ScilabContext.clear (ScilabContext.getInstance ());
  let results = interp toplevel_env exp in
  match results with
  | None -> Sci.empty_double ()
  | Some v -> v

let unicode_of_ascii s =
  let len = String.length s in
  let u = String.make (len*4) '\000' in
  for i = 0 to len-1 do
    let pos = i * 4 in
    u.[pos] <- s.[i];
  done;
  u

let declare_macro macro_name f =
  let ctx = ScilabContext.getInstance () in
  let macro_name = unicode_of_ascii macro_name in
  ScilabContext.put ctx (ScilabContext.new_symbol macro_name)
    (Sci.ocamlfunction macro_name f)

let _ =
  declare_macro "global"
    (fun args opt_args iRetCount->
      assert (opt_args = [||]);
      let ctx = ScilabContext.getInstance () in
      Array.iter (fun v ->
        match Sci.get_type v with
        | Sci.RealString ->
          ScilabContext.global ctx
            (ScilabContext.new_symbol (Sci.get_string v 0))
        | _ -> failwith "global with Wrong type of argument"
      ) args;
      Some [||]
    );

(* TODO: other functions that we need to define here, if we don't use
   the scopes of Scilab.

   "clear", "clearfun"

Different behaviors between Scilab 5 and Scilab 6:
- "clear()" removes all variables in Scilab 5, but removes no variables in
    Scilab 6.
- "who" is implemented, but with a different meaning ?

Not implemented in Scilab 6:
- "predef", "clearfun"
- "comp"
- "delbpt",
- "deff"
- "errcatch", "errclear", "iserror"
- "fort"
- "funptr", "funcprot", "getf", "newfun"
- "what", "where", "whereami", "whos"
*)

