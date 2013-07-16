//  Scicos
//
//  Copyright (C) INRIA - Serge Steer <serge.steer@inria.fr>
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

function path=scs_full_path(P)
    // Given a path in the superclock hierachy, this function returns a path
    // in the scs_m data structure
    // P is a vector of numbers. All but the last entries are Superblocks index
    // path is a list such as scs_m(path) is the required object

    path=list("objs");
    for l=P(1:$-1),path($+1)=l;path($+1)="model";path($+1)= "rpar";path($+1)="objs";end
    path($+1)=P($);
endfunction
