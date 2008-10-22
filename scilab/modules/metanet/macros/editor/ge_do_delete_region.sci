
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_do_delete_region(GraphList,xc,yc)
// EGData is passed by context
  xinfo('Click, drag to select region and click to fix the selection')

  ge_disablemenus()
  drawnow()

  [rect,btn]=rubberbox([xc,yc])
  if btn==2 then  ge_enablemenus();return;end
  [del_nodes,keep]=ge_get_nodes_in_rect(GraphList,rect)

  edited=del_nodes<>[]

  if edited then
    ge_axes_handle=gca();n=size(ge_axes_handle.children,'*')
    gindex=ge_axes_handle.user_data;
    hedges=gindex.edge;hnodes=gindex.node;

      
    drawlater()
    del_nodes=gsort(del_nodes,'g','i')

    del_arcs=find(dsearch(GraphList.edges.tail,del_nodes,'d')>0|..
		  dsearch(GraphList.edges.head,del_nodes,'d')>0)
    if del_arcs<>[] then
      del_arcs=gsort(del_arcs,'g','i')
      Hista=list("delete_edges",del_arcs)
      Hista($+1)=GraphList.edges(del_arcs)
    else
      Hista=list()
    end
    
    Histn=list("delete_nodes",del_nodes)
    Histn($+1)=GraphList.nodes(del_nodes)
    if size(Hista)==0 then
      ge_add_history(Histn)
    else
      ge_add_history(list("compound",Histn,Hista))
    end
    
    //destroy the graphic entities associated with deleted arcs
    if del_arcs<>[] then
      delete(hedges(del_arcs));hedges(del_arcs)=[];
      //renumbers the arcs with index higher than first deleted arc
      for i=del_arcs(1):size(hedges,'*')
	ak=hedges(i);
	ak.children(1).text=string(i);
      end
      GraphList.edges(del_arcs)=[];
    end

    //delete the graphics entities associated with deleted nodes
    if del_nodes<>[] then
      delete(hnodes(del_nodes));hnodes(del_nodes)=[];
      // renumber all the nodes
      for i=del_nodes(1):size(hnodes,'*')
	ak=hnodes(i);
	ak.children(1).text=string(i);
      end
      GraphList=ge_delete_node(GraphList,del_nodes)
    end
    gindex.edge=hedges;gindex.node=hnodes;
    ge_axes_handle.user_data=gindex
    edited=%t
  else
    edited=%f
  end
  ge_enablemenus()  
  drawnow();show_pixmap()
  edited=return(edited)
endfunction
