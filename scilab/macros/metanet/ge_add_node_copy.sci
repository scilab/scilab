function GraphList=ge_add_node_copy(GraphList,K)
//Copyright INRIA
//Author : Serge Steer 2002

  for k=matrix(K,1,-1)
    for f=ge_node_fields()
      GraphList(f)($+1)=GraphList(f)(k)
    end
  end
endfunction
