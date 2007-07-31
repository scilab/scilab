function [n,m,xt,yt]=copfac(g,polf,polc,tol)
//[n,m,xt,yt]=copfac(G,[polf,polc,[tol]]) returns a right coprime 
//factorization of g :
// g = n*m^-1 where n and m are stable, proper and right coprime.
// (ie. [n m] left-invertible with stability)
// xt and yt satisfy:
//    [xt -yt].[m n]' = eye (Bezout identity)
// G is assumed stabilizable and detectable.
//-- G is is linear system (syslin list)
//-- polf are polc respectively the poles of xt and yt
//   and the poles of n and m .
//   These are optional arguments with defautl values -1.
//-- tol is a threshold for detecting stable poles.
//!
// Copyright INRIA
  [lhs,rhs]=argn(0),
  select typeof(g)
  case "rational" then
    g=tf2ss(g)
  case "state-space" then
  else
    error("first argument should be a linear dynamic system")
  end
  [r,p,t]=size(g);
  [a,b,c,d]=g(2:5),
  [n1,u1]=contr(a,b),[n2,u2]=contr(a',c'),
  select rhs,
  case 1 then
    polc=-ones(1,n1),polf=-ones(1,n2),tol=1000*%eps,
  case 2 then
    tol=polf,polc=-ones(1,n1),polf=-ones(1,n2),
  case 3 then tol=1000*%eps,
  end,
  //--------------------
  if n1<>t then
    w1=u1(:,n1+1:t),a1=w1'*a*w1,
    no=norm(a1),
    if maxi(real(spec(a1)))>no*tol then
      error('System is not stabilizable'),
    end,
  end,
  //------------------
  if n2<>t then
    w2=u2(:,n2+1:t),a2=w2'*a*w2,
    no=norm(a2),
    if maxi(real(spec(a2)))>no*tol then
      error('System is not detectable'),
    end,
  end,
  //-----------------------------
  v1=u1(:,1:n1),a1=v1'*a*v1,b1=v1'*b,
  k1=ppol(a1,b1,polc)*v1',
  ak1=a-b*k1,ck1=c-d*k1,
  v2=u2(:,1:n2),a2=v2'*a*v2,c2=c*v2,
  k2=ppol(a2',c2',polf),
  k2=v2*k2',
  ak2=a-k2*c,bk2=b-k2*d,
  //-------------------
  m=syslin('c',ak1,b,-k1,eye(p,p)),
  n=syslin('c',ak1,b,ck1,d),
  xt=syslin('c',ak2,-bk2,-k1,eye(p,p)),
  yt=syslin('c',ak2,k2,-k1),
endfunction
