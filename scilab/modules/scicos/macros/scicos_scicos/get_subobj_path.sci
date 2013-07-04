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

function full_path=get_subobj_path(path)
    // path      : vector of integer gives the path of the required block from
    //             top level (main diagram) to bottom level (required block).
    //             Each vector entry is a block index in the current level structure.
    // full_path : is the  path relative to scs_m  of the required block
    //             scs_m.objs(full_path) is the data structure of the required block

    full_path=list("objs",path(1));
    for l=path(2:$),
        full_path($+1)="model";
        full_path($+1)="rpar";
        full_path($+1)="objs";
        full_path($+1)=l;
    end
endfunction
