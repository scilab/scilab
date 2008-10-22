
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=delete_nodes(del_nodes,g)
  if argn(2)<>2 then error(39), end

  if del_nodes==[] then  return; end;
  del_nodes=unique(del_nodes)
  if del_nodes(1)<1|del_nodes($)>node_number(g) then
    error('A number in first argument is not a node number')
  end
  
  //connected arcs
  del_arcs=find(dsearch(g.edges.tail,del_nodes,'d')>0|..
		dsearch(g.edges.head,del_nodes,'d')>0)
  g=ge_delete_node(g,del_nodes)
  g.edges(del_arcs)=[]
endfunction
