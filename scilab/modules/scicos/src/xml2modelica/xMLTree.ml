(*
 *  XML to Modelica
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

type t =
  {
    name: string;
    elements: element list;
    equations: string list;
    when_clauses: string list
  }

and element =
  | Struct of structured_element
  | Terminal of terminal_element

and structured_element =
  {
    struct_name: string;
    subnodes: element list
  }

and terminal_element =
  {
    terminal_name: string;
    kind: kind;
    id: string;
    comment: string;
    initial_value: string;
    nominal: string;
    output: bool;
    select: bool;
    fixed: string
  }

and kind = Input | FixedParameter | Parameter | Variable | DiscreteVariable
