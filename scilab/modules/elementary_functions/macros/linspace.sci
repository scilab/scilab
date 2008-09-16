// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
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

  if argn(2) == 2 then 
    n = 100;
  end
  if n<=1 then
    error(msprintf(gettext("Unexpected value of n : %s. Expected value is greater than 1.\n"),string(n)));
  end
  // If n is a real, using n directly may lead to approximation
  // problems. Instead, add 1/2 and round so that the real is 
  // correctly approximated into an integer.
  // If n is allready an integer, this is a no-op.
  appn = floor(n+0.5)
  alpha= (d2-d1)/(appn-1)
  y1 = d1*ones(1,appn-1)+(0:appn-2)*alpha
  y = [y1,d2];
endfunction

