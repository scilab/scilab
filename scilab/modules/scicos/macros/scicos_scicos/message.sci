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

    function consoleDisplay(strings, modal, boxTitle, buttons)
        for i = 1:size(strings, '*')
            mprintf("%s\n", strings(i));
        end
    endfunction

    if getscilabmode() <> "NWNI" then
        warnUser = messagebox
    else
        warnUser = consoleDisplay
    end

    [lhs,rhs]=argn(0)
    if rhs==3 then
        if modal == %t then
            if buttons == "" then
                num=warnUser(strings, "modal", "scilab");
            else
                num=warnUser(strings, "modal", "scilab", buttons);
            end
        else //non modal messagebox
            if buttons == "" then
                num=warnUser(strings,"scilab");
            else
                num=warnUser(strings, "scilab", buttons);
            end
        end
    elseif rhs==2 then
        num=warnUser(strings, "modal", "scilab", buttons);
    else
        num=1
        warnUser(strings, "modal", "scilab");
    end
endfunction
