// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y=mtlb_setstr(x)
    // Emulation function for setstr() Matlab function

    if type(x)==10 then
        y=x
    else
        if or(type(x)==[4,6]) then
            x=bool2s(x)
        end
        y=[]
        for k=1:size(x,1)
            y=[y;ascii(x(k,:))]
        end
        // When x is an empty matrix
        if isempty(y) then
            y=""
        end
    end
endfunction
