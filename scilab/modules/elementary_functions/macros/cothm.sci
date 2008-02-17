// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=cothm(a)
// hyperbolic co-tangent of square matrix 

  if type(a)<>1 then error(53,1),end
  if a==[] then x=[],return,end
  [m,n]=size(a)
  if m<>n then error(20),end
  //diagonalization
  [x,t,bs]=bdiag(a+0*%i*ones(a),1/%eps)
  if find(bs>1)<>[] then
    error(msprintf(gettext("%s: Matrix is not diagonalisable.\n"),"cothm"));
  end
  x=t*diag(coth(diag(x)))/t
  if and(imag(a)==0) then x=real(x),end
endfunction
