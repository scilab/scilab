
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [a]=predecessors(i,g)
  [lhs,rhs]=argn(0)
  if rhs<>2 then error(39), end
  // check g
  check_graph(g,%f)
  if g.directed<>1 then
    error('The graph must be directed')
  end
  n=node_number(g);
  // check i
  if (i<1|i>n) then
    error(string(i)+' is not a node number')
  end
  ta=g.edges.tail;he=g.edges.head;
  [ir,ic]=find(he==i);
  a=ta(ic);
endfunction
