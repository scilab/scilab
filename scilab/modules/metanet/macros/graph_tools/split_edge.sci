
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g1=split_edge(i,j,g,name)
  [lhs,rhs]=argn(0)
  // check name
  if rhs==3 then
    name=0
  else 
    if rhs<>4 then error(39), end
    if type(name)<>10|prod(size(name))<>1 then
      error('Fourth argument must be a string')
    end
  end
  check_graph(g,%t)
  // split the edge from node i to node j by 
  // adding a new node between i and j
  ta=g.edges.tail;he=g.edges.head;
  ii=find((ta==i)&(he==j));
  if ii==[] then
    error('There is no edge from node i to node j')
  end
  if size(ii,2)>1 then
    error('There are more than one edge from node i to node j')
  end
  g1=g;
  n=node_number(g);
  xnodes=g1.nodes.graphics.x;ynodes=g1.nodes.graphics.y;
  xx=0.5*(xnodes(i)+xnodes(j));yy=0.5*(ynodes(i)+ynodes(j));
  if name==0 then 
    g1=add_node(g1,[xx yy]);
  else 
    g1=add_node(g1,[xx yy],name);
  end;
  g1=add_edge(i,n+1,g1);
  g1=add_edge(n+1,j,g1);
  g1=delete_arcs([i j],g1);
endfunction
