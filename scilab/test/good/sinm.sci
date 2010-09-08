// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=sinm(a)
//   sinm - computes the matrix sine 
//%CALLING SEQUENCE
//   x=sinm(a)
//%PARAMETERS
//   a   : square  matrix
//   x   : square  matrix

  rhs = argn(2);

  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"sinm",1));
  end

  if type(a)<>1 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"sinm",1));
  end

  if a==[] then x=[],return,end

  if norm(imag(a),1)==0 then
    x=imag(expm(%i*a));
  else
    x=-0.5*%i*(expm(%i*a)-expm(-%i*a));
  end

endfunction
