
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=graph_union(g,g1,opt)
// union of 2 graphs

if argn(2)<2 then error(39), end
if argn(2)<3 then opt=%t,end
//check g and g1
check_graph(g);
check_graph(g1);
no=g.nodes.graphics.name;no1=g1.nodes.graphics.name;

if opt & no<>[]&no1<>[] then //check for nodes with equal names in g and g1
  n=node_number(g); 
  n1=node_number(g1);
  ic1=[];index=[];
  //intersection computation should be improved.
  for i=1:n1,
    [ir,ic]=find(no==no1(i));
    if (ic<>[]) then index=[index i]; end;
    ic1=[ic1 ic];
  end
  uu=1:n1;uu(index)=[];
  w=1:n1;w(uu)=n+(1:size(uu,'*'));
  w(index)=ic1;
  //
  g.nodes($+1:$+size(ic1,'*'))=g1.nodes(ic1)

  m1=edge_number(g1)
  m=edge_number(g)
  g.edges($+1:$+m1)=g1.edges
  g.edges.tail(m+1:$)=w(g1.edges.tail);
  g.edges.head(m+1:$)=w(g1.edges.head);
else
  n=node_number(g); 
  g.nodes($+1:$+node_number(g1))=g1.nodes
  g1.edges.tail=g1.edges.tail+n
  g1.edges.head=g1.edges.head+n
  g.edges($+1:$+edge_number(g1))=g1.edges
end

endfunction
