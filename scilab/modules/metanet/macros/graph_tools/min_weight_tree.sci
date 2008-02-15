
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function t=min_weight_tree(i,g)
  [lhs,rhs]=argn(0)
  select rhs
   case 1
    if type(i)==16 then
      g=i
      i=1
    else
      error('First argument must be a number or a graph list')
    end
   case 2 then
    if prod(size(i))<>1 then
      error('First argument must be a number')
    end
  else
    error(39)
  end
  // check g
  check_graph(g,%f)
  // compute lp, la and ln
  n=node_number(g);
  ma=prod(size(g.edges.tail))
  if g.directed==1 then
    [lp,la,ln]=m6ta2lpd(g.edges.tail,g.edges.head,n+1,n)
  else
    [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
  end
  // value of weight
  if and(edgedatafields(g)<>"weight")|g.edges.data.weight==[] then
    w=zeros(1,ma);
  else
    w=g.edges.data.weight
  end

  // compute minimal spanning tree
  if g.directed==1 then
    alf=m6dmtree(i,la,lp,ln,n,w)
    t=m6prevn2st(alf,la,lp,ln)
  else 
    if ma<0.5*n*n then 
      alf=m6umtree1(la,lp,ln,n,w)
    else 
      alf=m6umtree(la,lp,ln,n,w)
    end
    t=m6edge2st(alf) 
  end
endfunction
