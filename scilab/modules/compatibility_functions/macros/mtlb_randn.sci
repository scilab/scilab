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

function r=mtlb_randn(a,opt)
    // Emulation function for randn() Matlab function

    [lhs,rhs]=argn()
    if rhs==1 then
        if and(size(a)==[1 1]) then
            r=rand(a,a,"normal")
        else
            tmp=list()
            for k=1:size(a,"*")
                tmp(k)=a(k)
            end
            tmp($+1)="normal"
            r=rand(tmp(1:$))
        end
    else
        if and(size(a)==[1 1]) then
            r=rand(a,a,"normal")
        else
            tmp=list()
            for k=1:size(a,"*")
                tmp(k)=a(k)
            end
            tmp($+1)="normal"
            r=rand(tmp(1:$))
        end
    end
endfunction
