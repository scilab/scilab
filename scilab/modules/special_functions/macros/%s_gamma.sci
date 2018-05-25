// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = %s_gamma(x)
    // call for complex numbers or/and for hypermatrix of reals
    s = size(x);
    x = x(:);
    if isreal(x, 0)
        y = gamma(real(x));
    else
        if isdef("%s_gamma_user") & type(%s_gamma_user)==13
            y = %s_gamma_user(x);
        else
            msg = _("%s: Function not defined for the given argument type.\n  Check arguments or define function %s for overloading.\n")
            error(msprintf(msg, "%s_gamma", "%s_gamma_user()"))
        end
    end
    y = matrix(y, s);
endfunction
