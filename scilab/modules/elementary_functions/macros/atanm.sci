// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=atanm(a)
//   atanm - computes the matrix arctangent
//%CALLING SEQUENCE
//   x=atanm(a)
//%PARAMETERS
//   a   : square  matrix
//   x   : square  matrix

  rhs = argn(2);

  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"atanm",1));
  end

  if type(a)<>1 then
   error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"atanm",1));
  end

  if a==[] then x=[],return,end

  [m,n]=size(a);
  if m<>n then
   error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"atanm",1));
  end

  //diagonalization
  [x,t,bs]=bdiag(a+0*%i*ones(a),1/%eps);

  if find(bs>1)<>[] then
    error(msprintf(gettext("%s: Matrix is not diagonalisable.\n"),"atanm"));
  end

  x=t*diag(atan(diag(x)))/t;

  if and(imag(a)==0) then x=real(x),end

endfunction
