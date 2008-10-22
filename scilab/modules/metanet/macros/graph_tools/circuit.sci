
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [p,r]=circuit(g)
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // check g
  check_graph(g,%f)
  // compute lp, la and ln
  n=node_number(g)
  ma=prod(size(g.edges.tail))
  if g.directed==1 then
    [lp,la,ln]=m6ta2lpd(g.edges.tail,g.edges.head,n+1,n)
  else
    error('The graph must be directed')
  end
  // compute rank function
  [i,r]=m6frang(lp,ln,n)
  if i==0 then 
    p=[]
  else 
    p=m6prevn2p(i,i,r,la,lp,ln,g.directed), r=[]
  end
endfunction
