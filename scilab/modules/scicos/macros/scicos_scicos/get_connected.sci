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

function connected=get_connected(scs_m,k,typ)
    //return the vector of number of link connected to a given block
    //   scs_m      :   structure of blocks and links
    //   k          :   block_number
    //   typ        :   'in','out','clkin','clkout'
    //   connected  :   vector of connected link numbers
    [lhs,rhs]=argn(0)

    connected=[]

    //disp('get_connected')
    // pause
    if rhs<=2 then // all connected links
        graphics=scs_m.objs(k).graphics

        ip=graphics.pin
        connected=[connected ip(find(ip>0))']

        op=graphics.pout
        connected=[connected op(find(op>0))']

        cip=graphics.pein
        connected=[connected cip(find(cip>0))']

        cop=graphics.peout
        connected=[connected cop(find(cop>0))']
    else

        if typ=="in" then
            ip=scs_m.objs(k).graphics.pin
            connected=[connected ip(find(ip>0))'],
        elseif typ=="out" then
            op=scs_m.objs(k).graphics.pout
            connected=[connected op(find(op>0))'],
        elseif typ=="clkin" then
            cip=scs_m.objs(k).graphics.pein
            connected=[connected cip(find(cip>0))'],
        elseif typ=="clkout" then
            cop=scs_m.objs(k).graphics.peout
            connected=[connected cop(find(cop>0))'],
        end
    end

endfunction
