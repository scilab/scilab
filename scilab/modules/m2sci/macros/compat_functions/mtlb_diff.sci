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

function y=mtlb_diff(x,N,dim)
    // Emulation function for Matlab diff()

    [lhs,rhs]=argn()
    if rhs==1 then N=1,end
    if rhs<=2 then
        if length(size(x))>2 then
            dim=firstnonsingleton(x)
        elseif size(x,1)==1 | size(x,2)==1 then
            dim=0
        else
            dim="r"
        end

    end

    if dim==0 then
        y=diff(x,N)
    else
        if N>=size(x,"*") then
            y=[]
            return
        else
            for k=1:N
                x=diff(x,1,dim)
                if size(x,dim)==1 then
                    dim=firstnonsingleton(x)
                end
            end
        end
        if x==[] then
            y=0
        else
            y=x
        end
    end
endfunction
