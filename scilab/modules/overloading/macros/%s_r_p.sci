// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f=%s_r_p(m,p)
    //f=M/p  M:scalar matrix p=polynomial
    //!


    [mp,np]=size(p)
    if mp*np<>1 then
        f=m*invr(p),
    else
        [l,c]=size(m)
        if m<>[] then m=m+poly(0,varn(p),"c"),end
        if mp==-1&l*c==1|l==-1 then
            f=rlist(m,p*eye(),[])
        else
            f=simp(rlist(m,p*ones(l,c),[]))
        end
    end
endfunction
