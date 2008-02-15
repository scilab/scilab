
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_do_undo(GraphList,op)

  if argn(2)<2 then
    ierr=execstr('global EGhist_'+w+';op=EGhist_'+w+'($);'+..
		 'EGhist_'+w+'($)=null()','errcatch')
    if ierr<>0 then return,end
  end
  drawlater()
  ge_axes_handle=gca();
  gindex=ge_axes_handle.user_data
  hedges=gindex.edge;hnodes=gindex.node;

  select  op(1)
   case "add_node" then
    K=op(2)
    karcs=find(GraphList.edges.tail==K|GraphList.edges.head==K)
    GraphList=ge_delete_node(GraphList,K)
    delete(hnodes(K));hnodes(K)=[];
    
    if karcs<>[] then //??
      GraphList.edges(karcs)=[];
      delete(hedges(karcs));hedges(karcs)=[];
    end
   case "add_arc" then
    K=op(2)
    GraphList.edges(K)=[];
    delete(hedges(K));hedges(K)=[];
    
   case "node_property" then
    k=op(2)
    karcs=find(GraphList.edges.tail==k|GraphList.edges.head==k)
    delete(hnodes(k));
    //update data structure
    for i=3:2:size(op)
      opi=op(i)
      if opi(1)=='graphics' then
	GraphList.nodes.graphics(opi(2))(:,k)=op(i+1); 
      else
	GraphList.nodes.data(opi(2))(:,k)=op(i+1); 
      end
    end
    // recreate the node
    hnodes(k)=ge_draw_node(k)
    if karcs<>[] then
      //update arcs
      modified=hedges(karcs)
      ge_update_edges(karcs,modified)
    end
   case "edge_property" then
    k=op(2)
    delete(hedges(k))
    for i=3:2:size(op)
      if op(i)(1)=='graphics' then
	GraphList.edges.graphics(op(i)(2))(:,k)=op(i+1);
      else
	GraphList.edges.data(op(i)(2))(:,k)=op(i+1);
      end
    end
    hedges(k)=ge_draw_edge(k)
   case "move" then
    ksel=op(2)
    karcs=find(GraphList.edges.tail==ksel|GraphList.edges.head==ksel)
    loops=find(GraphList.edges.tail(karcs)==GraphList.edges.head(karcs))
    kloops=karcs(loops)
    karcs(loops)=[]
    //selected node and loop arcs will be simply moved whilst other arcs
    //need to be recomputed
    moved=[hnodes(ksel);hedges(kloops)]
    nmoved=size(moved,'*')
    modified=hedges(karcs)
    x=GraphList.nodes.graphics.x(ksel);
    y=GraphList.nodes.graphics.y(ksel);
    GraphList.nodes.graphics.x(ksel)=op(3)(1)
    GraphList.nodes.graphics.y(ksel)=op(3)(2)
    for k=1:nmoved,move(moved(k),op(3)-[x y]),end
    ge_update_edges(karcs,modified)
   case "delete_edges" then
    n=size(GraphList.edges.tail,'*')
    // reorganize edges to make room edge to recreate
    del_arcs=op(2) //index of deleted arcs
    
    ind=1:n
    inc=0
    for k=del_arcs
      if k<=n+inc then ind(k-inc:n)=ind(k-inc:n)+1;inc=inc+1;end
    end
    k=3;
    //insert destroyed arcs at their original location in GraphList
    //data structure
    GraphList.edges(ind)=GraphList.edges;
    GraphList.edges(op(2))=op(3);
    
    
    //rebuild the associated data structures
    new_edges=[];
    for k=del_arcs
      e=ge_draw_edge(k);
      new_edges=[new_edges;e]
      ind($+1)=k;
    end
    // insert  the handles on the restred arcs at their original locations
    hedges=[hedges;new_edges];
    hedges(ind)= hedges
    
    // renumber  the arcs
    for i=del_arcs(1):size(hedges,'*')
      ak=hedges(i);
      ak.children(1).text=string(i);
    end
   case "delete_nodes" then
    n=size(GraphList.nodes)
    // reorganize node to make room edge to recreate
    ind=1:n
    inc=0
    del_nodes=op(2)
    
    //insert destroyed nodes at their original location in GraphList
    //data structure
    for k=del_nodes
      if k<=n+inc then 
	ind(k-inc:n)=ind(k-inc:n)+1;inc=inc+1
	s=find(GraphList.edges.tail>=k)
	if s<>[] then
	  GraphList.edges.tail(s,1)=GraphList.edges.tail(s)+1
	end
	s=find(GraphList.edges.head>=k)
	if s<>[] then
	  GraphList.edges.head(s,1)=GraphList.edges.head(s)+1
	end
      end
    end

    GraphList.nodes(ind)=GraphList.nodes
    GraphList.nodes(del_nodes)=op(3)
    
    //rebuild the associated data structures
    new_nodes=[];
    for k=del_nodes
      e=ge_draw_node(k)
      new_nodes=[new_nodes;e]
      ind($+1)=k;
    end

    // insert  the handles on the restred arcs at their original locations
    hnodes=[hnodes;new_nodes];
    hnodes(ind)= hnodes
    
    // renumber  the nodes
    for i=del_nodes(1):size(hnodes,'*')
      ak=hnodes(i);
      ak.children(1).text=string(i);
    end

    
   case "compound" then
    for k=2:size(op)
      GraphList=ge_do_undo(GraphList,op(k))
    end
    edited=return(%t)
   case "move_region" then
    ksel=op(2);
    karcs=find(dsearch(GraphList.edges.tail,ksel,'d')>0|...
	       dsearch(GraphList.edges.head,ksel,'d')>0)
    
    //find arcs connected with two internal node
    kinternals=find(dsearch(GraphList.edges.tail,ksel,'d')>0& ...
		    dsearch(GraphList.edges.head,ksel,'d')>0)
    //find arcs connected with only one internal node
    karcs(kinternals)=[]
    
    moved=[hnodes(ksel);hedges(kinternals)]
    nmoved=size(moved,'*')
    modified=hedges(karcs)
    x=GraphList.nodes.graphics.x(ksel);
    y=GraphList.nodes.graphics.y(ksel);
    GraphList.nodes.graphics.x(ksel)=GraphList.nodes.graphics.x(ksel)-op(3)(1);
    GraphList.nodes.graphics.y(ksel)=GraphList.nodes.graphics.y(ksel)-op(3)(2);
    for k=1:nmoved,move(moved(k),-op(3)),end
    ge_update_edges(karcs,modified)
   case "paste" then
    na=size(GraphList.edges)
    nn=size(GraphList.nodes)
    added=op(2)+1:nn;
    if added<>[] then delete(hnodes(added)),hnodes(added)=[];end
    added=op(3)+1:na
    if added<>[] then delete(hedges(added)),hedges(added)=[];end
    
    //remove
    GraphList=ge_delete_node(GraphList,op(2)+1:nn)
    GraphList.edges(op(3)+1:na)=[]
   case  "shortestpath" then
    for k=op(2)
      e=hedges(k);
      e.children(2).foreground=op(3)
      e.children(3).foreground=op(3)
    end
  end
  drawnow();show_pixmap()
  gindex.edge=hedges;gindex.node=hnodes;
  ge_axes_handle.user_data=gindex

endfunction


