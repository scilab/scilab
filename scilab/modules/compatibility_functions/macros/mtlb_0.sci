// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y=mtlb_0(a)
    // Emulation function for Matlab dot-transposition

    if type(a)==10 then
        tmp=asciimat(a).'
        y=[]
        for k=1:size(tmp,1)
            y(k)=strcat(ascii(tmp(k,:)))
        end
    elseif or(type(a)==[4,6]) then
        y=bool2s(a).'
    else
        y=a.'
    end

endfunction
