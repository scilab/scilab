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

  if argn(2) == 2 then n = 100;end
  y = [d1*ones(1,n-1)+(0:n-2)*(d2-d1)/(n-1),d2];
endfunction
