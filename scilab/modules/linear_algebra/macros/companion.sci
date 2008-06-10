
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function A = companion(p)
// Companion matrix.
// A=compan(p) is a companion matrix with 
// characteristic polynomial equal to (or proportional to) 
// p. If p is a vector of polynomials, the A matrix is block
// diagonal and block number i has characteristic polynomial
// equal to p(i).

  if type(p) ~= 2 
    error(msprintf(gettext('%s: Wrong type for input argument #%d: Polynomial expected.'),'companion',1));
  end
  p=p(:);
  p=p';
  A=[];
  for pp=p;
    c=coeff(pp);
    c=c($:-1:1);
    n = length(c);
    if n <= 1
      B=[];
    elseif n == 2
      B=-c(2)/c(1);
    else
      c=c(:)';
      B=diag(ones(1,n-2),-1);
      B(1,:) = -c(2:n)/c(1);
    end
    A=sysdiag(A,B);
  end
endfunction
