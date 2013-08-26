// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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


