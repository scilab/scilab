
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_new_edge(GraphList,xc1,yc1)

//edition of a link from an output block to an input  block

  kfrom=ge_getnode([xc1;yc1])

  if kfrom==[] then return,end

  xl=GraphList.nodes.graphics.x(kfrom)
  yl=GraphList.nodes.graphics.y(kfrom)

  clr=1
  xe=xl;ye=yl
  ge_win_handle=gcf()
  ge_win_handle.immediate_drawing='off'
  ge_win_handle.pixel_drawing_mode='equiv'
  xpoly([xl;xe],[yl;ye],'lines');e=gce();draw(e)
  while %t do //wait for a node selection
    rep(3)=-1
    while rep(3)==-1 do //get a new point
      rep=xgetmouse([%t %t])
      xe=rep(1);ye=rep(2)
      draw(e)//erase
      e.data(2,:)=rep(1:2); draw(e)//at new position
      show_pixmap()
    end
    if rep(3)==2 then // abort
      draw(e);delete(e);show_pixmap()//erase
      return
    end
    kto=ge_getnode(rep(1:2))
    if kto<>[] then break,end // a destination node selected
  end
  draw(e);delete(e);//erase
  ge_win_handle.pixel_drawing_mode='copy'
  // ge_win_handle.immediate_drawing='on'
  n=size(GraphList.edges)+1
  GraphList.edges.tail(1,n)=kfrom
  GraphList.edges.head(1,n)=kto
  GraphList.edges.graphics.name(1,n)=""
  GraphList.edges.graphics.foreground(1,n)=0;// use default value
  GraphList.edges.graphics.width(1,n)=0// use default value
  GraphList.edges.graphics.font(:,n)=[0;0;0]// use default valus
  Fd=getfield(1,GraphList.edges.data);Fd=Fd(2:$)
  for f=Fd
    GraphList.edges.data(f)(:,n)=0;// use default value
  end
  if kfrom==kto then
    GraphList.edges.graphics.profile_index(1,n)=1
  else
    GraphList.edges.graphics.profile_index(1,n)=0// use default value
  end
  ge_axes_handle=gca();
  gindex=ge_axes_handle.user_data
  hedges=gindex.edge;
  hedges(n)=ge_draw_edge(n)
  gindex.edge=hedges
  ge_axes_handle.user_data=gindex
  ge_win_handle.pixmap='off' //to avoid a full show_pixmap
  draw(hedges(n)); 
  ge_win_handle.pixmap='on'
  draw(hedges(n)); 
  ge_add_history(list('add_arc',n))
  edited=resume(%t)

endfunction

function xyp=ge_arc_profile(isloop)
  if isloop then
    xyp=[1.2 1]
  else
    xyp=ge_default_edge_profile()
    // interpolate it if required
    use_splin=%t
    if use_splin then
      xn=(0:0.1:1)';
      yn = interp(xn, xyp(:,1), xyp(:,2), splin(xyp(:,1), xyp(:,2)) );
      xyp=[xn yn];
    end
  end
endfunction
