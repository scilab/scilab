// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Bruno Pincon
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function r=%hm_h_hm(a,b)
    // define the operation a & b for boolean hypermatrices a and b
    // a & b is defined if a and b are boolean hypermatrices
    //                  and if a and b have the same dimensions
    //                      or if one have only one element
    if type(a.entries) ~= 4 | type(b.entries) ~= 4 then
        error(msprintf(_("%s: Wrong type for input argument(s): hypermatrix expected.\n"),"%hm_h_hm"));
    end
    if and(a.dims==b.dims) then
        r=hypermat(a.dims,a.entries & b.entries)
    elseif prod(a.dims) == 1 then
        r=hypermat(b.dims,a.entries & b.entries)
    elseif prod(b.dims) == 1 then
        r=hypermat(a.dims,a.entries & b.entries)
    else
        error(msprintf(_("%s: Incompatible operand dimensions.\n"),"%hm_h_hm"));
    end
endfunction
