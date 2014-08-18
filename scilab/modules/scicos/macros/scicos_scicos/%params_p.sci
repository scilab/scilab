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

function %params_p(par)
    txt = params2txt(par);
    for i = 1:size(txt, 'r')
        mprintf("%s\n", txt(i))
    end
endfunction

function txt=params2txt(par)
    fn=getfield(1,par)
    txt=[]
    for k=2:size(fn,"*")
        txt=[txt
        sci2exp(par(fn(k)),fn(k))]
    end
endfunction
