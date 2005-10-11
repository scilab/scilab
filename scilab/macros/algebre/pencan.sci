function [Q,M,i1]=pencan(E,A)
// [Q,M,i1]=pencan(E,A)
// Given the pencil s*E-A pencan returns matrices Q and M
// such than M*(s*E-A)*Q is in "canonical" form i.e.
// M*E*Q is a block matrix [I,0;
//                         0,N]    with N nilpotent
// and i1 = size of the I matrix above (# of finite zeros of (sE-A)).
// M*A*Q is a block matrix [Ar,0;
//                         0,I ]
//See glever,  penlaur
//F.D.
//!
// Copyright INRIA
  [LHS,RHS]=argn(0);
  if RHS==1 then [E,A]=pen2ea(E);end
  [Si,Pi,Di,index]=penlaur(E,A);
  [Q1,M1]=fullrf(Si);
  [Q2,M2]=fullrf(Pi);
  [i1,i2]=size(M1);
  M=[M1;M2];
  Q=[Q1,Q2];
endfunction
