
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [nart]=articul(i,g)
  [lhs,rhs]=argn(0)
  n=node_number(g)
  if rhs==1 then
    g=i
    i=1
  elseif rhs==2 then
    // check i
    if (i<1|i>n) then
      error(string(i)+' is not a node number')
    end
  else error(39)
  end
  // check g
  check_graph(g,%f)
  // compute lp, la and ln
  ma=size(g.edges)
  [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
  // compute articulation nodes
  [nart,iart]=m6dfs2(i,lp,ln,n)
  if iart==0|iart==1 then
    nart=[]
  else
    nart=nart(1:iart-1)
  end
endfunction
