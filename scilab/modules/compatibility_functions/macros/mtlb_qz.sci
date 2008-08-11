// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [AA, BB, Q, Z, V]=mtlb_qz(A,B)
[lhs,rhs]=argn(0)
[AA, BB, Q, Z]=gschur(A,B)
if lhs==5 then
  n=size(A,1)
  LA=diag(AA)
  LB=diag(BB)
  V=zeros(n,n)
  for k=1:n
    K=kernel(A*LB(k)-B*LA(k))
    if size(K,2)<>1 then
      error(msprintf(gettext("%s: pencil is not diagonalizable.\n"),"mtlb_qz"))
    end
    V(:,k)=K
  end
end
endfunction
