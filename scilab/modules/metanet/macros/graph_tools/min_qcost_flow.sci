
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [c,phi,flag]=min_qcost_flow(eps,g)
  [lhs,rhs]=argn(0)
  if rhs<>2 then error(39), end
  // check eps
  if prod(size(eps))<>1 then
    error('First argument must be a scalar')
  end
  if eps<=0 then
    error('First argument must be strictly positive')  
  end
  p=-log(eps)/log(2)
  // check g
  check_graph(g)
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

  verif=find(mincap>maxcap)
  if verif<>[] then
    error('Maximum capacities must be greater than minimal capacities')
  end
  verif=find(mincap<>maxcap)
  if verif==[] then
    error('Maximum capacities must not be all equal to minimal capacities')
  end
  // check quadratic costs
  if and(Fed<>"q_orig")|g.edges.data.q_orig==[] then
    qorig=zeros(1,ma);
  else
    qorig=g.edges.data.q_orig;
  end
  
  if and(Fed<>"q_weight")|g.edges.data.q_weight==[] then
    qweight=zeros(1,ma);
  else
    qweight=g.edges.data.q_weight;
  end

  // check demand
  if and(nodedatafields(g)<>"demand")|g.nodes.data.demand==[] then
    demand=zeros(1,n)
  else
    demand=g.nodes.data.demand
  end

  verif=find(demand<>0)
  if verif<>[] then 
    error('Demands must be equal to zero')
  end
  // compute lp, la and ln
  // always consider the graph as undirected!
  [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
  // compute min quadratic cost flow
  [phi,flag]=m6floqua(p,mincap,maxcap,g.edges.head,g.edges.tail,la,lp,n,qorig,qweight)
  c=sum(qweight.*(phi-qorig).*(phi-qorig))/2
endfunction
