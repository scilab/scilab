function [X,F]=gcare(Sl)
//[X,F]=gcare(Sl)
//Generalized Control Algebraic Riccati Equation
//X = solution , F = gain
//!
//FD.
// Copyright INRIA
[A,B,C,D]=Sl(2:5);
S=eye()+D'*D;R=eye()+D*D';
Si=inv(S);
Ar=A-B*Si*D'*C;
H=[Ar,-B*Si*B';
   -C'*inv(R)*C,-Ar'];
X=ric_desc(H);
F=-Si*(D'*C+B'*X)
endfunction
