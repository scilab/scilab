// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) Scilab Enterprises - Adeline CARNIS
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function res=%sp_norm(S,flag)

    [lhs,rhs]=argn(0)
    if rhs==1 then flag=2;end //norm(S)=norm(S,2)
    [m,n]=size(S)

    if m==1|n==1 then //vector norm
        [ij,v]=spget(S);
        res=norm(v,flag);
        return
    end

    select flag
    case 1 then
        res=max(ones(1,m)*abs(S))
    case 2 then
        if m<n then
            S1=S*S'
        elseif m>n then
            S1=S'*S
        else
            S1 = S;
        end

        tol=%eps;
        x = sum(abs(S1),1)';
        res = norm(x);
        if res==0 then return,end
        x = x/res;res0 = 0;
        while abs(res-res0) > tol*res
            res0 = res;   Sx = S1*x;
            
            // Bug #10178: norm failed for some sparse matrix
            // If Sx = 0, we had "division by zero" with x/norm(x)
            // So, use to sum(abs(S).^2).^(1/2)
            if Sx == 0 then
                res = sum(abs(S).^2).^(1/2);
                return
            end
            // End Bug #10178
            
            res = norm(Sx);
            x = S1'*Sx;
            x = x/norm(x);
        end
        if m<>n then res=sqrt(res),end
    case %inf then
        res=max(abs(S)*ones(n,1))
    case 'inf' then
        res=max(abs(S)*ones(n,1))
    case 'fro' then
        [ij,v]=spget(S);
        res=sqrt(sum(abs(v.*v)))
    else
        [ij,v]=spget(S);
        res=norm(v,flag);
    end
endfunction
