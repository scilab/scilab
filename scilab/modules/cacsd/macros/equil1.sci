// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [t,siz]=equil1(p,q,tol)
    //

    [n,n]=size(p);
    [lhs,rhs]=argn(0);
    // t1
    if rhs==2 then
        [u,p,u,np]=svd(p);
    else
        [u,p,u,np]=svd(p,tol);
    end;
    p=sqrt(diag(p));p=[p(1:np);ones(n-np,1)];
    t1=diag(ones(n,1)./p)*u';
    //
    // t2
    q=diag(p)*(u'*q*u)*diag(p) // q1=t1**-1'*q*t1**-1
    if rhs==2 then
        [u,sigma1,u,nq]=svd(q(1:np,1:np))
    else
        [u,sigma1,u,nq]=svd(q(1:np,1:np),tol)
    end;
    sigma1=sqrt(diag(sigma1(1:nq,1:nq)))
    t2=[u',0*ones(np,n-np);0*ones(n-np,np) eye(n-np,n-np)];
    //
    // t3
    q=t2*q*t2';t3=eye(n,n);
    if np<>n then
        x=diag(ones(nq,1)./(sigma1.*sigma1))*q(1:nq,np+1:n)
        t3(1:nq,np+1:n)=-x;
        q=t3'*q*t3;// here t3 value is in reality t3**-1
        t3(1:nq,np+1:n)=x;
    end;
    //
    // t4
    if rhs==2 then
        [u,q,u,n3]=svd(q(np+1:n,np+1:n));
    else
        [u,q,u,n3]=svd(q(np+1:n,np+1:n),tol);
    end;
    t4=[diag(sqrt(sigma1))  0*ones(nq,n-nq);
    0*ones(np-nq,nq)   eye(np-nq,np-nq)       0*ones(np-nq,n-np);
    0*ones(n-np,np)               u']
    t=t4*t3*t2*t1
    siz=[nq,np-nq,n3]
endfunction
