function [go,xo]=par(a,b,c,rx,cx);
//
// [go,xo]=par(a,b,c,rx,cx) solves the minimization problem:
//
//	    || a   b ||
//     min  ||       ||
//      X   || c   x ||2
//
//  an explicit solution is
//                   2      T  -1  T
//      Xo = - c ( go  I - a a)   a b
//  where			        T   T
//      go = max ( || (a , b) || , || (a , c) || )
//
//  rx and cx are the dimensions of Xo (optional if a is nondegenerate)
//
//!
//constant
// Copyright INRIA
TOLA=1.0e-8; // threshold used to discard near singularities in
//		gs I - A'*A
 
 
go=maxi(norm([a b]),norm([a;c]));
[ra,cb]=size(b); [rc,ca]=size(c); xo=0;
 
 
//MONITOR LIMIT CASES
//--------------------
if ra==0 | ca == 0 | go == 0 then xo(rx,cx)=0; return; end
 
 
 
//COMPUTE Xo IN NONTRIVIAL CASES
//------------------------------
 
gs=(go**2);
[u,s,v]=svd(a);
 
//form  gs I - s' * s
ns=mini(ra,ca);
d=diag(s);
dd=gs*ones(d)-d**2;
 
 
//isolate the non (nearly) singular part of gs I - A'*A
nnz1=nthresh(dd/go,TOLA);
nd=ns-nnz1;   //number of singular values thresholded out
 
//compute xo
if nnz1==0 then
   xo(rc,cb)=0;
 
else
   unz=u(:,nd+1:ra);
   vnz=v(:,nd+1:ca);
   s=s(nd+1:ra,nd+1:ca);
   for i=1:nnz1, s(i,i)=s(i,i)/dd(i+nd); end
 
   cross=diag(dd)\(s(nd+1:ra,nd+1:ca))';
 
//cross  now contains the nonsingular part of s / (gs I - s' * s)
 
   xo=-c*vnz*s'*unz'*b;
 
end



