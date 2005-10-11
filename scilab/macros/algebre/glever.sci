function [Bfs,Bis,chis]=glever(E,A,s)
// [Bfs,Bis,chis]=glever(E,A,'s')
// Computation of (s*E-A)^-1 ('s'=character string with default value 's')
// Generelized Leverrier's algorithm for a matrix pencil ;
// (s*E-A)^-1 = (Bfs/chis) - Bis
// chis = characteristic polynomial (up to a multiplicative constant)
// Bfs  = polynomial matrix
// Bis  = polynomial matrix ( - expansion of (s*E-A)^-1 at infinity).
// Caveat: uses clean to simplify Bfs,Bis and chis !
// See also shuffle, determ, invr, coffg
// F.D. (1988)
//!
// Copyright INRIA
  [LHS,RHS]=argn(0);
  if RHS==1 then [E,A]=pen2ea(E),s=poly(0,'s');end 
  if RHS==2 then s=poly(0,'s'),end;
  if RHS==3 then s=poly(0,s);end
  [Si,Pi,Di,index]=penlaur(E,A);
  k=round(sum(diag(Si*E)));

  a0=1;
  B=Si;
  SiASi=Si*A*Si;
  chis=a0+0*s;
  Bfs=Si+0*s*Si;
  
  for i=1:k,
    B=SiASi*E*B;
    alfa=-sum(diag(E*B))/i;
    B=B+alfa*Si;
    chis=s*chis+alfa;
    if i<k then Bfs=s*Bfs+B;end
  end
  Bis=Pi;
  AAD=s*A*Di;
  P=eye(A);
  
  for nu=1:index+1,
    P=AAD*P;
    Bis=clean(Bis+Pi*P,1.d-10);
  end
  Bfs=clean(Bfs,1.d-10);
  chis=clean(chis,1.d-10);
endfunction
