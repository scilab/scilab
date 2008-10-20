function [R,Q]=pdiv(P1,P2)
// Element wise euclidan division of a polynomial matrix 
// by a polynomial
// This is just a loop for the primitive pppdiv
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
[n,m]=size(P1);
[n1,m1]=size(P2);
// Special case for constant matrices
if type(P1)==1&type(P2)==1 then
  Q=P1./P2;R=0*P1;
  if lhs==1 then R=Q; end
  return;
end
R=[],Q=[]
if n1==1 & m1==1 then
 for l=1:n,
    for k=1:m,
      [rlk,qlk]=pppdiv(P1(l,k),P2),R(l,k)=rlk;Q(l,k)=qlk;
    end;
 end
 if lhs==1 then R=Q;end

 return;
end
for l=1:n,
    for k=1:m,
      [rlk,qlk]=pppdiv(P1(l,k),P2(l,k)),R(l,k)=rlk;Q(l,k)=qlk;
    end;
end
if lhs==1 then R=Q; end
endfunction
