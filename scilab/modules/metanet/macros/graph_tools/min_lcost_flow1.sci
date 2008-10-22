
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [c,phi,flag]=min_lcost_flow1(g)
  if argn(2)<>1 then error(39), end
  // check g
  check_graph(g)
  // check capacities
  ma=size(g.edges.tail,'*')
  n=node_number(g)
  Fed=edgedatafields(g)
  if and(Fed<>"max_cap")|g.edges.data.max_cap==[] then
    maxcap=zeros(1,ma);
  else
    maxcap=g.edges.data.max_cap
  end
  if and(Fed<>"min_cap")|g.edges.data.min_cap==[] then
    mincap=zeros(1,ma);
  else
    mincap=g.edges.data.min_cap
  end
  if find(mincap<0)<>[] then 
    error('Minimum capacities must be non negative')
  end
  if find(mincap>maxcap)<>[] then
    error('Maximum capacities must be greater than minimal capacities')
  end
  if or(mincap<>round(mincap)) then
    error('Minimum capacities must be integer')
  end
  if or(maxcap<>round(maxcap)) then
    error('Maximum capacities must be integer')
  end
  // check costs
   if and(Fed<>"cost")|g.edges.data.cost==[] then
    cost=zeros(1,ma);
  else
    cost=g.edges.data.cost
  end

  if or(cost<0) then 
    error('Costs must be non negative')
  end

  // compute lp, la and ln
  // always consider the graph as undirected!
  [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
  // compute linear min cost flow by out-of-kilter algorithm
  [phi,flag]=m6kilter(mincap,maxcap,g.edges.head,g.edges.tail,la,lp,n,cost)
  c=cost*phi'
endfunction
