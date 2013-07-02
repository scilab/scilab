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

function o=mark_prt(o,prt_number,inout,typ,v)
    //mark a port of a block free or used
    // o           : block data structure
    // prt_number  : port number
    // inout       : port orientation ('in' or 'out')
    // typ         : port type
    //               1  : standard port
    //              -1  : event port
    // v           : value to assign
    //               k>0 : port is connected to link #k
    //               0 : port is free

    if inout=="out" then //set an output port
        if typ==1 | typ==2 then  //standard (regular or implicit) port
            o.graphics.pout(prt_number)=v;
        else //clock port
            o.graphics.peout(prt_number)=v;
        end
    else //set an input port
        if typ==1 | typ==2 then  //standard (regular or implicit) port
            o.graphics.pin(prt_number)=v;
        else //clock port
            o.graphics.pein(prt_number)=v;
        end
    end
endfunction
