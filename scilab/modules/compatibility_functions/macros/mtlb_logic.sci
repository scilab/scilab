// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
