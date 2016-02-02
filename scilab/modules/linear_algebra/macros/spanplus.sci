
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - François DELEBECQUE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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



