
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_new_node(GraphList,node_x,node_y)
  ge_win_handle=gcf()

  n=size(GraphList.nodes)+1
  GraphList.nodes.graphics.name(1,n)=string(n)
  GraphList.nodes.graphics.type(1,n)=GraphList.nodes.graphics.defaults.type;
  GraphList.nodes.graphics.x(1,n)=node_x
  GraphList.nodes.graphics.y(1,n)=node_y
  GraphList.nodes.graphics.colors(:,n)=[0;0];//request the default values
  GraphList.nodes.graphics.diam(1,n)=0;//request the default values
  GraphList.nodes.graphics.border(1,n)=0;//request the default values
  GraphList.nodes.graphics.font(:,n)=[0;0;0];//request the default values
  GraphList.nodes.number=n;
  
  Fd=getfield(1,GraphList.nodes.data);Fd=Fd(2:$)
  for f=Fd
    GraphList.nodes.data(f)(:,n)=0;// use default value
  end
  ge_axes_handle=gca();
  gindex=ge_axes_handle.user_data
  hnodes=gindex.node;

  hnodes(n)=ge_draw_node(n)
  gindex.node=hnodes;
  ge_axes_handle.user_data=gindex
  
  drawlater() 
  ge_win_handle.pixmap='off' //to avoid a full show_pixmap
  draw(hnodes(n)); 
  ge_win_handle.pixmap='on'
  draw(hnodes(n));
  
  ge_add_history(list('add_node',n))
endfunction
