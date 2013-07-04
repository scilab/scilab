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

function varargout=%Block_e(i,o)
    //function used only for backward compatibility of scicos blocks gui
    warning("Obsolete use of o(i) in this scicos block")
    varargout=list()
    for k=1:size(i,"*")
        ik=i(k)
        if ik==1 then
            varargout($+1)="Block"
        else
            varargout($+1)=getfield(ik,o)
        end
    end
endfunction
