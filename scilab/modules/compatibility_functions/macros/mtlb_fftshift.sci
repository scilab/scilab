// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y=mtlb_fftshift(x,dim)
    // Emulation function for fftshift() Matlab function

    rhs=argn(2)

    // Y = fftshift(X)
    if rhs==1 then
        if type(x)==10 then
            tmp=fftshift(mstr2sci(x))
            y=[]
            for k=1:size(tmp,1)
                y=[y;strcat(tmp(k,:))]
            end
        else
            y=fftshift(x)
        end
        // Y = fftshift(X,dim)
    else
        if dim<=size(size(x),"*") then
            if type(x)==10 then
                tmp=fftshift(mstr2sci(x),dim)
                y=[]
                for k=1:size(tmp,1)
                    y=[y;strcat(tmp(k,:))]
                end
            else
                y=fftshift(x,dim)
            end
        else
            y=x
        end
    end
endfunction
