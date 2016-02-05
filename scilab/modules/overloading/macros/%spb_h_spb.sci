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

function r=%spb_h_spb(a,b)
    //  perform logical elementwise and a&b where a and b are boolean sparse
    //  matrices

    if prod(size(a))==1 then
        if full(a) then
            r=b
        else
            r=sparse([],[],size(b))<>0
        end
    elseif prod(size(b))==1  then
        if full(b) then
            r=a
        else
            r=sparse([],[],size(a))<>0
        end
    else
        r=a&b
    end
endfunction
