
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

function [x0,kerA]=linsolve(A,b,x0)
    %tol=1.D-10;
    // Finds all x solution to Ax+b=0;
    // x0=particular solution; kerA=nullspace of A
    // Any x=x0+kerA*w with arbitrary w solves A*x+b=0
    [LHS,RHS]=argn(0);
    select type(A)
    case 1 then        //full matrix
        Ab=[A,b];[ma,na]=size(Ab);
        [W,rk]=colcomp(Ab);
        W=W(:,1:na-rk);last=W(na,:);
        [W2,rk1]=colcomp(last);
        if rk1==0 then
            warning(gettext("Conflicting linear constraints."));
            x0=[];kerA=[];return;
        end
        W=W*W2;
        kerA=W(1:na-1,1:na-rk-1);
        if RHS==3 then
            if norm(A*x0+b,1)<%tol then
                return;
            end
            disp(gettext("Recomputing initial guess"));
        end
        piv=W(na,na-rk);x0=W(1:na-1,na-rk)/piv;
    case 5 then        //sparse matrix

        [ma,na]=size(A);
        %tol=1.D-10*max(abs(A))*max(ma,na);
        if ma<na then
        A=[A;sparse([],[],[na-ma,na])];b=[b;zeros(na-ma,1)];end
        if ma>na then
            //A=[A,sparse([],[],[ma,ma-na])];x0=[x0;zeros(ma-na,1)];
            b=A'*b;A=A'*A;ma=na;
        end
        [ptr,rkA]=lufact(A,[%tol,0.001]);
        [P,L,U,Q]=luget(ptr);
        if RHS==3 then
            if norm(A*x0+b,1)>%tol then
                x0=lusolve(ptr,-b);
                disp("recomputing initial guess");
            end
        end
        if RHS==2 then
            x0=lusolve(ptr,-b);
        end
        ludel(ptr);
        U=clean(U);
        [ptrU,rk]=lufact(U,[%tol,.001]);
        nma=max(na,ma);
        Y=[sparse([],[],[rkA,nma-rkA]);speye(nma-rkA,nma-rkA)];
        kerA=[];
        for k=1:na-rkA
            bb=full(Y(:,k));
            ww=sparse(lusolve(ptrU,bb));
            kerA=[kerA,ww];
        end
        if na<>rkA then
            kerA=clean(Q'*kerA);
        end
        if norm(A*x0+b,1)>%tol then
            warning(msprintf(gettext("Possible Conflicting linear constraints, error in the order of %s"),string(norm(A*x0+b,1)) ));
        end
        if ma>na then kerA=kerA(1:na,:);x0=x0(1:na,1);end
        ludel(ptrU);
    end
endfunction
