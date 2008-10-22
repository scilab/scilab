
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ns=strong_con_nodes(i,g)
  [lhs,rhs]=argn(0)
  if rhs<>2 then error(39), end
  // check i
  if prod(size(i))<>1 then
    error('First argument must be a scalar')
  end
  // checking of g is made in strong_connex
  [l,nc]=strong_connex(g)
  ns=m6sconcom(i,nc)
endfunction
