
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_do_options(GraphList)
  df=getfield(1,GraphList.nodes.data)
  NodeDataFields=['nothing','number','name', df(2:$)];
  kn=find( NodeDataFields==GraphList.nodes.graphics.display)
  if kn==[] then kn=1,end
  node=list('Nodes',kn,NodeDataFields);

  ef=getfield(1, GraphList.edges.data);
  EdgeDataFields=['nothing','number','name',ef(2:$)];
  ka=find( EdgeDataFields== GraphList.edges.graphics.display)
  if ka==[] then ka=1,end
  
  arc=list('Arcs',ka,EdgeDataFields)
  rep=x_choices("Select information to display",list(node,arc))
  if rep<>[] then
    GraphList.nodes.graphics.display=NodeDataFields(rep(1))
    GraphList.edges.graphics.display=EdgeDataFields(rep(2))
  end
endfunction
