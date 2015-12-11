// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
