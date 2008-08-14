// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function num=numer(r)
//returns the numerator num of a rational function matrix r (r may be
//also a scalar or polynomial matrix
//!
  select typeof(r)
  case 'constant' then
    num=r;
  case 'polynomial' then
    num=r;
  case 'rational' then
    num=r.num
  else
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A floating point number or polynomial or rational fraction array expected.\n"),"numer",1))
  end

endfunction
