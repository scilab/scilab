function ge_do_undo(op)
//Copyright INRIA
//Author : Serge Steer 2002
  if argn(2)<1 then
    ierr=execstr('global EGhist_'+w+';op=EGhist_'+w+'($);'+..
		 'EGhist_'+w+'($)=null()','errcatch')
    if ierr<>0 then return,end
  end
   execstr('global EGdata_'+w+'; GraphList=EGdata_'+w+'.GraphList')
   xset('alufunction',6)   
   select  op(1)
   case "add_node" then
     K=op(2)
     karcs=find(GraphList.tail==K|GraphList.head==K)
     ge_drawobjs(GraphList,K,karcs) //erase node and connected arcs
     GraphList=ge_delete_node(GraphList,K)
     GraphList=ge_delete_arc(GraphList,karcs)
   case "add_arc" then
     K=op(2)
     nt=GraphList.tail(K)
     nh=GraphList.head(K)
     sel=find((GraphList.head==nt&GraphList.tail==nh)|..
	      (GraphList.head==nh&GraphList.tail==nt))
     ge_drawarcs(sel) //erase
     GraphList=ge_delete_arc(GraphList,K)
     sel=find((GraphList.head==nt&GraphList.tail==nh)|..
	      (GraphList.head==nh&GraphList.tail==nt))
     ge_drawarcs(sel) //redraw
   case "node_property" then
     k=op(2)
     karcs=find(GraphList.tail==k|GraphList.head==k)
     ge_drawarcs(karcs);ge_drawnodes(k); //erase arcs and node
     for i=3:2:size(op)
       GraphList(op(i))(k)=op(i+1);
     end
     ge_drawnodes(k);ge_drawarcs(karcs) //redraw 
   case "edge_property" then
     k=op(2)
     T=GraphList.tail(k);H=GraphList.head(k)
     karcs=find((GraphList.tail==T&GraphList.head==H)|(GraphList.tail==H&GraphList.head==T))
     ge_drawarcs(karcs) //erase arc
     for i=3:2:size(op)
       GraphList(op(i))(k)=op(i+1);
     end
     ge_drawarcs(karcs) //redraw
   case "move" then
     ksel=op(2)
     karcs=find(GraphList.tail==ksel|GraphList.head==ksel)
     ge_drawobjs(GraphList,ksel,karcs) //erase
     GraphList.node_x(ksel)=op(3)(1)
     GraphList.node_y(ksel)=op(3)(2)
     ge_drawobjs(GraphList,ksel,karcs) //draw
   case "delete_edges" then
     n=size(GraphList.tail,'*')
     // reorganize edges to make room edge to recreate
     ind=1:n
     ge_drawarcs(min(op(2)):n)//erase
     inc=0
     for k=op(2)
       if k<=n+inc then ind(k-inc:n)=ind(k-inc:n)+1;inc=inc+1;end
     end
     k=3;
     for f=ge_arc_fields(),
       GraphList(f)(1,ind)=GraphList(f);
       GraphList(f)(1,op(2))=op(k);
       k=k+1;
     end
     ge_drawarcs(min(op(2)):size(GraphList.tail,'*'))
   case "delete_nodes" then
     n=size(GraphList.node_x,'*')
     // reorganize node to make room edge to recreate
     ind=1:n
     ge_drawnodes(min(op(2)):n)//erase
     inc=0
     for k=op(2)
       if k<=n+inc then 
	 ind(k-inc:n)=ind(k-inc:n)+1;inc=inc+1
	 s=find(GraphList.tail>=k)
	 if s<>[] then
	   GraphList.tail(s)=GraphList.tail(s)+1
	 end
	 s=find(GraphList.head>=k)
	 if s<>[] then
	   GraphList.head(s)=GraphList.head(s)+1
	 end
       end
     end
     k=3;
     for f=ge_node_fields(),
       GraphList(f)(1,ind)=GraphList(f);
       GraphList(f)(1,op(2))=op(k);
       k=k+1;
     end
     ge_drawnodes(min(op(2)):size(GraphList.node_x,'*'))
   case "compound" then
     for k=2:size(op)
       ge_do_undo(op(k))
     end
   case "move_region" then
     keep=op(2); karcs=[]
     for ksel=keep
       //get arcs connected to this node
       karcs=[karcs,find(GraphList.tail==ksel|GraphList.head==ksel)];
     end
     karcs=unique(karcs)
     ge_drawarcs(karcs);ge_drawnodes(keep) //erase
     GraphList.node_x(keep)=GraphList.node_x(keep)+op(3)(1);
     GraphList.node_y(keep)=GraphList.node_y(keep)+op(3)(2);
     ge_drawnodes(keep);ge_drawarcs(karcs) //redraw at the new position
   case "paste" then
     //erase
     na=size(GraphList.head,'*')
     nn=size(GraphList.node_x,'*')
     ge_drawarcs(op(3)+1:na)
     ge_drawnodes(op(2)+1:nn)
     //remove
     for f=ge_node_fields(),
       GraphList(f)(op(2)+1:nn)=[]
     end
     for f=ge_arc_fields(),
       GraphList(f)(op(3)+1:na)=[]
     end
   case  "shortestpath" then
     GraphList.edge_color(op(2))=op(3)
     ge_drawarcs(op(2))
   end

   xset('alufunction',3)
   if op(1)<> "compound" then 
     execstr('EGdata_'+w+'.GraphList=GraphList')
   end
   edited=return(%t)

endfunction

     
