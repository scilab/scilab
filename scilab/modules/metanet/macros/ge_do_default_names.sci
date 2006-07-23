function GraphList=ge_do_default_names(GraphList,ArcId,NodeId)
//Copyright INRIA
//Author : Serge Steer 2002
  r=x_message(['This operation will replaces ALL node and arc names'
	       'by their default values.'
	       'Do you really want to do it?'],['yes','no'])
  if r==1 then
    
    GraphList.node_name=string(1:size(GraphList.node_name,'*'))
    GraphList.edge_name=string(1:size(GraphList.edge_name,'*'))

    if ArcId<>0|NodeId<>0 then
      xbasc()
      ge_set_winsize()
      ge_drawobjs(GraphList),
    end
  end
endfunction
