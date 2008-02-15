
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [nc,ncomp]=strong_connex(g)
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // g
  check_graph(g,%f)
  // compute lp and ln
  ma=prod(size(g.edges.tail))
  n=node_number(g);
  if g.directed == 1 then
    [lp,la,ln]=m6ta2lpd(g.edges.tail,g.edges.head,n+1,n)
  else
    [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
  end
  // compute connexity
  [nc,ncomp]=m6compfc(lp,ln,n)
endfunction
