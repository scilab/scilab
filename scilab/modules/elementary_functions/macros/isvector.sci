// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r = isvector(x)

// Checks if all dimensions of x but one are 1
// Input: x
// Output: r = boolean value (true if x is a vector)
  rhs = argn(2);
  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"isvector", 1));
  end
  nbones = 0;
  n = size(size(x), "*");
  r = %F;
  for k = 1:n
    if size(x,k) == 1 then
      nbones = nbones + 1;
    elseif size(x, k) == 0 then
     return
    end
  end
  r = (nbones == (n - 1));
endfunction

