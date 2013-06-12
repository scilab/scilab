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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//
//## scstxtedit : Input function of the text editor
//##    We exit from that function if Quit or use
//##    the commit button.
//##
//## Inputs : str_in : the input text to edit
//##          clos : a flag to close the window
//##                 0 : No
//##                 1 : close
//##
//##          str_out : the edited output text
//##                   can be [] if abort
//##          Quit : a flag to say if the buffer has been
//##                 closed
//##                 0 : No
//##                 1 : Quit
//##
function [str_out,Quit] = scstxtedit(str_in,ptxtedit)
    //** check lhs/rhs arg
    [lhs,rhs]=argn(0)

    //## param from ptxtedit
    clos = ptxtedit.clos
    typ  = ptxtedit.typ
    head = ptxtedit.head

    if clos<>1 then
        if head==[] then
            str_out = dialog(["DIALOG"], str_in) ;
        else
            str_out = dialog([head], str_in) ;
        end
    else
        str_out=[];
    end

    if str_out == [] then
        Quit = 1
    else
        Quit = 0
    end

endfunction
