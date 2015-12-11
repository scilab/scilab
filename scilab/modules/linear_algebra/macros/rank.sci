
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=rank(A,tol)
    //Matrix rank
    if type(A)==1 then
        if A==[] then r=0;return,end
        s = svd(A);
        if argn(2) == 1 then
            tol = max(size(A)) * s(1) * %eps;
        end
        r = size(find(s > tol),"*");
    else
        [t,n]=typename();n=stripblanks(n(find(t==type(A))))
        fun="%"+n+"_rank"
        if exists(fun)==1 then
            if argn(2)==1 then
                execstr("r="+fun+"(A)")
            else
                execstr("r="+fun+"(A,tol)")
            end
        else
            error(msprintf(gettext("%s: Function not defined for type ''%s''. Check argument or define function %s."),"rank",n,fun));
        end
    end
endfunction
