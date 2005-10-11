function [Q,Z,Ec,Ac,Qd,Zd,numbeps]=quaskro(E,A,tol)
// quasi- Kronecker form: s*Ec - Ac = Q*(sE-A)*Z
//
//             | sE(eps)-A(eps) |        X       |      X     |
//             |----------------|----------------|------------|
//             |        O       | sE(inf)-A(inf) |      X     |
//  Q(sE-A)Z = |=================================|============|
//             |                                 |            |
//             |                O                | sE(r)-A(r) |
//
// Ec=Q*E*Z, Ac=Q*A*Z, eps=Qd(1) x Zd(1) ,inf=Qd(2) x Zd(2)
// r = Qd(3) x Zd(3)
// numbeps(1) = # of eps blocks of size 0 x 1
// numbeps(2) = # of eps blocks of size 1 x 2
// numbeps(3) = # of eps blocks of size 2 x 3     etc...
// interface  from Slicot-fstair (F.D.) 
// T. Beelen's routines
//!
// Copyright INRIA
  [LHS,RHS]=argn(0);
  if RHS==1 then [E,A]=pen2ea(E);tol=1.d-10;end
  if RHS==2 then
    if type(E)==2 then [E,A]=pen2ea(E);end  //quaskro(pencil,tol)
    if type(E)==1 then tol=1.d-10;end   //quaskro(E,A);
  end
  [na,ma]=size(A);
  Q=eye(na,na);Z=eye(ma,ma);
  if E~=[] then nE=norm(E,1);else nE=0;end
  [E,Q,Z,stair,rk]=ereduc(E,1000*%eps+tol*nE)
  A=Q*A*Z;

  if A~=[] then
    tol=tol*maxi([norm(A,'fro'),norm(E,'fro')])+10*tol;
  else
    tol=0
  end
  [Ac,Ec,Q,Z,nlbcks,muk,nuk,muk0,nuk0,mnei]=fstair(A,E,Q,Z,stair,rk,tol)
  numbeps=muk0(1:nlbcks)-nuk0(1:nlbcks);
  Qd=[mnei(1),mnei(3),na-mnei(1)-mnei(3)];
  Zd=[mnei(2),mnei(3),ma-mnei(2)-mnei(3)];

endfunction
