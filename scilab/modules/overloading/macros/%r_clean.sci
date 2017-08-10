// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function a=%r_clean(a,epsa,epsr)
    //Syntax: a=%r_ clean(a,epsa,epsr)
    // Given a, matrix of rationals , this function
    // eliminates  all the coefficients of a with absolute value < epsa
    // and realtive value < epsr  (relative means realive wrt  norm 1 of
    // the coefficients)
    // Default values : epsa=1.d-10; epsr=1.d-10;
    //!


    [lhs,rhs]=argn(0)
    if rhs == 1 then
        epsa=1.d-10;
        epsr=1.d-10;
    elseif rhs==2 then
        epsr=1.d-10;
    end
    tdom=a("dt")
    a=simp(clean(a("num"),epsa,epsr)./clean(a("den"),epsa,epsr));a("dt")=tdom
endfunction
