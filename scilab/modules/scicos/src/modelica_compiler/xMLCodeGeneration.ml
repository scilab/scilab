
(*  Scicos *)
(* *)
(*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr> *)
(* *)
(* This program is free software; you can redistribute it and/or modify *)
(* it under the terms of the GNU General Public License as published by *)
(* the Free Software Foundation; either version 2 of the License, or *)
(* (at your option) any later version. *)
(* *)
(* This program is distributed in the hope that it will be useful, *)
(* but WITHOUT ANY WARRANTY; without even the implied warranty of *)
(* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the *)
(* GNU General Public License for more details. *)
(* *) 
(* You should have received a copy of the GNU General Public License *)
(* along with this program; if not, write to the Free Software *)
(* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. *)
(*  *)
(* See the file ./license.txt *)


type 'a tree = Leaf of (string * 'a) | Node of string * 'a tree list

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
  let rec print_expression' expr = match SymbolicExpression.nature expr with
    | SymbolicExpression.Addition [] -> Printf.fprintf oc "0"
    | SymbolicExpression.Addition [expr] -> print_expression' expr
    | SymbolicExpression.Addition (expr :: exprs) ->
        Printf.fprintf oc "(";
        print_expression' expr;
        List.iter
          (fun expr ->
            Printf.fprintf oc " + ";
            print_expression' expr)
          exprs;
        Printf.fprintf oc ")"
    | SymbolicExpression.And [] -> Printf.fprintf oc "false"
    | SymbolicExpression.And [expr] -> print_expression' expr
    | SymbolicExpression.And (expr :: exprs) ->
        Printf.fprintf oc "(";
        print_expression' expr;
        List.iter
          (fun expr ->
            Printf.fprintf oc " and ";
            print_expression' expr)
         exprs;
        Printf.fprintf oc ")"
    | SymbolicExpression.ArcCosine expr ->
        Printf.fprintf oc "(Modelica.Math.acos(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.ArcHyperbolicCosine expr ->
        Printf.fprintf oc "(Modelica.Math.acosh(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.ArcHyperbolicSine expr ->
        Printf.fprintf oc "(Modelica.Math.asinh(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.ArcHyperbolicTangent expr ->
        Printf.fprintf oc "(Modelica.Math.atanh(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.ArcSine expr ->
        Printf.fprintf oc "(Modelica.Math.asin(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.ArcTangent expr ->
        Printf.fprintf oc "(Modelica.Math.atan(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.BlackBox (s, []) -> Printf.fprintf oc "(%s())" s
    | SymbolicExpression.BlackBox (s, [expr]) ->
        Printf.fprintf oc "(%s(" s;
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.BlackBox (s, (expr :: exprs)) ->
        Printf.fprintf oc "(%s(" s;
        print_expression' expr;
        List.iter
          (fun expr ->
            Printf.fprintf oc ", ";
            print_expression' expr)
         exprs;
        Printf.fprintf oc "))"
    | SymbolicExpression.BooleanValue false -> Printf.fprintf oc "false"
    | SymbolicExpression.BooleanValue true -> Printf.fprintf oc "true"
    | SymbolicExpression.Constant s -> Printf.fprintf oc "(%s)" s
    | SymbolicExpression.Cosine expr ->
        Printf.fprintf oc "(Modelica.Math.cos(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.Derivative (expr, Num.Int 1) ->
        Printf.fprintf oc "(der(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.Derivative _ -> assert false
    | SymbolicExpression.DiscreteVariable i when i >= 0 ->
        Printf.fprintf oc "`%s`" model.Optimization.discrete_variables.(i).Optimization.d_v_name
    | SymbolicExpression.DiscreteVariable i ->
        Printf.fprintf oc "`%s`" model.Optimization.inputs.(-1 - i)
    | SymbolicExpression.Equality (expr, expr') ->
        Printf.fprintf oc "(";
        print_expression' expr;
        Printf.fprintf oc " == ";
        print_expression' expr';
        Printf.fprintf oc ")"
    | SymbolicExpression.Exponential expr ->
        Printf.fprintf oc "(Modelica.Math.exp(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.Floor expr ->
        Printf.fprintf oc "(Modelica.Math.floor(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.Greater (expr, expr') ->
        Printf.fprintf oc "(";
        print_expression' expr;
        Printf.fprintf oc " > ";
        print_expression' expr';
        Printf.fprintf oc ")"
    | SymbolicExpression.HyperbolicCosine expr ->
        Printf.fprintf oc "(Modelica.Math.cosh(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.HyperbolicSine expr ->
        Printf.fprintf oc "(Modelica.Math.sinh(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.HyperbolicTangent expr ->
        Printf.fprintf oc "(Modelica.Math.tanh(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.If (expr, expr', expr'') ->
        Printf.fprintf oc "(if ";
        print_expression' expr;
        Printf.fprintf oc " then ";
        print_expression' expr';
        Printf.fprintf oc " else ";
        print_expression' expr'';
        Printf.fprintf oc ")"
    | SymbolicExpression.Logarithm expr ->
        Printf.fprintf oc "(Modelica.Math.log(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.Multiplication [] -> Printf.fprintf oc "1"
    | SymbolicExpression.Multiplication [expr] -> print_expression' expr
    | SymbolicExpression.Multiplication (expr :: exprs) ->
        Printf.fprintf oc "(";
        print_expression' expr;
        List.iter
          (fun expr ->
            Printf.fprintf oc " * ";
            print_expression' expr)
         exprs;
        Printf.fprintf oc ")"
    | SymbolicExpression.Not expr ->
        Printf.fprintf oc "(not(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.Number num -> Printf.fprintf oc "(%g)" (Num.float_of_num num)
    | SymbolicExpression.Or [] -> Printf.fprintf oc "true"
    | SymbolicExpression.Or [expr] -> print_expression' expr
    | SymbolicExpression.Or [expr; expr'] ->
        begin match SymbolicExpression.nature expr, SymbolicExpression.nature expr' with
          | SymbolicExpression.Equality (expr1, expr2),
            SymbolicExpression.Greater (expr1', expr2') |
            SymbolicExpression.Greater (expr1', expr2'),
            SymbolicExpression.Equality (expr1, expr2)
            when expr1 == expr1' && expr2 == expr2' || expr1 == expr2' && expr2 == expr1' ->
              (* Special case to recognize '>=' *)
              Printf.fprintf oc "(";
              print_expression' expr1';
              Printf.fprintf oc " >= ";
              print_expression' expr2';
              Printf.fprintf oc ")"
          | _ ->
              Printf.fprintf oc "(";
              print_expression' expr;
              Printf.fprintf oc " or ";
              print_expression' expr';
              Printf.fprintf oc ")"
        end
    | SymbolicExpression.Or (expr :: exprs) ->
        Printf.fprintf oc "(";
        print_expression' expr;
        List.iter
          (fun expr ->
            Printf.fprintf oc " or ";
            print_expression' expr)
         exprs;
        Printf.fprintf oc ")"
    | SymbolicExpression.Parameter i ->
        Printf.fprintf oc "`%s`" model.Optimization.parameters.(i).Optimization.p_name
    | SymbolicExpression.PartialDerivative _ -> assert false
    | SymbolicExpression.RationalPower (expr, num) ->
        Printf.fprintf oc "(";
        print_expression' expr;
        Printf.fprintf oc " ^ (%s))" (Num.string_of_num num)
    | SymbolicExpression.Sign expr ->
        Printf.fprintf oc "(Modelica.Math.sgn(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.Sine expr ->
        Printf.fprintf oc "(Modelica.Math.sin(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.Tangent expr ->
        Printf.fprintf oc "(Modelica.Math.tan(";
        print_expression' expr;
        Printf.fprintf oc "))"
    | SymbolicExpression.TimeVariable -> Printf.fprintf oc "time"
    | SymbolicExpression.Variable i ->
        Printf.fprintf oc "`%s`" model.Optimization.variables.(i).Optimization.v_name
  in
  print_expression' expr

let print_tree oc model ts =
  let rec print_tabs tabs =
    if tabs > 0 then begin
      Printf.fprintf oc "  ";
      print_tabs (tabs - 1);
    end
  in
  let string_of_kind = function
    | Input -> "input"
    | Parameter -> "fixed_parameter"
    | Variable -> "variable"
    | DiscreteVariable -> "discrete_variable"
  in
  let start_value = function
    | None -> SymbolicExpression.zero
    | Some expr -> expr
  in
  let rec print_tree_element tabs = function
    | Node (s, ts) ->
        print_tabs tabs; Printf.fprintf oc "<struct>\n";
        print_tabs (tabs + 1); Printf.fprintf oc "<name>%s</name>\n" s;
        print_tabs (tabs + 1); Printf.fprintf oc "<subnodes>\n";
        List.iter (print_tree_element (tabs + 2)) ts;
        print_tabs (tabs + 1); Printf.fprintf oc "</subnodes>\n";
        print_tabs tabs; Printf.fprintf oc "</struct>\n"
    | Leaf (s, elt) ->
        print_tabs tabs; Printf.fprintf oc "<terminal>\n";
        print_tabs (tabs + 1); Printf.fprintf oc "<name>%s</name>\n" s;
        print_tabs (tabs + 1); Printf.fprintf oc "<kind>%s</kind>\n" (string_of_kind elt.kind);
        print_tabs (tabs + 1); Printf.fprintf oc "<id>%s</id>\n" elt.id;        
        print_tabs (tabs + 1); Printf.fprintf oc "<comment value=\"%s\"/>\n" elt.comment;        
        print_tabs (tabs + 1); Printf.fprintf oc "<initial_value value=\"";
        print_expression oc model (start_value elt.initial_value);
        Printf.fprintf oc "\"/>\n";
        if elt.output then begin print_tabs (tabs + 1); Printf.fprintf oc "<output/>\n" end;
        if elt.kind <> Parameter && elt.initial_value <> None then begin
          print_tabs (tabs + 1); Printf.fprintf oc "<select/>\n"
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
        Printf.fprintf oc "`%s` = " model.Optimization.variables.(i).Optimization.v_name
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
  Printf.fprintf oc "  <name>%s</name>\n" fun_name;
  print_tree oc model (build_tree model);
  print_equations oc model;
  print_when_clauses oc model;
  Printf.fprintf oc "</model>\n";
  close_out oc
