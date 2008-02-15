
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [v,phi,ierr]=max_flow(i,j,g)

  if argn(2)<>3 then error(39), end
  // check i and j
  if size(i,'*')<>1 then
    error('First argument must be a scalar')
  end
  if size(j,'*')<>1 then
    error('Second argument must be a scalar')
  end
  if (i == j) then
    error('Source and sink must not be the same node')
  end
  // check g
  check_graph(g)
  if g.directed <> 1 then
    error('The graph must be directed')
  end
  ma=size(g.edges.tail,'*')
  n=node_number(g);
  if ( (i > n) | (i < 0) ) then
    error('i is not a node number')
  end
  if ( (j > n) | (j < 0) ) then
    error('i is not a node number')
  end
  // check capacities
  Fed=getfield(1,g.edges.data)
  if and(Fed<>"max_cap")|g.edges.data.max_cap==[] then
    maxcap=zeros(1,ma);
  else
    maxcap=g.edges.data.max_cap
  end
  // compute maximum flow
  [v,phi,ierr]=m6fordfulk(n,ma,i,j,maxcap,g.edges.tail,g.edges.head)
endfunction
