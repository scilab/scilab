function [m1,m2,u2,sl2]=dt_ility(sl,tol)
//dual of st_ility
//detectability means m1=0;
//m1=dimension of unstable,unobservable subspace
//m2=dimension of unobservable subspace; m2>=m1
//sl2=ss2ss(sl,u2)
//!
// Copyright INRIA
[LHS,RHS]=argn(0)
if RHS==2 then
[n1,n2,u1,sl1]=st_ility(sl',tol);
else
[n1,n2,u1,sl1]=st_ility(sl');
end
[nx,nx]=size(sl(2));
u2=[u1(:,n1+1:nx),u1(:,n2+1:n1),u1(:,1:n2)];
sl2=ss2ss(sl,u2);
m1=nx-n1;m2=nx-n2;
endfunction
