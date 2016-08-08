// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function f = %p_r_p(p1,p2)
    // f = %p_r_p(p1,p2) <=> f= p1*(p2^(-1))
    //p1 : single polynomial or matrix of polynomials
    //p2 : polynomial scalar or matrix

    [l,c] = size(p2)
    if l*c <>1 then
        f = p1*invr(p2)
    else
        f = %p_d_p(p1, p2)   //   p1./p2
    end
endfunction
