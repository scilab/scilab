
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function X=pinv(A,tol)
    //Pseudo inverse
    if type(A)==1 then
        if A==[] then X=[],return,end

        [U,S,V] = svd(A,"e");
        S = diag(S)
        if argn(2) < 2
            tol = max(size(A)) * S(1) * %eps;
        end
        r=size(find(S>tol),"*") //Rank
        if r == 0 then
            X = zeros(A');
        else
            //X=V1*inv(S1)*U1'
            X = V(:,1:r)* diag(ones(r,1)./S(1:r)) *U(:,1:r)';
        end
    else
        [t,n]=typename();n=stripblanks(n(find(t==type(A))))
        fun="%"+n+"_pinv"
        if exists(fun)==1 then
            if argn(2)==1 then
                execstr("X="+fun+"(A)")
            else
                execstr("X="+fun+"(A,tol)")
            end
        else
            error(msprintf(gettext("%s: Function not defined for type ''%s''. Check argument or define function %s."),"pinv",n,fun));
        end
    end
endfunction

