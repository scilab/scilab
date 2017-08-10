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

function y=mtlb_logic(a,op,b)
    // Emulation function for Matlab logical operators

    // Because isreal function does not work with Boolean values
    if or(type(a)==[4,6]) then
        a=bool2s(a)
    end
    if or(type(b)==[4,6]) then
        b=bool2s(b)
    end

    // Because <, >, <=, and >= operators do not work with complex values
    if or(op==["<",">","<=",">="]) then
        if ~isreal(a) then
            a=real(a)
        end
        if ~isreal(b) then
            b=real(b)
        end
    end

    if a==[] | b==[] then
        y=[]
    else
        if op=="==" then
            y=a==b
        elseif op=="<" then
            y=a<b
        elseif op==">" then
            y=a>b
        elseif op=="<=" then
            y=a<=b
        elseif op==">=" then
            y=a>=b
        else
            y=a~=b
        end
    end
endfunction
