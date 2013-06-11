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
// See the file ./license.txt
//

function [txt,files]=returntoscilab()
    files= listfiles(TMPDIR+"/Workspace")
    n=size(files,1)
    if n==0 then
        txt=[],
        return,
    else
        txtg="["+files(1)
        txtd="resume("+files(1)
        for i=2:n
            txtg=txtg+","+files(i)
            txtd=txtd+","+files(i)
        end
        txtg=txtg+"]"
        txtd=txtd+")"
        txt=txtg+"="+txtd
    end
endfunction
