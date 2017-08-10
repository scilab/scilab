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

function z=mtlb_toeplitz(x,y)
    // Emulation function for Matlab toeplitz()

    if argn(2)==1 then
        if isreal(x) then
            if find(size(x)==1)==[] then
                tmp=toeplitz(x);
                s=size(x)
                z=tmp(1:s($),:)
            else
                z=toeplitz(x);
            end
            return
        end
        y=x
        x=conj(x)
        x(1)=conj(x(1))
    end
    if x(1)<>y(1) then
        y(1)=x(1)
    end
    if find(size(x)==1)==[] then
        tmp=toeplitz(x,y);
        s=size(x)
        z=tmp(1:s($),:)
    else
        z=toeplitz(x,y);
    end

endfunction
