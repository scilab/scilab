
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_do_salesman(GraphList)
//Author : Serge Steer 2008
  if GraphList.directed then
    cir = salesman(GraphList);
  else
    ta=GraphList.edges.tail;
    he=GraphList.edges.head;
    g1=make_graph('foo1',1,node_number(GraphList),[ta he],[he ta]);
    cir = salesman(g1)
    ii=find(cir > edge_number(GraphList)); 
    if(ii <> []) then cir(ii)=cir(ii)-edge_number(GraphList);end;
  end
  show_arcs(cir,'sup');
endfunction

