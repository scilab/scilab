// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function W = pertrans(A)
  // W = pertranspose(A)

  [lhs, rhs] = argn(0);
  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "pertrans", 1));
  end

  [n, m] = size(A);
  A = A';
  W = A(m:-1:1,n:-1:1);
endfunction
