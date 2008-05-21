
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function v=%graph_e(i,g)
//called for compatibility with scilab4.x graph-list data structure

  if i=='node_number' then
    v=node_number(g)
  elseif i=='node_name' then
    v=g.nodes.graphics.name
  elseif i=='node_type' then
    v=g.nodes.graphics.type
  elseif i=='node_x' then
    v=g.nodes.graphics.x
  elseif i=='node_y' then
    v=g.nodes.graphics.y
  elseif i=='node_color' then
    v=g.nodes.graphics.colors(1,:)
  elseif i=='node_diam' then
    v=g.nodes.graphics.diam
  elseif i=='node_border' then
    v=g.nodes.graphics.border
  elseif i=='node_font_size' then
    v=g.nodes.graphics.font(1,:)
  elseif i=='node_demand' then
    if or(nodedatafields(g)=='demand') then v=g.nodes.data.demand;else v=[];end
  elseif i=='node_label' then
    if or(nodedatafields(g)=='label') then v=g.nodes.data.label; else v=[];end
  elseif i=='default_node_diam' then
    v=g.nodes.graphics.defaults.diam
  elseif i=='default_node_border' then
    v=g.nodes.graphics.defaults.border
  elseif i=='tail' then
    v=g.edges.tail
  elseif i=='head' then
    v=g.edges.head
  elseif i=='edge_name' then
    v=g.edges.graphics.name
  elseif i=='edge_color' then
    v=g.edges.graphics.foreground
  elseif i=='edge_width' then
    v=g.edges.graphics.width
  elseif i=='edge_hi_width' then
    v=3*g.edges.graphics.width
  elseif i=='edge_font_size' then
    v=g.edges.graphics.font(1,:)
  elseif i=='edge_cost' then
    if or(edgedatafields(g)=='cost') then v=g.edges.data.cost;else v=[];end 
  elseif i=='edge_cost' then
    if or(edgedatafields(g)=='cost') then v=g.edges.data.cost; else v=[];end 
  elseif i=='edge_min_cap' then
    if or(edgedatafields(g)=='min_cap') then v=g.edges.data.min_cap;else v=[];end 
  elseif i=='edge_max_cap' then
    if or(edgedatafields(g)=='max_cap') then v=g.edges.data.max_cap;else v=[];end 
  elseif i=='edge_q_weight' then
    if or(edgedatafields(g)=='q_weight') then v=g.edges.data.q_weight;else v=[]; end 
  elseif i=='edge_q_orig' then
    if or(edgedatafields(g)=='q_orig') then v=g.edges.data.q_orig;else v=[];end 
  elseif i=='edge_weight' then
    if or(edgedatafields(g)=='weight') then v=g.edges.data.weight;else v=[];end 
  elseif i=='edge_label' then
    if or(edgedatafields(g)=='label') then v=g.edges.data.label;else v=[];end 
  elseif i=='default_edge_width' then
    v=g.edges.graphics.defaults.width
  elseif i=='default_edge_hi_width' then
    v=3*g.edges.graphics.defaults.width
  elseif i=='default_font_size' then
    v=g.edges.graphics.defaults.font(1,:)
  else
    v=[];
  end
endfunction
