function ge_do_options()
//Copyright INRIA
//Author : Serge Steer 2002

  execstr('global EGdata_'+w+';EGdata=EGdata_'+w)
  node=list('Nodes',EGdata.NodeId+1,['Nothing','Number','Name','Demand'])
  arc=list('Arcs',EGdata.ArcId+1,['Nothing','Number','Name','Cost','Min cap','Max cap',..
		     'Length', 'Quadratic weight','Quadratic origin', ...
		    'Weight'])
  graph=list('Is graph directed',EGdata.GraphList.directed+1,['No','Yes'])
  rep=x_choices("Select information to display",list(node,arc,graph))
  
  if EGdata.NodeId<>rep(1)-1 | ...
	EGdata.ArcId<>rep(2)-1 | ...
	EGdata.GraphList.directed<>rep(3)-1 then
    EGdata.NodeId=rep(1)-1
    EGdata.ArcId=rep(2)-1
    EGdata.GraphList.directed=rep(3)-1
    execstr('EGdata_'+w+'=EGdata')
    GraphList=EGdata.GraphList
    xbasc()
    ge_set_winsize()
    ge_drawobjs(GraphList),
  end
endfunction
