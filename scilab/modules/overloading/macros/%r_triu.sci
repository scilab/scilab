// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1995-2010 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function d = %r_triu(a,k)
    // r_triu - implement triu function for  rationnal matrix ,..

    if argn(2)<2 then
        k=0,
    else
        fname = "%r_triu";
        if type(k)<>1 then
            msg = _("%s: Argument #%d: Decimal number(s) expected.\n")
            error(msprintf(msg, fname, 2))
        end
        if size(k,"*")<>1 then
            msg = _("%s: Argument #%d: Scalar (1 element) expected.\n")
            error(msprintf(msg, fname, 2))
        end
        if ~isreal(k) then
            msg = _("%s: Argument #%d: Decimal number(s) expected.\n")
            error(msprintf(msg, fname, 2))
        end
    end

    d = rlist(triu(a.num,k),triu(a.den,k)+tril(ones(a.den),k-1),a.dt)
endfunction
