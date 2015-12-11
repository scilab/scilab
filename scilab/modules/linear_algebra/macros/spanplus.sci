
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - François DELEBECQUE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [x,dim,dima]=spanplus(a,b,tol)
    //[X,dim,dima]=spanplus(A,B,tol) computes an orthogonal basis of
    // a+b such that : the first dima columns of x span Range(A)
    // and the following (dim-dima) columns make a basis of a+b
    // relative to a. tol is an optional argument.
    // The dim first columns of x make a basis for A+B.
    //!
    [na,ma]=size(a);[nb,mb]=size(b);
    if na*ma==0 then
        dima=0;[x,dim]=rowcomp(b);x=x';return;
    end
    if nb*mb==0 then [x,dima]=rowcomp(a);dim=dima;x=x';return;end
    [lhs,rhs]=argn(0);
    if rhs==2 then tol=%eps*na*nb*ma*mb;end
    if na<>nb then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"spanplus",1,2));
    end
    [x,dima]=rowcomp(a);
    b=x*b;x=x'    //update b,x
    if dima == na then dim=dima,return,end;
    low=(dima+1):na;
    blow=b(low,:);
    if norm(blow,1) <= tol*norm(b,1),dim=dima,return,end
    [u2,r2]=rowcomp(blow);
    dim=dima+r2;
    x(:,low)=x(:,low)*u2';    //update
endfunction



