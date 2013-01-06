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
exception ReturnExn of (ScilabContext.symbol * Sci.t) list

let nargin_sy = ScilabContext.new_symbol "nargin"
let nargout_sy = ScilabContext.new_symbol "nargout"
let varargin_sy = ScilabContext.new_symbol "varargin"
let varargout_sy = ScilabContext.new_symbol "varargout"

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
  | OpExp_ldivide -> Sci.Ldivide
  | OpExp_power -> Sci.Power
  | OpExp_unaryMinus -> Sci.UnaryMinus
  | OpExp_dottimes -> Sci.Dottimes
  | OpExp_dotrdivide -> Sci.Dotrdivide
  | OpExp_dotldivide -> Sci.Dotldivide
  | OpExp_dotpower -> Sci.Dotpower
  | OpExp_krontimes -> Sci.Krontimes
  | OpExp_kronrdivide -> Sci.Kronrdivide
  | OpExp_kronldivide -> Sci.Kronldivide
  | OpExp_controltimes -> Sci.Controltimes
  | OpExp_controlrdivide -> Sci.Controlrdivide
  | OpExp_controlldivide -> Sci.Controlldivide
  | OpExp_eq -> Sci.Eq
  | OpExp_ne -> Sci.Ne
  | OpExp_lt -> Sci.Lt
  | OpExp_le -> Sci.Le
  | OpExp_gt -> Sci.Gt
  | OpExp_ge -> Sci.Ge

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

(* Initialize context methods, to create empty values, and copy on
   share values, based on refcounts *)
let _ =
  let ctx = ScilabContext.getInstance () in
  ScilabContext.set_context_create_empty_value ctx Sci.empty_double;
  (*  Sci.set_context_string_of_value ctx f; *)
  ScilabContext.set_context_refcount_setters ctx
    increase_refcount decrease_refcount;
  ()

(* Initialize the context with gatways loaded from Scilab context *)
let _ =
  let ctx = ScilabContext.getInstance () in
  let fun_names = Sci.get_funlist () in
  Array.iter (fun name ->
    let v = Sci.context_get name in
    ScilabContext.put ctx (ScilabContext.new_symbol name) v
  ) fun_names

let rec interp env exp =
  match exp.exp_desc with

(************************************************************ TOTAL   *)

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

  | ControlExp ( WhileExp  whileExp ) ->
    let env = { env with can_break = true; can_continue = true } in
    begin try
            while Sci.is_true (interp_one env whileExp.whileExp_test) do
              try
                ignore_interp env whileExp.whileExp_body
              with ContinueExn -> ()
            done;
            None
      with BreakExn -> None
    end


  | ControlExp ( ReturnExp { returnExp_exp = None } ) ->
    if env.can_return then raise (ReturnExn[]);
    None

  | ControlExp ( ReturnExp { returnExp_exp = Some exp } ) ->
    ignore_interp env exp;
    if env.can_return then raise (ReturnExn[]);
    None


  | Var  { var_desc =  ColonVar } ->
  (* TODO: we should write that BUT isColon() is sometimes optimized
     in the runtime, instead of optimizing 1:1:$, that is supposed to
     be equivalent. Scilint should print a warning for 1:1:$ to use
     (:) instead.

     let start = Sci.double 1. in
     let step = Sci.double 1. in
     let stop = Sci.dollar () in
     Some (Sci.implicitlist start step stop)
  *)
    Some (Sci.colon ())

  | Var  { var_desc =  DollarVar } ->
    (* TODO: why not set a context variable '$' everytime we do an
       extraction from a matrix, and completely remove the need for
       this. Is-it really optimized ? *)
    Some (Sci.dollar ())

  | Var { var_desc = SimpleVar sy } ->
    let ctx = ScilabContext.getInstance () in
    Some (ScilabContext.get ctx sy)


  | ControlExp (ForExp forExp) ->
    let varDec = forExp.forExp_vardec in (* name, init, kind *)
    let body = forExp.forExp_body in
    let name = varDec.varDec_name in
    let init = varDec.varDec_init in

    let init = interp env init in
    let ctx = ScilabContext.getInstance () in

    let loop iterator =
              try
                let env = { env with can_break = true; can_continue = true } in
                let rec iter iterator =
                  match iterator () with
                    None -> None
                  | Some v ->
                    ScilabContext.put ctx name v;
                    begin try
                            ignore_interp env body
                      with ContinueExn -> ()
                    end;
                    iter iterator
                in
                iter iterator
              with
              | ContinueExn -> None
    in

    begin
      match init with
        None -> None
      | Some looper ->
        let iterator =
          match Sci.get_type looper with
          | Sci.RealImplicitList ->
            Sci.iterator_of_implicitlist looper

          | Sci.RealList
          | Sci.RealTList
          | Sci.RealMList ->
            Sci.iterator_of_list looper

          | typ ->
            if Sci.isGeneric typ then
              Sci.iterator_of_generic looper
            else begin
              Printf.fprintf stderr "ScilabInterp: Don't know how to for:\n%s" (Sci.to_string looper);
              raise InterpFailed
            end
        in
        match iterator with
          None -> None
        | Some iterator -> loop iterator
    end

  | MathExp (OpExp (oper, args)) ->
    let left = interp_one env args.opExp_left in
    let right = interp_one env args.opExp_right in

    let left = Sci.extractFullMatrix left in
    let right = Sci.extractFullMatrix right in

    Some (Sci.operation (binop_of_OpExp_Oper oper) left right)

  | MathExp ( NotExp  { notExp_exp = exp } ) ->
    Some (Sci.not_exp (interp_one env exp))


(* These ones can only happen within other constructs, in specific positions.
   Do the pattern-matching directly there for them ! *)
  | ArrayListExp  _ -> assert false
  | AssignListExp  _ -> assert false
  | Var  { var_desc =  ArrayListVar _ } -> assert false

(************************************************************ PARTIAL *)

(* OCAML TODO: it would be nice to have "binding code" in pattern-matching, i.e.
 the ability to bind values not only inside the pattern, but also complementary to
 the pattern:

   AssignListExp vars
|  _exp /{ _exp -> vars = [| _exp |] } ->
*)


  | AssignExp { assignExp_left_exp = left;
                assignExp_right_exp = {
                  exp_desc = ControlExp (ReturnExp return) }
              }
      when env.can_return ->
    let vars =
      match left.exp_desc with
      | AssignListExp vars -> vars
      | _ -> [| left |]
    in
    let exps =
      match return.returnExp_exp with
        None ->
          (* With Scilab 5, it works if only one variable is assigned, the
             variable is just left undefined, no ? *)
          assert false
      | Some { exp_desc = ArrayListExp exps } -> exps
      | Some exp -> [| exp |]
    in
    let nvars = Array.length vars in
    let nexps = Array.length exps in
    if nvars <> nexps then
      failwith "ScilabInterp: incompatible number of returned arguments";

(* TODO: incorrect, arguments should be evaluated in the opposite order *)
    let exps = Array.map (interp_one env) exps in
    let bindings = ref [] in
    for iArg = 0 to nvars-1 do
      match vars.(iArg) with
      | { exp_desc = Var { var_desc = SimpleVar sy } } ->
        let exp = Sci.extractFullMatrix exps.(iArg) in
        bindings := (sy, exp) :: !bindings
      | _ -> assert false
    done;
    raise (ReturnExn (List.rev !bindings));

(* If this expression cannot return, the bindings are directly
   inserted in the current environment (i.e. toplevel only). *)
  | AssignExp { assignExp_left_exp = left;
                assignExp_right_exp = {
                  exp_desc = ControlExp (ReturnExp
                                           { returnExp_exp = Some right }) }
              }
  | AssignExp { assignExp_left_exp = left; assignExp_right_exp = right } ->

    let vars =
      match left.exp_desc with
      | AssignListExp vars -> vars
      | _ -> [| left |]
    in
    let nvars = Array.length vars in
    let rhs = match right.exp_desc with
      | ArrayListExp exps ->
(* TODO: incorrect, arguments should be evaluated in the opposite order *)
        Array.map (interp_one env) exps
      | _ -> interp_rhs nvars env right in
    let nrhs = Array.length rhs in
    if nrhs < nvars &&
      nvars > 1 (* In scilab 5, it is OK to have nrhs=0, nvars=1, the variable remains undefined *)
    then
      failwith (Printf.sprintf "ScilabInterp: trying to assign %d values to %d variables" nrhs nvars);

    for iArg = 0 to (min nvars nrhs)-1 do
      match vars.(iArg) with
      | { exp_desc = Var { var_desc = SimpleVar sy } } ->
        let ctx = ScilabContext.getInstance () in
        let exp = rhs.(iArg) in
        let exp = Sci.extractFullMatrix exp in
        ScilabContext.put ctx sy exp


(*
    | CellCallExp _ (* TODO *)
    (* we have to compute [c] to know what has to be modified, e.g. a.b(1) = x,
       in particular extraction in a cell field. It is either a variable,
       or a CallExp. *)

    | CallExp _ (* TODO *)
    (* Field assignment avec extraction *)

    | FieldExp _ (* TODO *)

    | _ ->
      Printf.fprintf stderr "ScilabInterp: Don't know how to asssign to:\n%s" (ScilabAstPrinter.to_string left);
      raise InterpFailed

    end
*)


      | _ -> assert false
    done;
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

      (* define "nargin" and "nargout" so that primitive "argn"
         keeps working. *)
      ScilabContext.put ctx nargin_sy (Sci.double (float_of_int nvalues));
      ScilabContext.put ctx nargout_sy (Sci.double (float_of_int iRetCount));
      (* TODO: if the return argument is "varargout", we need to
         create it as an empty list so that assignments like varargout(1) = 1
         will not create it as a vector. *)
      let bindings =
        try
          ignore_interp macro_env f.functionDec_body;
          []
        with ReturnExn bindings -> bindings
      in

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

      List.iter (fun (name, v) ->
        ScilabContext.put ctx name v
      ) (* bindings are assigned from right to left *)
        (List.rev bindings);

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
  | ControlExp ( SelectExp  _ )
  | ControlExp ( TryCatchExp  _ )
  | MathExp ( MatrixExp  _ )
  | MathExp ( CellExp  _ )
  | MathExp ( LogicalOpExp ( _ ,  _ ))
  | MathExp ( TransposeExp  _ )

  | Dec ( VarDec  _ )
    ->
    Printf.fprintf stderr "ScilabInterp: Don't know how to exec:\n%s" (ScilabAstPrinter.to_string exp);
      raise InterpFailed

(* When calling a function, we need to provide the number of results
   that are expected in return. Since this only appears in a few places
   (assigns and returns) and is not propagated through other
   expressions we have a specific function to execute it. *)
and interp_rhs expected_size env exp =
  match exp.exp_desc with

  (* This is supposed to be the only use case of ArrayListExp *)
  | CallExp {
    callExp_name = name;
    callExp_args = [| { exp_desc = ArrayListExp args } |] }

  | CallExp {
    callExp_name = name;
    callExp_args = args; } ->
    (* TODO: We need to know how many return values arg expected by the
       calling expression. *)

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

exception FromToplevel

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

  declare_macro "argn"
    (fun args opt_args iRetCount->
      try
        assert (opt_args = [||]);
        let ctx = ScilabContext.getInstance () in
        let args = match args with
            [| |] -> None
          | [| t |] ->
            if Sci.get_type t = Sci.RealDouble then
              Some (Sci.get_double t 0)
            else
              failwith "ScilabInterp.argn: expect one double"
          | _ ->
            failwith "ScilabInterp.argn: expect at most one argument"
        in
        let nargin =
          try
            ScilabContext.get ctx nargin_sy
          with _ -> raise FromToplevel
        in
        let nargout =
          try ScilabContext.get ctx nargout_sy
          with _ ->
            (* Someone is playing with "nargin"... *)
            assert false
        in

        match args, iRetCount with
          ((None | Some 0.), 1) (* [lhs]= argn() or [lhs]=argn(0) *)
        | (Some 1. , _)         (* [lhs]= argn(1) *)
          -> Some [| nargin |]
        | (None | Some 0.), 2  (* [lhs,rhs]=argn() or [lhs,rhs]=argn(0) *)
          -> Some [| nargin; nargout |]
        | (Some 2. , _)          (* [rhs]= argn(2) *)
        | _ ->
          failwith "ScilabInterp.argn: wrong value of argument"
      with FromToplevel ->
        Some [| Sci.double 0. |]
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

