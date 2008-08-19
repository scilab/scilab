
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function e=ge_draw_node(n)

//Construct the graphic object associated with a new node
  prot=funcprot() ;funcprot(0)
  execstr('ge_drawnode=ge_drawnode_'+string(GraphList.nodes.graphics.type(n)))
  funcprot(prot)
  e=ge_drawnode(n)
endfunction

function e=ge_drawnode_0(n)
// Construct initial graphical entity associated with a circular node
// label is placed below the node
// n is the node index
// e is the handle on graphical entity a Compound containing 
//   -  node shape
//   -  node label
  
// Node shape
  node_x=GraphList.nodes.graphics.x(n);
  node_y=GraphList.nodes.graphics.y(n);
  node_diam=ge_get_node_diam(n);
  node_border=ge_get_node_border(n)
  node_colors=ge_get_node_colors(n)

  updateAxisForNode(node_border, node_colors(1), node_colors(2));

  xarc(node_x-node_diam/2,node_y+node_diam/2,node_diam,node_diam,0,360*64);
  e1=gce();

  // Node label
  font=ge_get_node_font(n)
  
  [Id,loc]=ge_get_node_id(n)
  e2=ge_set_node_id(Id,font,loc,node_x,node_y,node_diam,node_diam)


  e=glue([e1,e2])
endfunction


function e=ge_drawnode_1(n)
// Construct initial graphical entity associated with a Sink node
// n is the node index
// e is the handle on graphical entity a Compound containing 
//   -  node shape
//   -  node label

// Node shape
  node_x=GraphList.nodes.graphics.x(n);
  node_y=GraphList.nodes.graphics.y(n);
  node_diam=ge_get_node_diam(n);
  node_border=ge_get_node_border(n)
  node_colors=ge_get_node_colors(n)

  updateAxisForNode(node_border, node_colors(1), node_colors(2));
  
  //circle
  xarc(node_x-node_diam/2,node_y+node_diam/2,node_diam,node_diam,0,360*64);
  e1=gce();
  
  //Big Arrow
  vx=[-1/2;-1/2;-3/4;-1;0;+1;+3/4;+1/2;+1/2;-1/2]
  vy=[1/2;3/2;1;1;5/2;1;1;3/2;1/2;1/2]-3;
  xpoly(node_x+vx*node_diam,node_y+vy*node_diam)
  e=gce();
  //make a Compound for node shape
  e1=glue([e1,e])
  
  // Node label
  font=ge_get_node_font(n)
  [Id,loc]=ge_get_node_id(n)
  e2=ge_set_node_id(Id,font,loc,node_x,node_y,node_diam,node_diam)
  e=glue([e1,e2])
endfunction

function e=ge_drawnode_2(n)
// Construct initial graphical entity associated with a Source node
// n is the node index
// e is the handle on graphical entity a Compound containing 
//   -  node shape
//   -  node label

// Node shape
  node_x=GraphList.nodes.graphics.x(n);
  node_y=GraphList.nodes.graphics.y(n);
  node_diam=ge_get_node_diam(n);
  node_border=ge_get_node_border(n)
  node_colors=ge_get_node_colors(n)

  updateAxisForNode(node_border, node_colors(1), node_colors(2));

  //circle
  xarc(node_x-node_diam/2,node_y+node_diam/2,node_diam,node_diam,0,360*64);
  e1=gce();
  //Big Arrow
  vx=[-1/2;-1/2;-3/4;-1;0;+1;+3/4;+1/2;+1/2;-1/2]
  vy=[1/2;3/2;1;1;5/2;1;1;3/2;1/2;1/2];
  xpoly(node_x+vx*node_diam,node_y+vy*node_diam)
  e=gce();
  //make a Compound for node shape
  e1=glue([e1,e])
  // Node label
  font=ge_get_node_font(n)
  [Id,loc]=ge_get_node_id(n)
  e2=ge_set_node_id(Id,font,loc,node_x,node_y,node_diam,node_diam)

  e=glue([e1,e2])
endfunction

function e=ge_drawnode_3(n)
// Construct initial graphical entity associated with a Square node (not used)
// n is the node index
// e is the handle on graphical entity a Compound containing 
//   -  node shape
//   -  node label

// Node shape
  node_x=GraphList.nodes.graphics.x(n);
  node_y=GraphList.nodes.graphics.y(n);
  node_diam=ge_get_node_diam(n);
  node_border=ge_get_node_border(n)
  node_colors=ge_get_node_colors(n)

  updateAxisForNode(node_border, node_colors(1), node_colors(2));
  
  xrect(node_x-node_diam/2,node_y+node_diam/2,node_diam,node_diam)
  e1=gce();
  
  // Node label
  font=ge_get_node_font(n)
  [Id,loc]=ge_get_node_id(n)
  e2=ge_set_node_id(Id,font,loc,node_x,node_y,node_diam,node_diam)
  
  e=glue([e1,e2])
endfunction

function updateAxisForNode(thickness, foreground, background)

  // modify axes default values if needed
  a = gca();
  if (thickness <> a.thickness) then
     a.thickness = thickness;
  end
  if(foreground <> a.foreground) then
    a.foreground = foreground;
  end
  if(background <> a.background) then
    a.background = background;
  end

endfunction

