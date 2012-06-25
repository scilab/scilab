// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS

// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=cosm(a)
//   cosm - computes the matrix cosine 
//%CALLING SEQUENCE
//   x=cosm(a)
//%PARAMETERS
//   a   : square hermitian or diagonalizable matrix
//   x   : square hermitian matrix

  rhs = argn(2);

  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"cosm",1));
  end

  if type(a)<>1 then
   error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"cosm",1));
  end

  [m,n]=size(a);
  if m<>n then
   error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"cosm",1));
  end

  if a==[] then x=[],return,end

  if norm(imag(a),1)==0 then
    x=real(expm(%i*a))
  else
    x=0.5*(expm(%i*a)+expm(-%i*a));
  end

endfunction
