// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y = linspace(d1, d2, n)
// Linearly spaced vector.
// linspace(x1, x2) generates a row vector of 100 linearly
// equally spaced points between x1 and x2.
// linspace(x1, x2, n) generates n points between x1 and x2.

  rhs = argn(2);
  if rhs < 2 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"linspace", 2));
  end

  if argn(2) == 2 then 
    n = 100;
  end
  if (n - 1) <= 0 then
    y = d2;
  else
    // Note:
    // The following works even if n is a real value
    y = (0:n-1) * (d2-d1) / (n-1) + d1;
  end
endfunction
