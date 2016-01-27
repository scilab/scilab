// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Bruno Pincon
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function r=%hm_g_hm(a,b)
    // define the operation a | b for boolean hypermatrices a and b
    // a | b is defined if a and b are boolean hypermatrices
    //                  and if a and b have the same dimensions
    //                      or if one have only one element
    if type(a.entries) ~= 4 | type(b.entries) ~= 4 then
        error(msprintf(_("%s: Operand is not a boolean hypermatrix.\n"),"%hm_g_hm"));
    end
    if and(a.dims==b.dims) then
        r=hypermat(a.dims,a.entries | b.entries)
    elseif prod(a.dims) == 1 then
        r=hypermat(b.dims,a.entries | b.entries)
    elseif prod(b.dims) == 1 then
        r=hypermat(a.dims,a.entries | b.entries)
    else
        error(msprintf(_("%s: Incompatible operand dimensions.\n"),"%hm_g_hm"));
    end
endfunction
