// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function s = csgn(z)
    //Kahan, W., “Branch cuts for complex elementary functions, or, Much ado about nothing's sign bit”,
    //  Proceedings of the joing IMA/SIAM conference on The State of the Art
    //  in Numerical Analysis, University of Birmingham,
    //  A. Iserles & M.J.D. Powell, eds, Clarendon Press, Oxford, 1987, 165-210.

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"csgn", 1));
    end
    
    if and(typeof(z) <> ["constant" "sparse" "hypermat"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex, sparse or full matrix or hypermatrix expected.\n"), "csgn", 1));
    end

    s = -ones(z);
    s(real(z)>0|(real(z)==0&imag(z)>0))=1
    s(z==0) = %nan;
endfunction
