
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=supernode(v,g)
  [lhs,rhs]=argn(0)
  if rhs<>2 then error(39), end
  // check v
  s=size(v)
  if s(1)<>1 then
    error('First argument must be a row vector')
  end
  // check g
  check_graph(g)
  //set of nodes v replaced by one

  v=unique(v)
  if size(v,'*')<2 then return,end
  
  n=node_number(g);
  if (v(1)<1)|(v($)>n) then
    error('A number in first argument is not a node number')
  end
  if size(v,'*')<2 then return,end
  if size(v,'*')==n then
    error('The graph must not be reduced to one node') 
  end

  ta=g.edges.tail;
  he=g.edges.head;
  //find edges connected to an "internal" node
  indta=dsearch(ta,v,'d')
  indhe=dsearch(he,v,'d')
		  
  v1=v(1);v=v(2:$);
  nnn=ones(1,n);nnn(v)=0;nnn=cumsum(nnn);nnn(v)=v1;// new node numbers
  //renumber edges bounds
  ind=1:n;
  ta=nnn(ind(dsearch(ta,ind,'d')));
  he=nnn(ind(dsearch(he,ind,'d')));

  g.edges.tail=ta;
  g.edges.head=he;
  
  //remove internal arcs
  g.edges(find(indta>0&indhe>0))=[]
  //remove nodes
  g.nodes(v)=[]


endfunction
