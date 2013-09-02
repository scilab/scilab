
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - François DELEBECQUE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [Q,M]=pbig(A,thres,flag)
    //Projection on eigensubspace associated with eigenvalues
    //with real part >= thres (flag='c') or with modulus >= thres (flag='d')
    //Projection is defined by Q*M. Eigenvalues of M*A*Q = eigenvalues
    //of A with real part >= thres (case 'c',...).
    //If [Q1,M1]== full rank factorization (fullrf) of eye-Q*M then evals of
    // M1*A*Q1 = evals of A with real part < thres (case 'c',...).
    // See also psmall.
    //!

    [n,n]=size(A);
    thres=real(thres);
    if flag=="c" then
        deff("[flag]=%smallei(x)","flag=real(x) >= thres")
        deff("[flag]=%bigeig(x)","flag=real(x) < thres")
    elseif flag=="d" then
        deff("[flag]=%smallei(x)","flag=abs(x) >= thres")
        deff("[flag]=%bigeig(x)","flag=abs(x) < thres")
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"pbig",3,"''c'',''d''"));
    end
    //
    [X,dsmall] = schur(A,%smallei);
    [Y,dbig] = schur(A,%bigeig);
    Q=X(:,1:dsmall);
    if Q==[] then M=[];return;end
    Y1=Y';
    M1=Y1(dbig+1:n,:);
    E=M1*Q;
    if rcond(E)>1.d-7 then
        M=E\M1;
    else
        [Ab,X0]=balanc(A);
        [X,dsmall] = schur(Ab,%smallei);X1=X*X0;Q=X1(:,1:dsmall);
        [Y,dbig] = schur(Ab,%bigeig);Y1=inv(X0)*Y';M=Y1(dbig+1:n,:);
        E=M*Q;
        M=E\M;
    end
endfunction
