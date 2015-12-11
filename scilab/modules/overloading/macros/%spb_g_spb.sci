// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
