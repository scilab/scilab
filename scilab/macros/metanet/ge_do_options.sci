function EGdata=ge_do_options(EGdata)
//Copyright INRIA
//Author : Serge Steer 2002

  node=list('Nodes',EGdata.NodeId+1,['Nothing','Number','Name','Demand','Label'])
  arc=list('Arcs',EGdata.ArcId+1,['Nothing','Number','Name','Cost','Min cap','Max cap',..
		     'Length', 'Quadratic weight','Quadratic origin', ...
		    'Weight','Label'])
  rep=x_choices("Select information to display",list(node,arc))
  
  if EGdata.NodeId<>rep(1)-1 | ...
	EGdata.ArcId<>rep(2)-1 then
    EGdata.NodeId=rep(1)-1
    EGdata.ArcId=rep(2)-1
  end
endfunction
