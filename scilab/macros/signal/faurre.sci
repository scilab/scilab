function [P,R,T]=faurre(n,H,F,G,R0)
//[P,R,T]=faurre(n,H,F,G,R0)
//macro which computes iteratively the minimal solution of the algebraic
//Riccati equation and gives the matrices Rt and Tt of the filter model.
//   n     : number of iterations.
//   H,F,G : estimated triple from the covariance sequence of y.
//   R0    : E(yk*yk')
//   P    : solution of the Riccati equation after n iterations.
//   R,T  : gain matrix of the filter.
//!
//author: G. Le Vey  Date: 16 Febr. 1989
// Copyright INRIA

//initialization
Pn=G*inv(R0)*G'
//recursion
for k=1:n, 
  A=(G-F*Pn*H');
  Pn=F*Pn*F'+A/(R0-H*Pn*H')*A',
end;
P=Pn
//gain matrices of the filter.
R=R0-H*P*H';
T=(G-F*P*H')/R;
endfunction
