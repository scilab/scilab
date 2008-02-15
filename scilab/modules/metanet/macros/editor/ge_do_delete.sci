
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_do_delete(GraphList,xc,yc)

// ge_do_delete - delete a scicos object
// get first object to delete
//EGdata is passed by context
//!
  ge_disablemenus()
  //mprintf("number of edges %d before deletion\n",size(GraphList.edges))
  edited =%f
  [K,wh]=ge_getobj(GraphList,[xc;yc])
  if K==[] then return,end
  
  ge_axes_handle=gca();
  gindex=ge_axes_handle.user_data
  hedges=gindex.edge;hnodes=gindex.node;

  drawlater()
  edited=%t
  //mprintf("selected %s to delete %d\n",wh,K)
  if wh=='node' then
    karcs=find(GraphList.edges.tail==K|GraphList.edges.head==K)
    //mprintf("arcs to remove %s\n",strcat(string(karcs),','))
    //Preserve history
    if karcs<>[] then
      Hista=list("delete_edges",karcs,GraphList.edges(karcs));
    else
      Hista=list()
    end
    Histn=list("delete_nodes",K,GraphList.nodes(K))

    if size(Hista)==0 then
      ge_add_history(Histn)
    else
      ge_add_history(list("compound",Histn,Hista))
    end
    // renumber all the nodes display
    for k=K:size(GraphList.nodes)
      ak=hnodes(k)
      ak.children(1).text=string(k-1);
    end
    
    karcs=gsort(karcs,'g','i')

    // renumber all the arcs
    ka=[karcs size(GraphList.edges)]
    for i=1:size(ka,'*')-1
      for k=ka(i):ka(i+1)
	ak=hedges(k)
	ak.children(1).text=string(k-i);
      end
    end
    
    //delete the graphics entities associated with deleted node and arcs

    delete(hnodes(K));hnodes(K)=[];
    if karcs<>[] then
      delete(hedges(karcs));hedges(karcs)=[];
    end
    
    //delete the node and arcs out of the data structure
    GraphList=ge_delete_node(GraphList,K)
    GraphList.edges(karcs)=[]
    
  else //delete arc
    nt=GraphList.edges.tail(K)
    nh=GraphList.edges.head(K)
    //preverve history
    Hista=list("delete_edges",K,GraphList.edges(K))
    ge_add_history(Hista)
    
    // renumber all the arcs
    for k=K+1:size(GraphList.edges)
      ak=hedges(k)
      ak.children(1).text=string(k-1);
    end

    delete(hedges(K));hedges(K)=[]
    
    //delete the  arc out of the datastructure
    GraphList.edges(K)=[];
    sel=find((GraphList.edges.head==nt&GraphList.edges.tail==nh)|..
	     (GraphList.edges.head==nh&GraphList.edges.tail==nt))

  end
  //mprintf("number of edges %d after deletion\n",size(GraphList.edges))

  drawnow();show_pixmap()
  gindex.edge=hedges;gindex.node=hnodes;
  ge_axes_handle.user_data=gindex
  ge_enablemenus()
  edited=resume(%t)
endfunction
