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

(** This module provides functions that generate code for the Scicos target. *)

val generate_code : string -> string -> string -> Optimization.model -> bool
  -> bool -> string option * string option -> unit
(** [generate_code path filename fun_name model jac only_outputs
     init_filenames]
generates the
code that permits the numeric simulation of [model]. [path] is the path to the
external functions referenced in [model]. [filename] is the name of the file
where the code is generated. [fun_name] is the name of the entry point in the
generated code. [jac] is a flag which true value indicates that a symbolic
jacobian is requested. [only_outputs] indicates that only output variables
have to be observed (this is the only possibility under Scicos so setting it
to [false] has no effect on code generation). [init_filenames] gives the
name of the initialization file so that initialization code is generated. *)
