// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function K=mtlb_logical(A)
    // Emulation function for Matlab logical()

    if isempty(A) then
        K=[]
    elseif or(type(A)==[4,6]) then
        K=A
    else
        if type(A)==10 then
            A=mstr2sci(A)
        end
        K=A<>0
    end
endfunction
