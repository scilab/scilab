// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
