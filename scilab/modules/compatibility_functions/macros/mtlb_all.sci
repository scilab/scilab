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

function r=mtlb_all(a,dim)
    // Emulation function for all() Matlab function

    rhs=argn(2)

    if type(a)==4 then a=bool2s(a);end

    // B=all(A)
    if rhs==1 then
        if ~isreal(a) then
            a=abs(a)
        end
        if size(a,1)==1|size(a,2)==1 then
            r=and(a)
        elseif size(a,1)==0|size(a,2)==0 then
            r=and(a)
        else
            r=and(a,1)
        end
        // B=all(A,dim)
    else
        // Because Matlab tolerate dim to be an non-existent dimension of a
        if dim>size(size(a),"*") then
            if isempty(a) then
                r=[]
            else
                r=a<>0
            end
        else
            if ~isreal(a) then
                a=abs(a)
            end
            r=and(a,dim);
        end
    end
endfunction
