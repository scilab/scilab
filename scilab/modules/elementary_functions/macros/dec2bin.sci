// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) DIGITEO - 2011 - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// =============================================================================
//
// Author : F.Belahcene
// DEC2BIN function
//
// Given x, a positive scalar/vector/matix of reals, this function returns
// a column vector of strings. Each string is the binary representation
// of the input argument components (i.e y(i) is the binary representation
// of x(i))
//
// -Inputs :
//    x : a  scalar/vector/matix of positives reals
//    n : an integer
// -Output :
//    y : a vector of strings (positives)
//
// =============================================================================

function y = dec2bin(x, n)
  rhs = argn(2);

  // check the number of input arguments
  if (rhs < 1 | rhs > 2) then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"), "dec2bin", 1, 2));
  end

  if rhs == 2 then
    y = dec2base(double(x), 2, n);
  else
    y = dec2base(double(x), 2);
  end
endfunction

