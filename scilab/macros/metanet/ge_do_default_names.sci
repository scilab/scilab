function ge_do_default_names()
//Copyright INRIA
//Author : Serge Steer 2002

  execstr(['global EGdata_'+w
	   'EGdata=EGdata_'+w
	   'ArcId=EGdata_'+w+'.ArcId'
	   'NodeId=EGdata_'+w+'.NodeId'])
  EGdata.GraphList.node_name=string(1:size(EGdata.GraphList.node_name,'*'))
  EGdata.GraphList.edge_name=string(1:size(EGdata.GraphList.edge_name,'*'))

    
    execstr('EGdata_'+w+'=EGdata')
    if ArcId<>0|NodeId<>0 then
      GraphList=EGdata.GraphList
      xbasc()
      ge_set_winsize()
      ge_drawobjs(GraphList),
    end
endfunction
