// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Pierre MARECHAL
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// =============================================================================
// dec2oct function
// =============================================================================

function y = dec2oct(x)

  rhs = argn(2);

  // check the number of input arguments
  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "dec2oct", 1));
  end

  if or(type(x) <> 8) & (or(type(x) <> 1) | or(x < 0) ) then
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Scalar/vector/matrix of positive integers expected.\n"), "dec2oct", 1));
  end

  y = dec2base(double(x), 8);

endfunction
