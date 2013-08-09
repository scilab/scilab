//  Scicos
//
//  Copyright (C) INRIA - Author : EADS-CCR
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

function y = multVectStr(M, x)
    // Function that multiply a sparse 1xn of double by
    // a 1x1 string, resulting in a nx1 vector of string
    // Input:
    // - M a 1xn sparse vector of double
    // - x a string
    // Ouput:
    // - y a nx1 vector of string

    // Check the syntax
    if(argn(1) <> 1) then
        error("Usage: y = multMatVect(M, x)");
    end
    if(M == []) then
        y=[];
        return;
    end
    // Check the dimensions
    [ij,v,mn_M]=spget(M);
    // Perform the multiplication
    y = emptystr(1, mn_M(2));
    for n=1:length(v)
        y(ij(n, 2)) = mulfv(msprintfv(v(n)) ,x);
    end
    if (y == "") then
        y=[];
    end
endfunction

