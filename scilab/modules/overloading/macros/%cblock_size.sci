// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [m,n] = %cblock_size(M, o)
    n = size(definedfields(M),"*")-1;
    if ~isdef("o","l") then
        o = -1    // [m, n]
    elseif o=="r" | o==1
        o = 1
    elseif o=="c" | o==2
        o = 2
    elseif o=="*" | o==0
        o = 0
    else // 
        o = -1
    end
    if n==0 then
        m = 0
    else
        m = size(getfield(2,M),1);
        if o~=1
            n = 0
            for k = 2:size(definedfields(M),"*")
                n = n + size(getfield(k,M),2);
            end
        end
    end
    if argn(1)==1 then
        if o==0
            m = m*n
        elseif o==2
            m = n
        elseif o==-1
            m = [m,n]
        end
    end
endfunction
