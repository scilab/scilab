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

function n=countblocks(scs_m)
    // count number of blocks used in the scicos data structure scs_m

    n=0
    for o=scs_m.objs
        x=getfield(1,o)
        if x(1)=="Block" then
            if o.model.sim=="super"|o.model.sim=="csuper" then
                n=n+countblocks(o.model.rpar)
            else
                n=n+1
            end
        else
            n=n+1
        end
    end
endfunction
