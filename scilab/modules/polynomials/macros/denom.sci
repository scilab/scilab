// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// ...
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function den=denom(r)
    //returns the denominator of a rational matrix
    //%Syntax: den=denom(r)
    //with
    //r: rational function matrix (may be polynomial or scalar matrix)
    //den: polynomial matrix
    //!
    select typeof(r)
    case "constant" then
        den=ones(r);
    case "polynomial" then
        den=ones(r);
    case "rational" then
        den=r.den
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A floating point number or polynomial or rational fraction array expected.\n"),"denom",1))
    end
endfunction
