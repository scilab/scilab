
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [cost,nmatch]=perfect_match(g,arcost)
  [lhs,rhs]=argn(0)
  if rhs<>2 then error(39), end
  // check g
  if g.directed==1 then
    error('The graph must be undirected')
  end
  check_graph(g,%f)
  m=prod(size(g.edges.tail));m2=2*m;
  n=node_number(g);
  ii=n-2*round(n/2)
  if (ii <> 0) then
    error('The number of nodes must be even')
  end
  ii=size(arcost,2);
  if (ii <> m2) then
    error('Bad dimension for ""arcost""')
  end
  [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,m2)
  [cost,nmatch]=m6prfmatch(n,m,n+1,m2,ln,arcost,lp);
endfunction
