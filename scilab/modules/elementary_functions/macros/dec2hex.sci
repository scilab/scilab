// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// =============================================================================
//
// dec2hex(d) returns the hexadecimal representation corresponding to the
// numbers in vector v.
//
// =============================================================================

function h = dec2hex(v)

  rhs = argn(2);

  // check the number of input arguments
  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"dec2hex",1));
  end

  [nr, nc] = size(v);

  if (find(v<0) <> []) | (norm(round(v)-v,1) > norm(round(v),1)*2*%eps) then
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Non-negative integer matrix expected.\n"),"dec2hex",1));
  end

  h = [];
  for ii = 1:size(v,'*')
    h(ii) = msprintf("%X", v(ii));
  end
  h = matrix(h, nr, nc);

endfunction
