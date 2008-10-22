
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function set_edges_id(edges,Id,loc)
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
    hedges=gindex.edge
    // next lines derived from ge_draw_edge.sci
    for k=1:size(edges,'*')
      font=ge_get_edge_font(k)
      h=hedges(k).children(1);
      if GraphList.edges.tail(k)==GraphList.edges.head(k) then //loop arc
	[arc,xya,xyl,ang]=ge_profile2real(edges(k))
      else
	[xyc,xya,xyl,ang]=ge_profile2real(edges(k))
      end
      if loc=='tangeant' then 
	r=xstringl(0,0,Id(k),font(2),font(1));r=r(3)
	if ang>90&ang<270 then //co negatif
	  a=ang*%pi/180
	  h.data=[xyl(1)+r*cos(a),xyl(2)-r*sin(a)]
	  h.font_angle=ang+180
	else
	  h.data=[xyl(1),xyl(2)]
	  h.font_angle=ang
	end
      else
	h.data=[xyl(1),xyl(2)]
	h.font_angle=0
      end
      h.text=Id(k)
      h.visible='on'
    end
    fig.immediate_drawing=drawing;
    show_pixmap()
  end
  scf(old)
endfunction
