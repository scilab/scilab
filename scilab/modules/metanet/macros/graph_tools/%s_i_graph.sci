
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=%s_i_graph(i,d,g)
//called for compatibility with scilab4.x graph-list data structure
  if i=='node_name' then
    g.nodes.graphics.name=d
  elseif i=='node_type' then
    g.nodes.graphics.type=d
  elseif i=='node_x' then
    g.nodes.graphics.x=d
  elseif i=='node_y' then
    g.nodes.graphics.y=d
  elseif i=='node_color' then
    g.nodes.graphics.colors(1,:)=d
  elseif i=='node_diam' then
    g.nodes.graphics.diam=d
  elseif i=='node_border' then
    g.nodes.graphics.border=d
  elseif i=='node_font_size' then
    g.nodes.graphics.font(1,:)=d
  elseif i=='demand' then
    g=add_node_data(g,'demand',d)
  elseif i=='default_node_diam' then
    g.nodes.graphics.defaults.diam=d
  elseif i=='default_node_border' then
    g.nodes.graphics.defaults.border=d
  elseif i=='edge_color' then
    g.edges.graphics.foreground=d
  elseif i=='edge_width' then
    g.edges.graphics.width=d
  elseif i=='edge_hi_width' then
    //no equivalent
  elseif i=='edge_font_size' then
    g.edges.graphics.font(1,:)=d
  elseif i=='edge_cost' then
    g=add_edge_data(g,'cost',d)
  elseif i=='edge_min_cap' then
    g=add_edge_data(g,'min_cap',d)
  elseif i=='edge_max_cap' then
    g=add_edge_data(g,'max_cap',d)
  elseif i=='edge_q_orig' then
    g=add_edge_data(g,'q_orig',d)
  elseif i=='edge_q_weight' then
    g=add_edge_data(g,'q_weight',d)
  elseif i=='edge_weight' then
    g=add_edge_data(g,'weight',d)
  elseif i=='edge_length' then
    g=add_edge_data(g,'length',d)
  elseif i=='default_edge_width' then
    g.edges.graphics.defaults.width=d
  elseif i=='default_edge_hi_width' then
    //no equivalent
  elseif i=='default_font_size' then
    g.edges.graphics.defaults.font(1,:)=d
    g.nodes.graphics.defaults.font(1,:)=d
   
  end
endfunction
