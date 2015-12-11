// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
