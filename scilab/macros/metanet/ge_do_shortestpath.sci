function ge_do_shortestpath()
//Copyright INRIA
//Author : Serge Steer 2002

  // Copyright INRIA
  execstr('global EGdata_'+w+'; GraphList=EGdata_'+w+'.GraphList')
  GraphList.node_number=size(GraphList.node_x,'*')
  k1=[]
  while k1==[]
    [btn,xc,yc]=xclick()
    if btn==2 then return,end
    k1=ge_getnode(GraphList,[xc;yc])
  end
  b=GraphList.node_border(k1)
  if b==0 then b=GraphList.default_node_border,end
  GraphList.node_border(k1)=3*b
  
  ge_drawnodes(k1)

  k2=[]
  while k2==[]
    [btn,xc,yc]=xclick()
    if btn==2 then return,end
    k2=ge_getnode(GraphList,[xc;yc])
  end
  b=GraphList.node_border(k2)
  if b==0 then b=GraphList.default_node_border,end
  GraphList.node_border(k2)=3*b;ge_drawnodes(k2)

  
  l1=list('shortest path type',1,['Number of arcs','Arcs length'])
  l2=list('colors',5,string(1:xget("lastpattern")))
  rep=x_choices("Select shortest path type and highlighting color ",list(l1,l2))
  if rep==[] then return,end
  if rep(1)==1 then typ='arc',else typ='length',end
  [p,lp] = shortest_path(k1,k2,GraphList,typ)
  old=GraphList.edge_color(p)
  GraphList.edge_color(p)=rep(2)
  ge_drawarcs(p)
  
  //ge_add_history(list("shortestpath",p,old))
  //execstr('EGdata_'+w+'.GraphList=GraphList')
endfunction
