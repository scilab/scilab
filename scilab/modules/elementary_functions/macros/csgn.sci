// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function s = csgn(z)
//Kahan, W., “Branch cuts for complex elementary functions, or, Much ado about nothing's sign bit”, 
//  Proceedings of the joing IMA/SIAM conference on The State of the Art
//  in Numerical Analysis, University of Birmingham, 
//  A. Iserles & M.J.D. Powell, eds, Clarendon Press, Oxford, 1987, 165-210.

  rhs = argn(2);
  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"csgn", 1));
  end

  s = -ones(z);
  s(real(z)>0|(real(z)==0&imag(z)>0))=1
  s(z==0) = %nan;
endfunction
