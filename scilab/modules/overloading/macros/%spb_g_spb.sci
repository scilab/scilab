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
// === LICENSE_END ===

function r=%spb_g_spb(a,b)
    //  perform logical elementwise or a|b where a and b are boolean sparse
    //  matrices

    if prod(size(a))==1 then
        if full(a) then
            [mb,nb]==size(b)
            r=(ones(mb,nb)==1)
        else
            r=b
        end
    elseif prod(size(b))==1  then
        if full(b) then
            [ma,na]==size(a)
            r=(ones(ma,na)==1)
        else
            r=a
        end
    else
        r=a|b
    end
endfunction
