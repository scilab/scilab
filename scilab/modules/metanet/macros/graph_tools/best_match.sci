
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [card,match]=best_match(g)
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // check graph
  if g.directed then
    error('The graph must be undirected')
  end
  check_graph(g,%f)
  m=size(g.edges);
  n=node_number(g)
  nd2=n/2;nd8=int(n/8);np1=n+1;nt2=2*n;mt2=2*m;mt3=3*m;mt4=4*m;
  n8t8=ceil(n/8)*8;
  [card,match]=m6bmatch(n,m,nd2,n8t8,nd8,np1,nt2,mt2,mt3,mt4,n,m,g.edges.tail,g.edges.head)
endfunction
