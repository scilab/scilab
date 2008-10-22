
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function set_nodes_id(nodes,Id,loc)
//sets the string entity properties and compute its position
//h : handle on the string entity
  global EGcurrent
  if type(EGcurrent)<>1|size(EGcurrent,'*')<>1 then
    error('No current edit_graph window defined, use netwindow')
  end
  
  win=EGcurrent;w=string(win)
  old=gcf();fig=scf(win)
  execstr(['global EGdata_'+w
	   '  EGdata=EGdata_'+w])
  if typeof(EGdata)=='egdata' then
    drawing=fig.immediate_drawing
    fig.immediate_drawing='off'
    GraphList=EGdata.GraphList
    
    ge_axes_handle=gca();
    gindex=ge_axes_handle.user_data;
    hnodes=gindex.node
     
    node_x=GraphList.nodes.graphics.x(nodes)
    node_y=GraphList.nodes.graphics.y(nodes)
    node_diam=ge_get_node_diam(nodes)
    for k=1:size(nodes,'*')
      h=hnodes(k).children(1)
      r=xstringl(0,0,Id(k),h.font_style, h.font_size)
      // next lines copied from ge_set_node_id (factorize?)
      select loc
      case 'center'
	h.data=[node_x(k)-r(3)/2,node_y(k)-r(4)/2]
      case 'right'
	h.data=[node_x(k)+node_diam(k)/2,node_y(k)-r(4)/2]
      case 'left'
	h.data=[node_x(k)-node_diam(k)/2-r(3),node_y(k)-r(4)/2]
      case 'up' 
	h.data=[node_x(k)-r(3)/2,node_y(k)+node_diam(k)/2+r(4)/2]
      case 'down'
	h.data=[node_x(k)-r(3)/2,node_y(k)-node_diam(k)/2-3*r(4)/2]
      end
      h.text=Id(k)
      h.visible='on'
    end
    fig.immediate_drawing=drawing;
    show_pixmap()
  end
  scf(old)

endfunction
