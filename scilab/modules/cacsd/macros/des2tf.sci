function [Bfs,Bis,tf]=des2tf(des)
//des admits a D matrix.
// Copyright INRIA
[LHS,RHS]=argn(0);
if LHS<>1 & LHS<> 3 then error('des2tf: 1 or 3 output args needed');end
[A,B,C,D,E]=des(2:6);
[Bfs,Bis,chis]=glever(E,A);
if LHS==3 then Bfs=C*Bfs*B; Bis=C*Bis*B+D;tf=chis;return;end
if LHS==1 then ww=C*Bfs*B;Bfs=ww/chis-C*Bis*B+D;return;end
endfunction
