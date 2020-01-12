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
