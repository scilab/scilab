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

function cors=shiftcors(cors,ns)
    n=size(cors)
    for k=1:n
        if type(cors(k))==15 then
            cors(k)=shiftcors(cors(k),ns)
        else
            if cors(k)<0 then cors(k)=cors(k);
            elseif cors(k)<>0 then cors(k)=cors(k)+ns,end
        end
    end
endfunction
