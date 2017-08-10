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

function x=sinm(a)
    //   sinm - computes the matrix sine
    //%SYNTAX
    //   x=sinm(a)
    //%PARAMETERS
    //   a   : square  matrix
    //   x   : square  matrix

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"sinm",1));
    end

    if type(a)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"sinm",1));
    end

    if a==[] then x=[],return,end

    if norm(imag(a),1)==0 then
        x=imag(expm(%i*a));
    else
        x=-0.5*%i*(expm(%i*a)-expm(-%i*a));
    end

endfunction
