function GraphList=ge_do_delete(GraphList,xc,yc)
//Copyright INRIA
//Author : Serge Steer 2002

// ge_do_delete - delete a scicos object
// get first object to delete
  //EGdata is passed by context
//!
// Copyright INRIA
  edited =%f
  [K,wh]=ge_getobj(GraphList,[xc;yc])
  if K==[] then return,end

  edited=%t
  if wh=='node' then
    karcs=find(GraphList.tail==K|GraphList.head==K)
    //Preserve history
    if karcs<>[] then
      Hista=list("delete_edges",karcs)
      for f=ge_arc_fields()
      	Hista($+1)=GraphList(f)(karcs)
      end
    else
      Hista=list()
    end
    Histn=list("delete_nodes",K)
    for f=ge_node_fields() 
	Histn($+1)=GraphList(f)(K)
    end
    if size(Hista)==0 then
      ge_add_history(Histn)
    else
      ge_add_history(list("compound",Histn,Hista))
    end
    ArcId=EGdata.ArcId
    NodeId=EGdata.NodeId
    if ArcId==1 then 
      ge_drawarcs(min(karcs):$),// deleted and renumbered
    else
      ge_drawarcs(karcs)// deleted
    end
    if NodeId==1 then 
      ge_drawnodes(K:$),// deleted and renumbered
    else
      ge_drawnodes(K)// deleted
    end

    GraphList=ge_delete_node(GraphList,K)
    GraphList=ge_delete_arc(GraphList,karcs)
    if ArcId==1 then 
      ge_drawarcs(min(karcs):$),// deleted and renumbered
    end
    if NodeId==1 then 
      ge_drawnodes(K:$),// deleted and renumbered
    end
  else
    nt=GraphList.tail(K)
    nh=GraphList.head(K)
    //preverve history
    Hista=list("delete_edges",K)
    for f=ge_arc_fields()
      Hista($+1)=GraphList(f)(K)
    end
    ge_add_history(Hista)
    sel=find((GraphList.head==nt&GraphList.tail==nh)|..
	     (GraphList.head==nh&GraphList.tail==nt))
    
    ArcId=EGdata.ArcId
    if ArcId==1 then 
      ge_drawarcs(sel(sel<K))
      ge_drawarcs(K:$),// deleted and renumbered
    else
      ge_drawarcs(sel)// deleted
    end

    GraphList=ge_delete_arc(GraphList,K)
    sel=find((GraphList.head==nt&GraphList.tail==nh)|..
	     (GraphList.head==nh&GraphList.tail==nt))
    if ArcId==1 then 
      ge_drawarcs(sel(sel<K))
      ge_drawarcs(K:$),// deleted and renumbered
    end
  end
  edited=resume(%t)
endfunction
