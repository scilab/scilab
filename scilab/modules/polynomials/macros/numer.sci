// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function num=numer(r)
    //returns the numerator num of a rational function matrix r (r may be
    //also a scalar or polynomial matrix
    //!
    select typeof(r)
    case "constant" then
        num=r;
    case "polynomial" then
        num=r;
    case "rational" then
        num=r.num
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A floating point number or polynomial or rational fraction array expected.\n"),"numer",1))
    end

endfunction
