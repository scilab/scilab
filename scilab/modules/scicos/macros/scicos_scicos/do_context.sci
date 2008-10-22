//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [context, ok] = do_context(context)

if context==[] then context=' ',end ; //** if the variable is empty put a blank space 

//** open a dialog window ; dialog() it is a wrapper of "x_dialog"
rep = dialog([
              'You may enter here scilab instructions to define '   ;
              'symbolic parameters used in block definitions using' ;
              'Scilab instructions; comments are not allowed.'      ; //** now the comments ARE ALLOWED ! 
              ' '                                                   ;
              'These instructions are evaluated once confirmed, i.e.,you';
              'click on OK, by Eval and every time diagram is loaded. '], context) ;
//** "rep" is a colum vector of strings 
if rep==[] then //** in case of no entry  
  ok = %f
else
  context = rep //** if some text is typed //** 
  ok = %t
end

endfunction
