
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [Gnew,modified]=update_graph(G)
  modified=%f
  if typeof(G)<>'graph' then error('Input argument must be a graph'),end
  if and(getfield(1,G)<>'version') //G is a Scilab4.X graph data structure
   Gnew=update_graphv5(G)
   modified=%t
   return
  end
  if or(G.version==['5.0.0' '5.0.1']) then
    Gnew=G
  else
    error('Graph data structure version: '+G.version+' is not yet handled')
  end
endfunction

function GraphList=update_graphv5(G)
//Author : Serge Steer 2008

  GraphList=ge_new_graph()
  GraphList.directed=G.directed
  GraphList.name=G.name
  //nodes
  n=size(G.node_x,'*')
  GraphList.nodes.number=G.node_number;
  
  //default values
  GraphList.nodes.graphics.defaults.diam=G.default_node_diam;
  GraphList.nodes.graphics.defaults.border=G.default_node_border;
  GraphList.nodes.graphics.defaults.font=[G.default_font_size;6;-1]
  
  //required fields
  GraphList.nodes.graphics.name=matrix(G.node_name,1,-1)
  GraphList.nodes.graphics.x=matrix(G.node_x,1,-1)
  GraphList.nodes.graphics.y=matrix(G.node_y,1,-1)
  GraphList.nodes.graphics.type=matrix(G.node_type,1,-1)
  GraphList.nodes.graphics.diam=matrix(G.node_diam,1,-1)
  GraphList.nodes.graphics.border=matrix(G.node_border,1,-1)
  GraphList.nodes.graphics.colors=[matrix(G.node_color,1,-1);zeros(1,n)]
  GraphList.nodes.graphics.font=[matrix(G.node_font_size,1,-1);zeros(2,n)]

  //optional data fields for nodes
  if or(G.node_demand<>0) then
    GraphList=add_node_data(GraphList,"demand",G.node_demand)
  end
  
  //edges
  n=size(G.tail,'*')
  //default values
  GraphList.edges.graphics.defaults.width=G.default_edge_width
  GraphList.edges.graphics.defaults.font=[G.default_font_size;6;-1]
  
  GraphList.edges.tail=matrix(G.tail,1,-1)
  GraphList.edges.head=matrix(G.head,1,-1)
  GraphList.edges.graphics.width=matrix(G.edge_width,1,-1)
  GraphList.edges.graphics.foreground=matrix(G.edge_color,1,-1)
  GraphList.edges.graphics.font=[matrix(G.edge_font_size,1,-1);zeros(2,n)]
  kloop=find(GraphList.edges.tail==GraphList.edges.head)
  index=0*ones(1,n);index(kloop)=1
  GraphList.edges.graphics.profile_index=index;
  
  //  add missing data fields for edges
  for f=['min_cap','max_cap','q_orig','cost','weight','length']
    if or(G('edge_'+f)<>0) then
      GraphList=add_edge_data(GraphList,f,G('edge_'+f))
    end
  end

endfunction
