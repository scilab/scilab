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

function [str]=mtlb_dec2hex(D,N)
    // Emulation function for Matlab dec2hex()

    rhs=argn(2)

    str=matrix(dec2hex(real(D)),-1,1)

    if isempty(str) then
        str=""
    end

    if rhs==2 then
        for k=1:size(str,1)
            if str(k)<>"" then
                str(k)=part("0",ones(1,N-length(str(k))))+str(k)
            end
        end
    end
endfunction


