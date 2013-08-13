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

function num=message(strings ,buttons, modal)
    //interface to message primitive to allow simple overloading for live demo
    [lhs,rhs]=argn(0)
    if rhs==3 then
        if modal == %t then
            if buttons == "" then
                num=messagebox(strings, "modal", "scilab");
            else
                num=messagebox(strings, "modal", "scilab", buttons);
            end
        else //non modal messagebox
            if buttons == "" then
                num=messagebox(strings,"scilab");
            else
                num=messagebox(strings, "scilab", buttons);
            end
        end
    elseif rhs==2 then
        num=messagebox(strings, "modal", "scilab", buttons);
    else
        num=1
        messagebox(strings, "modal", "scilab");
    end
endfunction
