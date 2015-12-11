// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y=mtlb_imp(a,b,c)
    // Emulation function for Matlab colon

    [lhs,rhs]=argn();

    if rhs==2 then
        if a==[] | b==[] then
            y=[]
        else
            y=a:b
        end
    else
        if a==[] | b==[] | c==[] then
            y=[]
        else
            y=a:b:c
        end
    end
endfunction
