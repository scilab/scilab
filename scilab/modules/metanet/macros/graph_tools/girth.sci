
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [d]=girth(g)
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // check g
  if ~g.directed  then
    error('The graph must be directed')
  end
  // girth of graph g (length of the shortest cycle)
  [p,r] = circuit(g);
  if p==[] then 
    d=0
  end
  l=size(p,2);
  d=l;
  ta=g.edges.tail;he=g.edges.head;
  n=node_number(g)
  X=sparse([ta' he'],ones(ta)',[n n]);Y=X;
  for i=2:(l-1),
    Y=Y*X;
    if sum(diag(Y))<>0 then
      d=i;
    end
  end
endfunction
