// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function s=%sp_s_sp(a,b)
    // %spas - adds 2 sparse matrices (special cases)
    //!

    if size(a)==[1 1] then
        a=full(a)
        if a==0 then
            s=b,
        else
            s=a-full(b)
        end
    elseif size(b)==[1 1] then
        b=full(b)
        if b==0 then
            s=a
        else
            s=full(a)-b
        end
    end
endfunction
