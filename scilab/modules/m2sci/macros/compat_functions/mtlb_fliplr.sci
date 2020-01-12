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

function x=mtlb_fliplr(y)
    // Emulation function for fliplr() Matlab function

    if type(y)==1 then
        x=y(:,$:-1:1)
    elseif type(y)==10 then
        tmp = asciimat(y)
        tmp=tmp(:,$:-1:1)
        x=[];
        for k=1:size(tmp,1)
            x=[x;ascii(tmp(k,:))]
        end
    elseif type(y)==4 then
        tmp = bool2s(y)
        x=tmp(:,$:-1:1)
    end
endfunction


