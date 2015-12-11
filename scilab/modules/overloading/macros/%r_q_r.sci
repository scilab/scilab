// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function s1=%r_q_r(s1,s2)
    // %r_q_r(s1,s2) <=> s1= s1.\s2   for rationals
    //author Serge Steer, INRIA
    //!
    if ndims(s1)<=2& ndims(s2)<=2 then
        [s1,s2]=sysconv(s1,s2)
        [num,den]=simp(s1.den.*s2.num,s1.num.*s2.den)
        s1=rlist(num,den,s1.dt)
    else
        if size(s1,"*")==1 then
            sz=size(s2)
        else
            sz=size(s1)
        end
        [num,den]=simp(s1.den(:).*s2.num(:),s1.num(:).*s2.den(:))
        s1=rlist(matrix(num,sz),matrix(den,sz),s1.dt)
    end

endfunction
