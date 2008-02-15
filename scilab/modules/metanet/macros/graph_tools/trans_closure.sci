
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=trans_closure(g)
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // check g and connectivity (checking of g made in is_connex)
  if is_connex(g)<>1 then
    error('The graph must be connected')
  end
  // compute lp, la and ln
  n=node_number(g);
  ma=prod(size(g.edges.tail))
  if g.directed==1 then
    [lp,la,ln]=m6ta2lpd(g.edges.tail,g.edges.head,n+1,n)
  else
    error('The graph must be directed')
  end
  // compute lp1 and ln1 for the transitive closure
  [lp1,ln1]=m6transc(lp,ln,n)
  // compute tail1 and head1 (without checking) for the transitive closure
  n1=prod(size(lp1))-1
  m1=prod(size(ln1))
  la1=1:m1
  [tail1,head1]=m6lp2tad(lp1,la1,ln1,n1)
  // compute transitive closure graph
  nodes=g.nodes;
  g=make_graph(g.name+'_trans_closure',1,node_number(g),tail1,head1)
  g.nodes=nodes

endfunction
