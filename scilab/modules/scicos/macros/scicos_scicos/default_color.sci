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

function clr=default_color(typ)

    if typ==-1 then //event links
        clr=options("Link")(2),
    elseif typ==0 then  //text, block shape,
        if size(options("Background"),"*")>=2 then //compatibility
            clr=options("Background")(2),
        else
            clr=1
        end
    elseif typ==1 | typ==2 then //regular links

        clr=options("Link")(1),
    end
endfunction
