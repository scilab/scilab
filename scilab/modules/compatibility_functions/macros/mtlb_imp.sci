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
