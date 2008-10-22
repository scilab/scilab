
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [p,cap]=max_cap_path(i,j,g)
  if argn(2)<>3 then error(39), end
  // check i and j
  if size(i,'*')<>1 then
    error('First argument must be a scalar')
  end
  if size(j,'*')<>1 then
    error('Second argument must be a scalar')
  end
  // check g
  check_graph(g)
  // compute lp, la and ln
  n=node_number(g)
  ma=size(g.edges.tail,'*')
  if g.directed==1 then
    [lp,la,ln]=m6ta2lpd(g.edges.tail,g.edges.head,n+1,n)
  else
    [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
  end
  // check max capacity
  if and(edgedatafields(g)<>'max_cap')|g.edges.data.max_cap==[] then
    cap=0
    p=[]
    return
  end
  // compute max capacity path
  [c,v]=m6chcm(i,la,lp,ln,n,g.edges.data.max_cap)
  p=m6prevn2p(i,j,v,la,lp,ln,g.directed)
  cap=c(j)
endfunction
