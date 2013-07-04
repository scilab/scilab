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

module Compiler = OptimizingCompiler.Make (ScicosCodeGeneration)

let name = Sys.argv.(0)

let _ =
  Printf.printf "This is Modelicac v.%s for Xcos.\n" Compiler.version;
  Printf.printf "%s\n"
    (String.sub Compiler.sccs_id 4 (String.length Compiler.sccs_id - 4));
  flush stdout

let () = Compiler.run ()
