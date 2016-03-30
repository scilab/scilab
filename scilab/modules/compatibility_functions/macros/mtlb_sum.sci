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

function r=mtlb_sum(a,dim)
    // Emulation function for sum() Matlab function

    rhs=argn(2)

    if rhs==1 then
        // Scalar, vector or empty matrix
        if find(size(a)==1)<>[] | find(size(a)==0)<>[] then
            r=sum(a)
        else
            r=sum(a,1)
        end
    else
        if dim<=size(size(a),"*") then
            r=sum(a,dim)
        else
            r=a
        end
    end
endfunction

