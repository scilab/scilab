// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function x=%r_isreal(a, eps)
    [lhs, rhs]=argn(0);
    if(rhs == 1)
        x = isreal(a.num) & isreal(a.den);
    else
        x = isreal(a.num, eps) & isreal(a.den, eps);
    end
endfunction
