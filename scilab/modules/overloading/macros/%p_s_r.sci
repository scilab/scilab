// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f=%p_s_r(m,f)
    //f=  m-f
    //author Serge Steer INRIA
    //!
    [num,den]=f(["num","den"])
    szf=size(den)
    szm=size(m)

    if and(szf>=0)&and(szm>=0) then
        if prod(szf)==1&prod(szm)>1 then
            den=den(ones(m))
            szf=szm
        end

        if size(szf,"*")>2 then
            num=num(:);den=den(:);m=m(:)
        end
        [num,den]=simp(-num+m.*den,den)
        num=matrix(num,szf)
        den=matrix(den,szf)
    else
        //at leat one matrix is eye*x
        if size(szf,"*")>2|size(szm,"*")>2 then
            error(9)
        end
        if or(szf<0)&or(szm<0) then
            [num,den]=simp(-num+m.*den,den)
        elseif or(szf<0) then
            [num,den]=simp(-num+m.*den,den*ones(m))
        elseif or(szm<0) then
            [num,den]=simp(-num+(m+0)*eye(den).*den,den)
        end
    end
    f=rlist(num,den,f.dt)
endfunction
