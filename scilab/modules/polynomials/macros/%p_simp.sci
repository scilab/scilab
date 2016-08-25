// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

function [num, den] = %p_simp(num, den)
    // Called when num or/and den are complex-encoded

    // Special cases when real or imaginary parts are both null:
    //   http://bugzilla.scilab.org/8493

    //   Case of both real null parts
    s = real(num)==0 & real(den)==0
    [tmpN,tmpD] = simp(imag(num(s)), imag(den(s)))
    num(s) = tmpN
    den(s) = tmpD

    //   Case of both imag null parts
    s = imag(num)==0 & imag(den)==0
    [tmpN,tmpD] = simp(real(num(s)), real(den(s)))
    num(s) = tmpN
    den(s) = tmpD
endfunction
