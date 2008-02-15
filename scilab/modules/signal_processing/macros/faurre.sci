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
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
