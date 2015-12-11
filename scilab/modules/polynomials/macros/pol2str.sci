// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function S = pol2str(p)
    if type(p) <> 2 & (typeof(p)~="hypermat" | type(p(:))~=2) then
        msg = _("%s: Wrong type for input argument #%d: Polynomial expected.\n")
        error(msprintf(msg, "pol2str", 1));
    end
    sp = size(p)
    p = p(:)
    d = max(degree(p))
    c = coeff(p)
    cr = real(c)
    ci = imag(c)
    S = stripblanks(string(c))
    k = find(c==0)
    S(k) = ""
    k = find(cr~=0 & ci~=0)
    k2 = find(k>length(p))
    S(k(k2)) = "+("+S(k(k2))+")"
    k = find((cr>0 & ci==0) | (cr==0 & ci>0))
    S(k) = "+"+S(k)
    vn = varn(p)
    clear ci cr k2
    for j = 2:(d+1)
        if j==2 then
            e = vn
        else
            e = vn+msprintf("^%d",j-1)
        end
        km1 = find(c(:,j)==-1)
        if km1~=[]
            S(km1,j) = "-"+e
        end
        kp1 = find(c(:,j)==1)
        if kp1~=[]
            S(kp1,j) = "+"+e
        end
        ko = find(c(:,j)~=-1 & c(:,j)~=0 & c(:,j)~=1)
        if ko~=[]
            S(ko,j) = S(ko,j)+"*"+e
        end
    end
    S = strcat(S,"","c")
    k = find(part(S,1)=="+")
    S(k) = part(S(k),2:$)
    k = find(S=="")
    S(k) = "0"
    S = matrix(S,sp)
endfunction
