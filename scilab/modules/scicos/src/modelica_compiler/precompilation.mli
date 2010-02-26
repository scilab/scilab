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

(** This module eases the compilation phase by sorting elements of the parse
tree obtained by calling the parsing functions. *)

(** The type of a preprocessed parse tree element. *)
type precompiled_class = {
  class_kind : ParseTree.class_kind;
  mutable extensions : extension list;
  mutable public_classes : (string * precompiled_class) list;
  mutable protected_classes : (string * precompiled_class) list;
  mutable public_cpnts : (string * precompiled_component) list;
  mutable protected_cpnts : (string * precompiled_component) list;
  mutable equs : ParseTree.equation_clause list;
  mutable algs : ParseTree.algorithm_clause list;
  mutable class_comment : ParseTree.string_comment;
}

and extension = string list * modification list

and modification =
    Modification of (string * ParseTree.array_subscripts) list *
      modification list * ParseTree.expression option

and precompiled_component = {
  class_name : string list;
  subscripts : ParseTree.array_subscripts;
  flow : ParseTree.flow option;
  variability : ParseTree.variability option;
  inout : ParseTree.inout option;
  modification : modification list * ParseTree.expression option;
  comment : ParseTree.comment;
}

(** [precompile tree] yields a precompiled class given a parse tree. Only one
class description is allowed per parse tree. *)
val precompile : ParseTree.t -> precompiled_class
