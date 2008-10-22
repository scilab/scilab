
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g1=graph_power(g,k)
  [lhs,rhs]=argn(0)
  if rhs<>2 then error(39), end
  // check g
  check_graph(g)
  if g.directed<>1 then
    error('The graph must be directed')
  end
  // check k
  if k<1 then
    error('Power must be greater than 0')
  end
  if k==1 then
    g1=g; return;
  end
  //graph power
  ta=g.edges.tail;he=g.edges.head;
  n=node_number(g); 
  X=sparse([ta' he'],ones(ta)',[n n]);Y=X;Z=X;
  for i=2:k,
    Y=Y*X;
    [ij,v,mn]=spget(Y);
    if (v <> []) then 
      Z=Z+Y;i=k;
    end;
  end;
  [ij,v,mn]=spget(Z);
  ta=[ij(:,1)'];he=[ij(:,2)'];
  g1=make_graph('foo',1,n,ta,he);
  g1.nodes=g.nodes
endfunction
