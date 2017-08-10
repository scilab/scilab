
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
