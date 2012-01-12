// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Farid BELAHCENE
// Copyright (C) 2006 - INRIA - Pierre MARECHAL
// Copyright (C) 2011 - DIGITEO - Pierre MARECHAL
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// =============================================================================
//
// BIN2dec function
// Given str a binary string, this function returns the decimal number whose the
// binary representation is given by str
//
// -Input :
//    str : a string (or a vector/matrix of strings), containing only characters
//         '1' and '0'
// -Output :
//    y : a scalar/vector/matrix
//
// F.Belahcene

// check the type of input argument

// 2006-06-26 : Modified by Pierre MARECHAL
// Check length of given string ( must be 47 bits or less )
// =============================================================================

function y = bin2dec(str)

  rhs = argn(2);
  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"bin2dec", 1));
  end

  if type(str)<>10
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),"bin2dec",1));
  end

  // delete all spaces included in the str
  str = strsubst(str,' ','');
  y = base2dec(str, 2);

endfunction
