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

function r=mtlb_any(a,dim)
    // Emulation function for any() Matlab function

    rhs=argn(2)

    if type(a)==4 then a=bool2s(a);end

    // B=any(A)
    if rhs==1 then
        if size(a,1)==1|size(a,2)==1 then
            r=or(a)
        elseif size(a,1)==0|size(a,2)==0 then
            r=or(a)
        else
            r=or(a,1)
        end
        // B=any(A,dim)
    else
        // Because Matlab tolerate dim to be an non-existent dimension of a
        if dim>size(size(a),"*") then
            if isempty(a) then
                r=[]
            else
                r=a<>0
            end
        else
            r=or(a,dim);
        end
    end
endfunction
