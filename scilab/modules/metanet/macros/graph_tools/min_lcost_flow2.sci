
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [c,phi,flag]=min_lcost_flow2(g)
  if argn(2)<1 then error(39), end
  // check g
  check_graph(g,%f)
  // check capacities
  ma=prod(size(g.edges.tail))
  n=node_number(g);
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
  if find(mincap<>0)<>[] then 
    error('Minimum capacities must be equal to zero')
  end
  if find(maxcap<0) <>[] then 
    error('Maximum capacities must be non negative')
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
  if or(cost<>round(cost)) then
    error('Costs must be integer')
  end
  // check demand
  if and(nodedatafields(g)<>"demand")|g.nodes.data.demand==[] then
    demand=zeros(1,n)
  else
    demand=g.nodes.data.demand
  end
  if or(demand<>round(demand)) then
    error('Demands must be integer')
  end
  if sum(demand)<>0 then
    error('Sum of demands must be equal to zero')
  end
  // compute linear min cost flow by relaxation method (Bertsekas)
  [c,phi,flag]=m6relax(g.edges.head,g.edges.tail,cost,maxcap,demand,arc_number(g),n)
endfunction
