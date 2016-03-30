// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y=mtlb_linspace(a,b,n)
    // Emulation function for linspace() Matlab function

    str=%F
    if type(a)==10 then
        a=asciimat(a)
        str=%T
    end
    if type(b)==10 then
        b=asciimat(b)
        str=%T
    end

    if or(type(a)==[4,6]) then
        a=bool2s(a)
    end
    if or(type(b)==[4,6]) then
        b=bool2s(b)
    end

    [lhs,rhs]=argn(0)

    if rhs==2 then
        if str then
            y=ascii(linspace(a,b))
        else
            y=linspace(a,b)
        end
    else
        if type(n)==10 then
            n=asciimat(n)
        end
        if or(type(n)==[4,6]) then
            n=bool2s(n)
        end
        if str then
            y=ascii(linspace(a,b,n))
        else
            y=linspace(a,b,n)
        end
    end
endfunction
