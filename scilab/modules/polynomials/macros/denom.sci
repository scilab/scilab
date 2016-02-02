// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// ...
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


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
