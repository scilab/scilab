
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ns=path_2_nodes(p,g)
  [lhs,rhs]=argn(0)
  if rhs<>2 then error(39), end
  // check p
  s=size(p)
  if s(1)*s(2) == 0 then ns=[]; return end
    if s(1)<>1 then
      error('First argument must be a row vector')
    end
    // check g
    check_graph(g,%f)
    // compute lp, la and ln
    ma=prod(size(g.edges.tail))
    n=node_number(g);
    if g.directed == 1 then
      [lp,la,ln]=m6ta2lpd(g.edges.tail,g.edges.head,n+1,n)
    else
      [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
    end
    // compute node set
    ns=m6p2ns(p,la,lp,ln,g.directed,n)
endfunction
