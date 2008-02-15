
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [outdegree,indegree]=nodes_degrees(g)
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // check g
  check_graph(g,%f)

  ta=g.edges.tail;he=g.edges.head;n=node_number(g);np1=n+1;
  i=g.directed
  [lp,la,ln] = adj_lists(i,n,ta,he);
  outdegree=lp(2:np1)-lp(1:n);
  [lp,la,ln] = adj_lists(i,n,he,ta);
  indegree=lp(2:np1)-lp(1:n);
endfunction
