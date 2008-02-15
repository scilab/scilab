
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=graph_sum(g,g1)
  [lhs,rhs]=argn(0)
  if rhs<>2 then error(39), end
  // check g and g1
  check_graph(g)
  check_graph(g1)
  n=node_number(g); 
  n1=node_number(g1);
  if (n1 <> n) then
    error('The two graphs must have the same number of nodes')
  end

  // sum of 2 graphs (sum of adjacency matrices)
  
  Fe=getfield(1,g.edges);
  for f=Fe(2:$)
    g.edges(f)=[g.edges(f) g1.edges(f)];
  end

endfunction
