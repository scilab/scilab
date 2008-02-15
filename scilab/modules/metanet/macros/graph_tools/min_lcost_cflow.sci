
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [c,phi,v,flag]=min_lcost_cflow(i,j,cv,g)
  if argn(2)<>4 then error(39), end
  // check i and j
  if size(i,'*')<>1 then error('First argument must be a scalar'),end
  if size(j,'*')<>1 then error('Second argument must be a scalar'),end
  // check cv
  if size(cv,'*')<>1 then error('Third argument (constrained cost) must be a scalar'),end
  if cv<0 then error('Constrained cost must be positive');end

  // check g
  check_graph(g,%f)
  // check capacities
  ma=edge_number(g)
  n=node_number(g);
  
  if or(edgedatafields(g)=='max_cap') then
    maxcap=g.edges.data.max_cap
  else
    maxcap=[]
  end
  
  if maxcap==[] then
    maxcap=zeros(1,ma)
  elseif find(maxcap<0)<>[] then 
    error('Maximum capacities must be non negative')
  elseif or(maxcap<>round(maxcap)) then
    error('Maximum capacities must be integer')
  end
  
  // check costs
  if or(edgedatafields(g)=='cost') then
    costs=g.edges.data.cost
    if costs==[] then 
      costs=zeros(1,ma);
    elseif find(costs<0)<>[] then 
      error('Costs must be non negative')
    end
  else
    costs=zeros(1,ma);
  end
 
  // compute lp, la and ln
  if g.directed==1 then
    [lp,la,ln]=m6ta2lpd(g.edges.tail,g.edges.head,n+1,n)
  else
    [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
  end
  // compute constrained min cost flow by Busacker and Goven algorithm
  [v,phi,flag]=m6busack(i,j,cv,maxcap,g.edges.head,g.edges.tail,la,lp,n,costs)
  c=costs*phi'
endfunction
