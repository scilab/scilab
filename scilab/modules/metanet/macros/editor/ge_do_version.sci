function GraphList=ge_do_version(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002

  for f=ge_node_fields()
    GraphList(f)=matrix(GraphList(f),1,-1)
  end
  for f=ge_arc_fields()
    GraphList(f)=matrix(GraphList(f),1,-1)
  end
  
endfunction
