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

function y=mtlb_diag(x,dim)
    // Emulation function for diag() Matlab function


    rhs=argn(2)

    // B = diag(A)
    if rhs==1 then
        if type(x)==10 then
            x=mstr2sci(x);
            tmp=diag(x)
            tmp(find(tmp==" "))=ascii(0)
            y=[]
            for k=1:size(tmp,1)
                y=[y;strcat(tmp(k,:))]
            end
        elseif or(type(x)==[4,6]) then
            y=diag(bool2s(x))
        else
            y=diag(x)
        end
        // B = diag(A,k)
    else
        if type(x)==10 then
            x=asciimat(x);
            x=diag(x,dim)
            y=[]
            for k=1:size(x,1)
                tmp=mstr2sci(ascii(x(k,:)))
                tmp(find(tmp==ascii(0)))=ascii(0)
                y=[y;strcat(tmp)]
            end
        elseif or(type(x)==[4,6]) then
            y=diag(bool2s(x),dim)
        else
            y=diag(x,dim)
        end
    end
endfunction
