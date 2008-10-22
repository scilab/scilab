
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [x,pi]=pipe_network(g)
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end 
  // g
  check_graph(g,%f)
  if g.directed <> 1 then
    error('The graph must be directed')
  end
  n=node_number(g);
  ma=edge_number(g);

  // check weights
  if and(edgedatafields(g)<>"weight")|g.edges.data.weight==[] then
    r=ones(1,ma)
  else
    r=g.edges.data.weight
  end

  if max(r)<=0 then
    error('Weights (resistances) must be strictly positive')
  end
  // check demands
  Fnd=nodedatafields(g)
  if and(Fnd<>"demand")|g.nodes.data.demand==[] then
    demand=zeros(1,n)
  else
    demand=g.nodes.data.demand
  end

  if sum(demand)<>0 then
    error('The problem is not feasible')
  end

  a=graph_2_mat(g);
  rp=sparse([[1:ma]' [1:ma]'],1 ./ r);
  ap=a(1:$-1,:);
  A=ap*rp*ap';
  B=demand(1:$-1)';
  [h,rk]=lufact(A);
  pi=lusolve(h,B); pi(n)=0;
  ludel(h);
  x=-rp*a'*pi;
endfunction
