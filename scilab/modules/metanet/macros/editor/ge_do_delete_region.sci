function GraphList=ge_do_delete_region(GraphList,xc,yc)
//Copyright INRIA
//Author : Serge Steer 2002
// EGData is passed by context
// Copyright INRIA
xinfo('Click, drag to select region and click to fix the selection')

ge_disablemenus()
[rect,btn]=rubberbox([xc,yc])
if btn==2 then  ge_enablemenus();return;end
[del,keep]=ge_get_nodes_in_rect(GraphList,rect)

edited=del<>[]
if edited then

  del_arcs=find(dsearch(GraphList.tail,del,'d')<>0|..
		dsearch(GraphList.head,del,'d')<>0)


 if del_arcs<>[] then
   Hista=list("delete_edges",del_arcs)
   for f=ge_arc_fields()
     Hista($+1)=GraphList(f)(del_arcs)
   end
 else
   Hista=list()
 end
 
 Histn=list("delete_nodes",del)
 for f=ge_node_fields() 
   Histn($+1)=GraphList(f)(del)
 end
 if size(Hista)==0 then
   ge_add_history(Histn)
 else
   ge_add_history(list("compound",Histn,Hista))
 end

 //erase deleted and renumbered objects 
 NodeId=EGdata.NodeId
 if NodeId==1 then 
   ge_drawnodes(min(del):$),// deleted and renumbered
 else
   ge_drawnodes(del)// deleted
 end
 ArcId=EGdata.ArcId
 if ArcId==1 then 
   ge_drawarcs(min(del_arcs):$),// deleted and renumbered
 else
   ge_drawarcs(del_arcs)// deleted
 end
   
 
 GraphList=ge_delete_arc(GraphList,del_arcs)
 GraphList=ge_delete_node(GraphList,del)
 //redraw renumbered objs

 if NodeId==1 then ge_drawnodes(min(del):$),end
 if ArcId==1 then ge_drawarcs(min(del_arcs):$),end 
 
 ge_enablemenus()
 edited=return(%t)
else
  ge_enablemenus()  
end


endfunction
