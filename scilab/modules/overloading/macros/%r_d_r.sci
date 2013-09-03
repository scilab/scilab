// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f=%r_d_r(s1,s2)
    // f=s1./s2
    //!
    if ndims(s1)<=2& ndims(s2)<=2 then
        [s1,s2]=sysconv(s1,s2)
        [num,den]=simp(s1.num.*s2.den,s1.den.*s2.num)
        f=rlist(num,den,s1.dt)
    else
        if size(s1,"*")==1 then
            sz=size(s2)
        else
            sz=size(s1)
        end
        [num,den]=simp(s1.num(:).*s2.den(:),s1.den(:).*s2.num(:))
        f=rlist(matrix(num,sz),matrix(den,sz),s1.dt)
    end
endfunction
