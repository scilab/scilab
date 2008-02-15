
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function n=node_number(g)
//returns the number of nodes in a graph
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  n=size(g.nodes.graphics.x,'*');
endfunction
