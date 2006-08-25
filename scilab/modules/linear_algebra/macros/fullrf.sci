function [Q,M,rk]=fullrf(A,tol)
//[Q,M,rk]=fullrf(A)
//Full rank factorization : A=Q.M
//with range(Q)=range(A) and ker(M)=ker(A),
//Q full column rank , M full row rank
// rk = rank(A) = #columns(Q) = #rows(M)
//F.D.
//!
// Copyright INRIA
  [lhs,rhs]=argn(0)
  na1=norm(A,1);
  if rhs==1 then tol=sqrt(%eps);end
  if na1 < 1.d-10 then Q=[];M=[];rk=0;return;end
  tol1=tol*na1;
  [U,s,V,rk]=svd(A,tol1);
  sq=sqrt(s);
  Q=U*sq;M=sq*V';
  Q=Q(:,1:rk);M=M(1:rk,:);
endfunction


