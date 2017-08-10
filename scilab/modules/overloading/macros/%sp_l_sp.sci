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

function x=%sp_l_sp(a,b)
    // a\b , a sparse b sparse

    [ma,na]=size(a)
    [mb,nb]=size(b)
    if ma<>mb then error(12),end
    if ma<>na then
        b=a'*b;a=a'*a
    end

    [h,rk]=lufact(a)
    if rk<min(ma,na) then warning("deficient rank: rank = "+string(rk)),end
    x=[]
    for k=1:nb
        x=[x,sparse(lusolve(h,full(b(:,k))))]
    end
    ludel(h)
endfunction
