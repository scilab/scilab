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

function f=%p_l_s(p,m)
    // f=%p_l_s(p,m) <=>f= p\m.
    //!
    [mp,np]=size(p)
    if mp*np<>1 then
        f=invr(p)*m,
    else
        [l,c]=size(m)
        if m<>[] then m=m+poly(0,varn(p),"c"),end
        if mp==-1&l*c==1|l==-1 then
            f = rlist(m,p*eye(),[])
        else
            f = rlist(m,p*ones(l,c),[])
        end
    end
endfunction
