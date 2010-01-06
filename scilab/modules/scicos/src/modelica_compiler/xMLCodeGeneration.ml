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

type 'a tree = Leaf of (string * 'a) | Node of string * 'a tree list

(* function used to hide XML special characters *)
let hide_spc s =
  let encoded_s = ref "" in
  let hide_special_character c = match c with
    | '<' -> encoded_s := !encoded_s ^ "&lt;"
    | '>' -> encoded_s := !encoded_s ^ "&gt;"
    | '&' -> encoded_s := !encoded_s ^ "&amp;"
    | '\'' -> encoded_s := !encoded_s ^ "&apos;"
    | '\"' -> encoded_s := !encoded_s ^ "&quot;"
    | _ -> encoded_s := !encoded_s ^ (String.make 1 c) in
  String.iter hide_special_character s;
  !encoded_s

let rec insert path x ts =
  let rec insert' s path' = function
    | [] -> [Node (s, insert path' x [])]
    | Node (s', ts'') :: ts' when s = s' -> Node (s', insert path' x ts'') :: ts'
    | t' :: ts' -> t' :: insert' s path' ts'
  in match path with
    | [s] -> Leaf (s, x) :: ts
    | s :: path' -> insert' s path' ts
    | [] -> assert false

let cut_on_dot s =
  let rec cut_on_dot' i =
    if i = String.length s then s, None
      else if s.[i] = '.' then String.sub s 0 i, Some (String.sub s (i + 1) (String.length s - i - 1))
      else cut_on_dot' (i + 1)
  in cut_on_dot' 0

let rec split name =
  let s, name_opt = cut_on_dot name in
  match name_opt with
    | None -> [s]
    | Some name' -> s :: split name'

type element =
  {
    kind: element_kind;
    id: string;
    comment: string;
    initial_value: SymbolicExpression.t option;
    output: bool
  }

and element_kind =
  | Input
  | Parameter
  | Variable
  | DiscreteVariable

let build_tree model =
  let bool_of_option = function
    | None -> false
    | Some _ -> true
  in
  let (_, ts) =
    Array.fold_left
      (fun (i, ts) s ->
        i + 1,
        insert
          (split s) 
          {
            kind = Input;
            id = s;
            comment = "";
            initial_value = Some SymbolicExpression.zero;
            output = false
          }
          ts)
      (0, [])
      model.Optimization.inputs in
  let (_, ts) =
    Array.fold_left
      (fun (i, ts) par ->
        i + 1,
        insert
          (split par.Optimization.p_name)
          {
            kind = Parameter;
            id = par.Optimization.p_name;
            comment = par.Optimization.p_comment;
            initial_value = Some par.Optimization.value;
            output = false
          }
          ts)
      (0, ts)
      model.Optimization.parameters in
  let (_, ts) =
    Array.fold_left
      (fun (i, ts) var ->
        i + 1,
        insert
          (split var.Optimization.v_name)
          {
            kind = Variable;
            id = var.Optimization.v_name;
            comment = var.Optimization.v_comment;
            initial_value = var.Optimization.start_value;
            output = bool_of_option var.Optimization.output
          }
          ts)
      (0, ts)
      model.Optimization.variables in
  let (_, ts) =
    Array.fold_left
      (fun (i, ts) dvar ->
        i + 1,
        insert
          (split dvar.Optimization.d_v_name)
          {
            kind = DiscreteVariable;
            id = dvar.Optimization.d_v_name;
            comment = dvar.Optimization.d_v_comment;
            initial_value = dvar.Optimization.d_start_value;
            output = bool_of_option dvar.Optimization.d_output
          }
          ts)
      (0, ts)
      model.Optimization.discrete_variables in
  ts

let print_expression oc model expr =
  let add_parenthesis expr_option sub_expr =
    match expr_option with
    | None -> sub_expr
    | Some _ -> Printf.sprintf "(%s)" sub_expr in
  let rec string_of_expression expr_option sub_expr =
    let expr_option' = Some sub_expr in
    match SymbolicExpression.nature sub_expr with
    | SymbolicExpression.Addition [] -> "0"
    | SymbolicExpression.Addition exprs ->
        let exprs' = List.map (string_of_expression expr_option') exprs in
        add_parenthesis expr_option (String.concat " + " exprs')
    | SymbolicExpression.And [] -> "false"
    | SymbolicExpression.And (exprs) ->
        let s = List.map (string_of_expression expr_option') exprs in
        add_parenthesis expr_option (String.concat " and " s)
    | SymbolicExpression.ArcCosine expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "acos(%s)" s
    | SymbolicExpression.ArcHyperbolicCosine expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "acosh(%s)" s
    | SymbolicExpression.ArcHyperbolicSine expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "asinh(%s)" s
    | SymbolicExpression.ArcHyperbolicTangent expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "atanh(%s)" s
    | SymbolicExpression.ArcSine expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "asin(%s)" s
    | SymbolicExpression.ArcTangent expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "atan(%s)" s
    | SymbolicExpression.BlackBox (s, args) ->
        let args' = List.map (string_of_argument expr_option') args in
        let s' = String.concat ", " args' in
        Printf.sprintf "%s(%s)" s s'
    | SymbolicExpression.BooleanValue false -> Printf.sprintf "false"
    | SymbolicExpression.BooleanValue true -> Printf.sprintf "true"
    | SymbolicExpression.Constant s -> s
    | SymbolicExpression.Cosine expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "cos(%s)" s
    | SymbolicExpression.Derivative (expr, Num.Int 1) ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "der(%s)" s
    | SymbolicExpression.Derivative _ -> assert false
    | SymbolicExpression.DiscreteVariable i when i >= 0 ->
        Printf.sprintf "`%s`"
          model.Optimization.discrete_variables.(i).Optimization.d_v_name
    | SymbolicExpression.DiscreteVariable i ->
        Printf.sprintf "`%s`" model.Optimization.inputs.(-1 - i)
    | SymbolicExpression.Equality (expr, expr') ->
        let s =
          Printf.sprintf "%s == %s"
            (string_of_expression expr_option' expr)
            (string_of_expression expr_option' expr') in
        add_parenthesis expr_option s
    | SymbolicExpression.Exponential expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "exp(%s)" s
    | SymbolicExpression.Floor expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "floor(%s)" s
    | SymbolicExpression.Greater (expr, expr') ->
        let s =
          Printf.sprintf "%s > %s"
            (string_of_expression expr_option' expr)
            (string_of_expression expr_option' expr') in
        add_parenthesis expr_option s
    | SymbolicExpression.GreaterEqual (expr, expr') ->
        let s =
          Printf.sprintf "%s >= %s"
            (string_of_expression expr_option' expr)
            (string_of_expression expr_option' expr') in
        add_parenthesis expr_option s
    | SymbolicExpression.HyperbolicCosine expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "cosh(%s)" s
    | SymbolicExpression.HyperbolicSine expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "sinh(%s)" s
    | SymbolicExpression.HyperbolicTangent expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "tanh(%s)" s
    | SymbolicExpression.If (expr, expr', expr'') ->
        let s =
          Printf.sprintf "if %s then %s else %s"
            (string_of_expression expr_option' expr)
            (string_of_expression expr_option' expr')
            (string_of_expression expr_option' expr'') in
        add_parenthesis expr_option s
    | SymbolicExpression.Integer i ->
        let s = Printf.sprintf "%ld" i in
        add_parenthesis expr_option s
    | SymbolicExpression.Logarithm expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "log(%s)" s
    | SymbolicExpression.Multiplication [] -> "1"
    | SymbolicExpression.Multiplication exprs ->
        let exprs' = List.map (string_of_expression expr_option') exprs in
        let s = String.concat " * " exprs' in
        add_parenthesis expr_option (Printf.sprintf "%s" s)
    | SymbolicExpression.Not expr ->
        let s = string_of_expression expr_option' expr in
        add_parenthesis expr_option (Printf.sprintf "not %s" s)
    | SymbolicExpression.Number num ->
        let s = Printf.sprintf "%.16g" (Num.float_of_num num) in
        add_parenthesis expr_option s
    | SymbolicExpression.Or [] -> "true"
    | SymbolicExpression.Or [expr] ->
        string_of_expression expr_option' expr
    | SymbolicExpression.Or [expr; expr'] ->
        begin
          let nat = SymbolicExpression.nature expr
          and nat' = SymbolicExpression.nature expr' in
          match nat, nat' with
          | SymbolicExpression.Equality (expr1, expr2),
            SymbolicExpression.Greater (expr1', expr2') |
            SymbolicExpression.Greater (expr1', expr2'),
            SymbolicExpression.Equality (expr1, expr2)
            when expr1 == expr1' && expr2 == expr2' || expr1 == expr2' &&
              expr2 == expr1' ->
              (* Special case to recognize '>=' *)
              let s = Printf.sprintf "%s >= %s"
                (string_of_expression expr_option' expr1')
                (string_of_expression expr_option' expr2') in
              add_parenthesis expr_option s
          | _ ->
              let s = Printf.sprintf "%s or %s"
                (string_of_expression expr_option' expr)
                (string_of_expression expr_option' expr') in
              add_parenthesis expr_option s
        end
    | SymbolicExpression.Or exprs ->
        let exprs' = List.map (string_of_expression expr_option') exprs in
        add_parenthesis expr_option (String.concat " or " exprs')
    | SymbolicExpression.Parameter i ->
        Printf.sprintf "`%s`"
          model.Optimization.parameters.(i).Optimization.p_name
    | SymbolicExpression.PartialDerivative _ -> assert false
    | SymbolicExpression.Pre expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "pre(%s)" s
    | SymbolicExpression.RationalPower (expr, num) ->
        let s = Printf.sprintf "%s ^ (%s)"
          (string_of_expression expr_option' expr)
          (Num.string_of_num num) in
        add_parenthesis expr_option s
    | SymbolicExpression.Sign expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "sgn(%s)" s
    | SymbolicExpression.Sine expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "sin(%s)" s
    | SymbolicExpression.String s -> Printf.sprintf "\"%s\"" s
    | SymbolicExpression.Tangent expr ->
        let s = string_of_expression expr_option' expr in
        Printf.sprintf "tan(%s)" s
    | SymbolicExpression.TimeVariable -> "time"
    | SymbolicExpression.Variable i ->
        Printf.sprintf "`%s`"
          model.Optimization.variables.(i).Optimization.v_name
  and string_of_argument expr_option arg =
    let string_of_array_argument dims exprs =
      let rec repeat n sprintf (i, s) =
        if n = 0 then i, ""
        else if n = 1 then sprintf i s
        else
          let i, s = sprintf i s in
          repeat (n - 1) sprintf (i, s ^ ", ") in
      let rec string_of_array_argument' dim dims (i, s) = match dims with
      | [] ->
          repeat
            dim
            (fun i s -> i + 1, s ^ string_of_expression expr_option exprs.(i))
            (i, s)
      | dim' :: dims ->
          repeat
            dim
            (fun i s ->
               let s = s ^ "{" in
               let i, s = string_of_array_argument' dim' dims (i, s) in
               i, s ^ "}")
            (i, s) in
    match dims with
    | [] -> assert false
    | dim :: dims ->
        let _, s = string_of_array_argument' dim dims (0, "{") in
        s ^ "}" in
    match arg with
    | SymbolicExpression.ScalarArgument expr ->
        string_of_expression expr_option expr
    | SymbolicExpression.ArrayArgument (dims, exprs) ->
        string_of_array_argument dims exprs
  in
  Printf.fprintf oc "%s" (hide_spc (string_of_expression None expr))

let print_expression_option oc model expr_option =
  match expr_option with
  | None -> ()
  | Some expr -> print_expression oc model expr

let print_tree oc model ts =
  let rec print_tabs tabs =
    if tabs > 0 then begin
      Printf.fprintf oc "  ";
      print_tabs (tabs - 1);
    end in
  let string_of_kind = function
    | Input -> "input"
    | Parameter -> "fixed_parameter"
    | Variable -> "variable"
    | DiscreteVariable -> "discrete_variable" in
  let rec print_tree_element tabs = function
    | Node (s, ts) ->
        print_tabs tabs;
        Printf.fprintf oc "<struct>\n";
        print_tabs (tabs + 1);
        Printf.fprintf oc "<name>%s</name>\n" (hide_spc s);
        print_tabs (tabs + 1);
        Printf.fprintf oc "<subnodes>\n";
        List.iter (print_tree_element (tabs + 2)) ts;
        print_tabs (tabs + 1);
        Printf.fprintf oc "</subnodes>\n";
        print_tabs tabs;
        Printf.fprintf oc "</struct>\n"
    | Leaf (s, elt) ->
        print_tabs tabs; Printf.fprintf oc "<terminal>\n";
        print_tabs (tabs + 1);
        Printf.fprintf oc "<name>%s</name>\n" (hide_spc s);
        print_tabs (tabs + 1);
        Printf.fprintf oc "<kind>%s</kind>\n" (string_of_kind elt.kind);
        print_tabs (tabs + 1);
        Printf.fprintf oc "<id>%s</id>\n" (hide_spc elt.id);        
        print_tabs (tabs + 1);
        Printf.fprintf oc "<comment value=\"%s\"/>\n" (hide_spc elt.comment);
        print_tabs (tabs + 1);
        Printf.fprintf oc "<initial_value value=\"";
        print_expression_option oc model elt.initial_value;
        Printf.fprintf oc "\"/>\n";
        if elt.output then begin print_tabs (tabs + 1);
        Printf.fprintf oc "<output/>\n" end;
        if elt.kind <> Parameter && elt.initial_value <> None then
        begin
          print_tabs (tabs + 1);
          Printf.fprintf oc "<select/>\n"
        end;
        print_tabs tabs; Printf.fprintf oc "</terminal>\n"
  in
  Printf.fprintf oc "  <elements>\n";
  List.iter (print_tree_element 2) ts;
  Printf.fprintf oc "  </elements>\n"

let print_equations oc model =
  Printf.fprintf oc "  <equations>\n";
  Array.iteri
    (fun i equ ->
      Printf.fprintf oc "    <equation value=\"";
      if equ.Optimization.solved then
        let s = Printf.sprintf "`%s` = "
          model.Optimization.variables.(i).Optimization.v_name in
        Printf.fprintf oc "%s" (hide_spc s)
      else Printf.fprintf oc "0 = ";
      print_expression oc model equ.Optimization.expression;
      Printf.fprintf oc ";\"/>\n")
    model.Optimization.equations;
  Printf.fprintf oc "  </equations>\n"

let print_when_clauses oc model =
  Printf.fprintf oc "  <when_clauses>\n";
  List.iter
    (fun (cond, equs) ->
      Printf.fprintf oc "    <when_clause value=\"";
        Printf.fprintf oc "when ";
        print_expression oc model cond;
        Printf.fprintf oc " then ";
        List.iter
          (function
            | Optimization.Assign (expr, expr') ->
                print_expression oc model expr;
                Printf.fprintf oc " := ";
                print_expression oc model expr';
                Printf.fprintf oc "; "
            | Optimization.Reinit (expr, expr') ->
                Printf.fprintf oc "reinit(";
                print_expression oc model expr;
                Printf.fprintf oc ", ";
                print_expression oc model expr';
                Printf.fprintf oc "); ")
          equs;
        Printf.fprintf oc "end when;\"/>\n")
    model.Optimization.when_clauses;
  Printf.fprintf oc "  </when_clauses>\n"

let generate_XML filename fun_name model =
  let oc = open_out filename in
  Printf.fprintf oc "<model>\n";
  Printf.fprintf oc "  <name>%s</name>\n" (hide_spc fun_name);
  print_tree oc model (build_tree model);
  print_equations oc model;
  print_when_clauses oc model;
  Printf.fprintf oc "</model>\n";
  close_out oc
