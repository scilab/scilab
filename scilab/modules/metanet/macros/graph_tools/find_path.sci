
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function p=find_path(i,j,g)
  [lhs,rhs]=argn(0)
  if rhs<>3 then error(39), end
  // check i and j
  if size(i,'*')<>1 then
    error('First argument must be a scalar')
  end
  if size(j,'*')<>1 then
    error('Second argument must be a scalar')
  end
  // check g
  check_graph(g,%f)
  // compute lp, la and ln
  n=node_number(g);
  ma=edge_number(g)
  if g.directed then
    [lp,la,ln]=m6ta2lpd(g.edges.tail,g.edges.head,n+1,n)
  else
    [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
  end
  // compute path
  [l,v]=m6dfs(i,lp,ln,n)
  p=m6prevn2p(i,j,v,la,lp,ln,g.directed)
endfunction
